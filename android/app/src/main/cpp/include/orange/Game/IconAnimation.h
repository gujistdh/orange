#pragma once

#include "orange/Common.h"

namespace Orange::Game
{
    class IconAnimation
    {
    public:
        static void Reset();
        static void Tick(float DeltaSeconds);
        static int FrameFor(int IconIndex);
        static int FrameForSpeed(int IconIndex, float FramesPerSecond);
        static unsigned int TextureFor(int IconIndex);
        static float Elapsed();
        static void SetGlobalSpeed(float Value);
        static float GlobalSpeed();
    };
}
