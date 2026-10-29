#pragma once

#include "orange/Common.h"

namespace Orange::Modules
{
    struct Slot
    {
        bool Flag = false;
        int Counter = 0;
        float Value = 0.0f;
        int Mode = 0;
        std::string Text;
    };

    void Reset();

    bool GetFlag(int Index);
    void SetFlag(int Index, bool Value);

    int GetCounter(int Index);
    void SetCounter(int Index, int Value);
    int AdvanceCounter(int Index, int Step);

    float GetValue(int Index);
    void SetValue(int Index, float Value);

    int GetMode(int Index);
    void SetMode(int Index, int Value);

    const std::string& GetText(int Index);
    void SetText(int Index, const std::string& Value);

    int ModuleSlot(const char* Name);
    void SetModuleFlag(const char* Name, bool Enabled);
    bool IsModuleEnabled(const char* Name);
    void SetModuleValue(const char* Name, int Index, float Value);
    float GetModuleValue(const char* Name, int Index);
    void SetModuleText(const char* Name, int Index, const std::string& Value);
    const std::string& GetModuleText(const char* Name, int Index);
    int EnabledModuleCount();

    enum : int
    {
        kSlotCount = 1024,
    };
}
