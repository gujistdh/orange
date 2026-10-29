#include "orange/Game/Offsets.h"

namespace Orange::Offsets
{
    namespace
    {
        const char* const kNames[kSlotCount] = {
#include "orange/Game/OffsetNames.inc"
        };

        intptr_t gValues[kSlotCount];
        bool gBound[kSlotCount];

        const Binding* gBindings = nullptr;
        int gBindingCount = 0;
        int gBindingSlots[512];
        int gStoredCount = 0;
        int gDetectedVersion = 0;
        bool gReady = false;

        std::string gConfig;
        std::string gConfigRaw;

        int FindSlot(const char* Name)
        {
            if (!Name)
            {
                return -1;
            }
            for (int Index = 0; Index < kSlotCount; ++Index)
            {
                if (std::strcmp(kNames[Index], Name) == 0)
                {
                    return Index;
                }
            }
            return -1;
        }
    }

    void Reset()
    {
        for (int Index = 0; Index < kSlotCount; ++Index)
        {
            gValues[Index] = -1;
            gBound[Index] = false;
        }
        gStoredCount = 0;
        gReady = false;
        gBindings = nullptr;
        gBindingCount = 0;
    }

    void Store(const char* Name, intptr_t Value)
    {
        int Index = FindSlot(Name);
        if (Index < 0)
        {
            return;
        }
        if (!gBound[Index])
        {
            ++gStoredCount;
        }
        gValues[Index] = Value;
        gBound[Index] = true;
    }

    intptr_t Load(Slot Index)
    {
        int Raw = static_cast<int>(Index);
        if (Raw < 0 || Raw >= kSlotCount)
        {
            return -1;
        }
        return gValues[Raw];
    }

    intptr_t RawValue(const char* Name)
    {
        int Index = FindSlot(Name);
        return Index < 0 ? -1 : gValues[Index];
    }

    bool IsBound(Slot Index)
    {
        int Raw = static_cast<int>(Index);
        return Raw >= 0 && Raw < kSlotCount && gBound[Raw];
    }

    bool IsMissingKey(const char* Name)
    {
        return FindSlot(Name) < 0;
    }

    int StoredCount()
    {
        return gStoredCount;
    }

    int LoadedCount()
    {
        return gStoredCount;
    }

    int MissingCount()
    {
        return kSlotCount - gStoredCount;
    }

    int MissingKeys()
    {
        return kSlotCount - gStoredCount;
    }

    bool Bind(const Binding* Table, int Count)
    {
        if (!Table || Count <= 0)
        {
            return false;
        }
        if (Count > 512)
        {
            Count = 512;
        }
        gBindings = Table;
        gBindingCount = Count;
        for (int Index = 0; Index < gBindingCount; ++Index)
        {
            gBindingSlots[Index] = FindSlot(Table[Index].Name);
        }
        return true;
    }

    bool ApplyOverlay()
    {
        if (!gBindings || gBindingCount <= 0)
        {
            return false;
        }
        if (gStoredCount <= 0)
        {
            return false;
        }
        for (int Index = 0; Index < gBindingCount; ++Index)
        {
            void* Target = gBindings[Index].Target;
            if (!Target)
            {
                continue;
            }
            int SlotIndex = gBindingSlots[Index];
            if (SlotIndex < 0 || !gBound[SlotIndex])
            {
                if (gBindings[Index].Wide)
                {
                    *static_cast<int*>(Target) = 0;
                }
                else
                {
                    *static_cast<intptr_t*>(Target) = 0;
                }
                continue;
            }
            if (gBindings[Index].Wide)
            {
                *static_cast<int*>(Target) = static_cast<int>(gValues[SlotIndex]);
            }
            else
            {
                *static_cast<intptr_t*>(Target) = gValues[SlotIndex];
            }
        }
        gReady = true;
        return true;
    }

    void ZeroBound()
    {
        for (int Index = 0; Index < kSlotCount; ++Index)
        {
            if (gBound[Index])
            {
                gValues[Index] = 0;
            }
        }
    }

    bool IsReady()
    {
        return gReady;
    }

    bool IsGame3100()
    {
        return gStoredCount > 0 && gDetectedVersion == 3100;
    }

    void ResolveNow()
    {
        ApplyOverlay();
    }

    int DetectedGameVersion()
    {
        return gDetectedVersion;
    }

    void SetDetectedGameVersion(int Value)
    {
        gDetectedVersion = Value;
    }

    const std::string& GetConfig(int Index)
    {
        (void)Index;
        return gConfig;
    }

    const std::string& GetConfigRaw(int Index)
    {
        (void)Index;
        return gConfigRaw;
    }

    void ApplyFromJson(const std::string& Text)
    {
        gConfigRaw = Text;
        gConfig = Text;
    }
}
