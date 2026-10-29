#include "orange/Core/ArmMemHook.h"

#include <sys/mman.h>
#include <unistd.h>

namespace Orange::Core
{
    namespace
    {
        struct HookEntry
        {
            void* Target;
            void* Replace;
            void* Original;
        };

        HookEntry gEntries[64];
        int gCount = 0;

        void MakeWritable(void* Address, std::size_t Size)
        {
            long PageSize = sysconf(_SC_PAGESIZE);
            if (PageSize <= 0)
            {
                return;
            }
            uintptr_t Start = reinterpret_cast<uintptr_t>(Address) & ~static_cast<uintptr_t>(PageSize - 1);
            uintptr_t End = (reinterpret_cast<uintptr_t>(Address) + Size + PageSize - 1) & ~static_cast<uintptr_t>(PageSize - 1);
            mprotect(reinterpret_cast<void*>(Start), End - Start, PROT_READ | PROT_WRITE | PROT_EXEC);
        }
    }

    bool ArmMemHook::Hook(void* Target, void* Replace, void** Original)
    {
        if (!Target || !Replace || gCount >= 64)
        {
            return false;
        }
        MakeWritable(Target, 16);
        HookEntry& Entry = gEntries[gCount++];
        Entry.Target = Target;
        Entry.Replace = Replace;
        Entry.Original = Target;
        if (Original)
        {
            *Original = Target;
        }
        return true;
    }

    bool ArmMemHook::Unhook(void* Target)
    {
        for (int Index = 0; Index < gCount; ++Index)
        {
            if (gEntries[Index].Target == Target)
            {
                gEntries[Index] = gEntries[--gCount];
                return true;
            }
        }
        return false;
    }
}
