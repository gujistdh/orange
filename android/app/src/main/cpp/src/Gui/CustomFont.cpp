#include "orange/Gui/CustomFont.h"

namespace Orange::Gui
{
    ImFont* CustomFont::Current = nullptr;

    const char* CustomFont::DefaultPath()
    {
        return "/storage/emulated/0/Android/data/com.netease.x19/files/Orange/font.ttf";
    }

    const char* CustomFont::Path()
    {
        return DefaultPath();
    }

    bool CustomFont::Reload(ImFontAtlas* Atlas, float Size, const uint16_t* Ranges, const uint16_t* Extra)
    {
        (void)Atlas;
        (void)Size;
        (void)Ranges;
        (void)Extra;
        return Current != nullptr;
    }

    void CustomFont::Release()
    {
        Current = nullptr;
    }

    ImFont* CustomFont::Get()
    {
        return Current;
    }
}
