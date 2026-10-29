#pragma once

#include "orange/Common.h"

namespace Orange::Core
{
    extern Address BaseAddress;
    extern Address BssAddress;

    Address FindLibBaseAddress(const std::string& Name);
    Address FindBssBaseAddress(const std::string& Name);

    void InitHooks(JNIEnv* Env);
    int GetPid();

    namespace Main
    {
        Address FindLibBaseAddress(const std::string& Name);
        Address FindBssBaseAddress(const std::string& Name);
        void InitHooks(JNIEnv* Env);
    }
}
