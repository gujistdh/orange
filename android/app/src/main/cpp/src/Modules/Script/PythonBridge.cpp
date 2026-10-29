#include "orange/Modules/Script/PythonBridge.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Core/Main.h"

#include <fstream>

namespace Orange::Modules::Script
{
    using namespace Orange::Game;

    namespace
    {
        using GilEnsureFn = void (*)();
        using GilReleaseFn = void (*)();
        using ImportAddModuleFn = void* (*)(const char*);
        using ModuleGetDictFn = void* (*)(void*);
        using RunStringFlagsFn = int (*)(const char*, int, void*, void*);
        using ExecutionLockedFn = bool (*)();

        GilEnsureFn gGilEnsure = nullptr;
        GilReleaseFn gGilRelease = nullptr;
        ImportAddModuleFn gImportAddModule = nullptr;
        ModuleGetDictFn gModuleGetDict = nullptr;
        RunStringFlagsFn gRunStringFlags = nullptr;
        ExecutionLockedFn gExecutionLocked = nullptr;

        bool gReady = false;
        bool gEnabled = false;
        std::string gPath;
        std::mutex gPythonMutex;

        template <typename Fn>
        Fn Resolve(Orange::Offsets::Slot Index)
        {
            intptr_t Offset = Orange::Offsets::Load(Index);
            if (!Offset || !Orange::Core::BaseAddress)
            {
                return nullptr;
            }
            return reinterpret_cast<Fn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        }
    }

    bool PythonBridge::ResolveSymbols()
    {
        gGilEnsure = Resolve<GilEnsureFn>(Orange::Offsets::Slot::Python_gilEnsure);
        gGilRelease = Resolve<GilReleaseFn>(Orange::Offsets::Slot::Python_gilRelease);
        gImportAddModule = Resolve<ImportAddModuleFn>(Orange::Offsets::Slot::Python_importAddModule);
        gModuleGetDict = Resolve<ModuleGetDictFn>(Orange::Offsets::Slot::Python_moduleGetDict);
        gRunStringFlags = Resolve<RunStringFlagsFn>(Orange::Offsets::Slot::Python_runStringFlags);
        gExecutionLocked = Resolve<ExecutionLockedFn>(Orange::Offsets::Slot::Python_executionLocked);
        return gGilEnsure && gGilRelease && gImportAddModule && gModuleGetDict && gRunStringFlags && gExecutionLocked;
    }

    void PythonBridge::Init()
    {
        std::lock_guard<std::mutex> Lock(gPythonMutex);
        gReady = false;
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        gReady = ResolveSymbols();
    }

    bool PythonBridge::IsReady()
    {
        std::lock_guard<std::mutex> Lock(gPythonMutex);
        return gReady;
    }

    void PythonBridge::SetEnabled(bool Value)
    {
        std::lock_guard<std::mutex> Lock(gPythonMutex);
        gEnabled = Value;
    }

    bool PythonBridge::IsEnabled()
    {
        std::lock_guard<std::mutex> Lock(gPythonMutex);
        return gEnabled;
    }

    void PythonBridge::SetPath(const std::string& Value)
    {
        std::lock_guard<std::mutex> Lock(gPythonMutex);
        gPath = Value;
    }

    const std::string& PythonBridge::GetPath()
    {
        return gPath;
    }

    bool PythonBridge::EnsureGil()
    {
        if (!gGilEnsure)
        {
            return false;
        }
        gGilEnsure();
        return true;
    }

    void PythonBridge::ReleaseGil()
    {
        if (gGilRelease)
        {
            gGilRelease();
        }
    }

    bool PythonBridge::AddModule(const std::string& Name)
    {
        if (!gImportAddModule || Name.empty())
        {
            return false;
        }
        void* Module = gImportAddModule(Name.c_str());
        if (!Module || !gModuleGetDict)
        {
            return false;
        }
        return gModuleGetDict(Module) != nullptr;
    }

    bool PythonBridge::RunString(const std::string& Code, int Flags)
    {
        if (!gRunStringFlags || Code.empty())
        {
            return false;
        }
        return gRunStringFlags(Code.c_str(), Flags, nullptr, nullptr) == 0;
    }

    bool PythonBridge::RunFile(const std::string& FilePath)
    {
        std::ifstream Stream(FilePath, std::ios::binary);
        if (!Stream)
        {
            return false;
        }
        std::string Content((std::istreambuf_iterator<char>(Stream)), std::istreambuf_iterator<char>());
        return RunString(Content, 0);
    }

    void PythonBridge::ProcessTick(void* LocalPlayer)
    {
        if (!gEnabled || !gReady || gPath.empty())
        {
            return;
        }
        std::lock_guard<std::mutex> Lock(gPythonMutex);
        if (!EnsureGil())
        {
            return;
        }
        if (LocalPlayer)
        {
            char Buffer[96];
            std::snprintf(Buffer, sizeof(Buffer), "local_player = 0x%llx",
                          static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(LocalPlayer)));
            RunString(Buffer, 0);
        }
        RunString("_orange_tick()", 0);
        ReleaseGil();
    }
}
