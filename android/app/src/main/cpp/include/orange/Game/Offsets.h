#pragma once

#include "orange/Common.h"

namespace Orange::Offsets
{
    enum class Slot : int
    {
#include "orange/Game/OffsetSlots.inc"
    };

    constexpr int kSlotCount = 264;

    struct Binding
    {
        const char* Name;
        void* Target;
        int Wide;
    };

    void Reset();
    void Store(const char* Name, intptr_t Value);
    intptr_t Load(Slot Index);
    intptr_t RawValue(const char* Name);
    bool IsBound(Slot Index);
    bool IsMissingKey(const char* Name);
    int StoredCount();
    int LoadedCount();
    int MissingCount();
    int MissingKeys();

    bool Bind(const Binding* Table, int Count);
    bool ApplyOverlay();
    void ZeroBound();

    bool IsReady();
    bool IsGame3100();
    void ResolveNow();

    int DetectedGameVersion();
    void SetDetectedGameVersion(int Value);

    const std::string& GetConfig(int Index);
    const std::string& GetConfigRaw(int Index);
    void ApplyFromJson(const std::string& Text);
}
