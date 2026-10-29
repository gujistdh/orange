#pragma once

#include "orange/Common.h"

namespace Orange::Game
{
    class IconAtlas
    {
    public:
        static void Init();
        static bool DecodeFile(const std::string& Path, int& OutWidth, int& OutHeight, std::vector<uint8_t>& OutRgba);
        static unsigned int LoadTexture(const std::string& Path);
        static unsigned int EnchantIcon(int EnchantId);
        static unsigned int TextureIcon();
        static void Release();
        static int CachedCount();

        static const char* const EnchantDir;
        static const char* const TextureDir;
    };
}
