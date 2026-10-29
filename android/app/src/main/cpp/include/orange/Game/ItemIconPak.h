#pragma once

#include "orange/Common.h"

namespace Orange::Game
{
    struct PakIcon
    {
        int Width = 0;
        int Height = 0;
        int Offset = 0;
        int Length = 0;
        unsigned int Texture = 0;
    };

    class ItemIconPak
    {
    public:
        static bool Load(const std::string& Path);
        static bool IsLoaded();
        static int Count();
        static const PakIcon* At(int Index);
        static bool Decode(int Index, std::vector<uint8_t>& OutRgba);
        static unsigned int EnsureTexture(int Index);
        static void Release();
        static const std::string& SourcePath();
        static int FrameCount(int Index);
        static bool IsStrip(int Index);
        static bool DecodeFrame(int Index, int Frame, std::vector<uint8_t>& OutRgba);
        static unsigned int EnsureFrameTexture(int Index, int Frame);
    };
}
