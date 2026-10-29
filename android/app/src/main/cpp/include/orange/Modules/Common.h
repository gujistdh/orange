#pragma once

#include "orange/Common.h"
#include "orange/Gui/Module.h"

namespace Orange::Modules
{
    inline int ModuleIdFromName(const char* Name)
    {
        if (!Name || !*Name)
        {
            return 0;
        }
        unsigned int Hash = 2166136261u;
        for (const char* Cursor = Name; *Cursor; ++Cursor)
        {
            Hash = (Hash ^ static_cast<unsigned int>(*Cursor)) * 16777619u;
        }
        return static_cast<int>(Hash & 0x3FFFFFFFu);
    }
}
