#pragma once

#include "orange/Common.h"

namespace Orange::Game
{
    class ModelFace
    {
    public:
        static void Init();
        static void Request(int Slot, float Yaw, float Pitch);
        static void Clear(int Slot);
        static void OnRenderFrame();
        static void InstallHook();

        static float YawTo(float X1, float Y1, float X2, float Y2);
        static float PitchTo(float X1, float Y1, float Z1, float Distance);
        static float NormalizeAngle(float Degrees);
        static float CurrentYaw();
        static float CurrentPitch();
        static bool HasRequest();
        static void ApplyToLocalPlayer(void* LocalPlayer);
    };
}
