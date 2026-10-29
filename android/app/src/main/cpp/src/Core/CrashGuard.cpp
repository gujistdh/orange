#include "orange/Core/CrashGuard.h"
#include "orange/Core/Toast.h"

#include <csignal>
#include <cstring>

namespace Orange::Core
{
    namespace
    {
        char gCtx[0x120];
        int gCount = 0;
        volatile int gActive = 0;
        bool gInstalled = false;
        struct sigaction gPrevSegv;
        struct sigaction gPrevBus;
        struct sigaction gPrevAbrt;

        void Handler(int Signal)
        {
            (void)Signal;
            gActive = 1;
            std::_Exit(0);
        }
    }

    void CrashGuard::Install()
    {
        if (gInstalled)
        {
            return;
        }
        gInstalled = true;
        struct sigaction Action{};
        std::memset(&Action, 0, sizeof(Action));
        Action.sa_handler = Handler;
        Action.sa_flags = 0x08000004;
        sigemptyset(&Action.sa_mask);
        sigaction(SIGSEGV, &Action, &gPrevSegv);
        sigaction(SIGBUS, &Action, &gPrevBus);
        sigaction(SIGABRT, &Action, &gPrevAbrt);
    }

    bool CrashGuard::GuardActive()
    {
        return gActive != 0;
    }

    void* CrashGuard::Ctx()
    {
        return gCtx;
    }

    bool CrashGuard::AlreadyCrashed(const char* Where)
    {
        if (!Where)
        {
            return false;
        }
        for (int Index = 0; Index < gCount; ++Index)
        {
            if (std::strncmp(gCtx + kEntryStride * Index, Where, kMaxLength) == 0)
            {
                return true;
            }
        }
        return false;
    }

    void CrashGuard::ReportCrash(const char* Where)
    {
        if (Where && gCount < kMaxEntries)
        {
            std::strncpy(gCtx + kEntryStride * gCount, Where, kMaxLength);
            gCtx[kEntryStride * gCount + kMaxLength] = 0;
            ++gCount;
        }
        std::string Message = "[Crash] ";
        Message += Where ? Where : "";
        Toast::Show(Message);
    }
}
