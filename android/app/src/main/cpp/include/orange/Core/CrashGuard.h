#pragma once

#include "orange/Common.h"

namespace Orange::Core
{
    class CrashGuard
    {
    public:
        static void Install();
        static bool GuardActive();
        static void* Ctx();
        static bool AlreadyCrashed(const char* Where);
        static void ReportCrash(const char* Where);

    private:
        static constexpr int kMaxEntries = 24;
        static constexpr int kEntryStride = 48;
        static constexpr int kMaxLength = 47;
    };
}
