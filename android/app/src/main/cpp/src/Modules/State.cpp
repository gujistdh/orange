#include "orange/Modules/State.h"
#include <map>
#include <string>

namespace Orange::Modules
{
    namespace
    {
        Slot gSlots[kSlotCount];
        float SlotValue[kSlotCount];

        Slot* At(int Index)
        {
            if (Index < 0 || Index >= kSlotCount)
            {
                return nullptr;
            }
            return &gSlots[Index];
        }
    }

    namespace
    {
        constexpr int kModuleBase = 512;
        constexpr int kModuleStride = 8;
        std::map<std::string, int> gModuleSlots;
        int gNextModuleSlot = 0;
        int gEnabledModules = 0;
        std::map<std::string, std::string> gModuleText;
    }

    int ModuleSlot(const char* Name)
    {
        if (!Name || !*Name)
        {
            return -1;
        }
        auto It = gModuleSlots.find(Name);
        if (It != gModuleSlots.end())
        {
            return It->second;
        }
        if (gNextModuleSlot >= 60)
        {
            return -1;
        }
        int Slot = kModuleBase + gNextModuleSlot * kModuleStride;
        gModuleSlots[Name] = Slot;
        ++gNextModuleSlot;
        return Slot;
    }

    void SetModuleFlag(const char* Name, bool Enabled)
    {
        int Slot = ModuleSlot(Name);
        if (Slot < 0)
        {
            return;
        }
        bool Previous = GetFlag(Slot);
        if (Previous != Enabled)
        {
            if (Enabled) { ++gEnabledModules; }
            else if (gEnabledModules > 0) { --gEnabledModules; }
        }
        SetFlag(Slot, Enabled);
    }

    bool IsModuleEnabled(const char* Name)
    {
        int Slot = ModuleSlot(Name);
        return Slot < 0 ? false : GetFlag(Slot);
    }

    void SetModuleValue(const char* Name, int Index, float Value)
    {
        int Slot = ModuleSlot(Name);
        if (Slot < 0 || Index < 0 || Index >= kModuleStride - 1)
        {
            return;
        }
        SlotValue[Slot + 1 + Index] = Value;
    }

    float GetModuleValue(const char* Name, int Index)
    {
        int Slot = ModuleSlot(Name);
        if (Slot < 0 || Index < 0 || Index >= kModuleStride - 1)
        {
            return 0.0f;
        }
        return SlotValue[Slot + 1 + Index];
    }

    void SetModuleText(const char* Name, int Index, const std::string& Value)
    {
        int Slot = ModuleSlot(Name);
        if (Slot < 0 || Index < 0)
        {
            return;
        }
        std::string Key = std::string(Name) + "#" + std::to_string(Index);
        gModuleText[Key] = Value;
    }

    const std::string& GetModuleText(const char* Name, int Index)
    {
        static const std::string Empty;
        int Slot = ModuleSlot(Name);
        if (Slot < 0 || Index < 0)
        {
            return Empty;
        }
        std::string Key = std::string(Name) + "#" + std::to_string(Index);
        auto It = gModuleText.find(Key);
        return It == gModuleText.end() ? Empty : It->second;
    }

    int EnabledModuleCount()
    {
        return gEnabledModules;
    }

    void Reset()
    {
        for (int Index = 0; Index < kSlotCount; ++Index)
        {
            gSlots[Index] = Slot();
        }
    }

    bool GetFlag(int Index)
    {
        Slot* Target = At(Index);
        return Target ? Target->Flag : false;
    }

    void SetFlag(int Index, bool Value)
    {
        Slot* Target = At(Index);
        if (Target)
        {
            Target->Flag = Value;
        }
    }

    int GetCounter(int Index)
    {
        Slot* Target = At(Index);
        return Target ? Target->Counter : 0;
    }

    void SetCounter(int Index, int Value)
    {
        Slot* Target = At(Index);
        if (Target)
        {
            Target->Counter = Value;
        }
    }

    int AdvanceCounter(int Index, int Step)
    {
        Slot* Target = At(Index);
        if (!Target)
        {
            return 0;
        }
        Target->Counter += Step;
        return Target->Counter;
    }

    float GetValue(int Index)
    {
        Slot* Target = At(Index);
        return Target ? Target->Value : 0.0f;
    }

    void SetValue(int Index, float Value)
    {
        Slot* Target = At(Index);
        if (Target)
        {
            Target->Value = Value;
        }
    }

    int GetMode(int Index)
    {
        Slot* Target = At(Index);
        return Target ? Target->Mode : 0;
    }

    void SetMode(int Index, int Value)
    {
        Slot* Target = At(Index);
        if (Target)
        {
            Target->Mode = Value;
        }
    }

    const std::string& GetText(int Index)
    {
        static const std::string Empty;
        Slot* Target = At(Index);
        return Target ? Target->Text : Empty;
    }

    void SetText(int Index, const std::string& Value)
    {
        Slot* Target = At(Index);
        if (Target)
        {
            Target->Text = Value;
        }
    }
}
