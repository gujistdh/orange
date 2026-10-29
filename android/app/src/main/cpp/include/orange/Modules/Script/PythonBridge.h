#pragma once

#include "orange/Common.h"

namespace Orange::Modules::Script
{
    class PythonBridge
    {
    public:
        static void Init();
        static bool IsReady();
        static void SetEnabled(bool Value);
        static bool IsEnabled();
        static void SetPath(const std::string& Value);
        const std::string& GetPath();

        static bool EnsureGil();
        static void ReleaseGil();
        static bool AddModule(const std::string& Name);
        static bool RunString(const std::string& Code, int Flags);
        static bool RunFile(const std::string& Path);
        static void ProcessTick(void* LocalPlayer);

    private:
        static bool ResolveSymbols();
    };
}
