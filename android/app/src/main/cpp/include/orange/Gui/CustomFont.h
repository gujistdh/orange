#pragma once

#include "orange/Common.h"

struct ImFont;
struct ImFontAtlas;

namespace Orange::Gui
{
    class CustomFont
    {
    public:
        static const char* DefaultPath();
        static const char* Path();

        static bool Reload(ImFontAtlas* Atlas, float Size, const uint16_t* Ranges, const uint16_t* Extra);
        static void Release();

        static ImFont* Get();

    private:
        static ImFont* Current;
    };
}
