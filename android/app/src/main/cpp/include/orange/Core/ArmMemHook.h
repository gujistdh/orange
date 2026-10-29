#pragma once

#include "orange/Common.h"

namespace Orange::Core
{
    class ArmMemHook
    {
    public:
        static bool Hook(void* Target, void* Replace, void** Original);
        static bool Unhook(void* Target);
    };
}
