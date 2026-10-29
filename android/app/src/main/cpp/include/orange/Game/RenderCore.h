#pragma once

#include "orange/Common.h"

namespace Orange::Game
{
    struct Vec3
    {
        float X;
        float Y;
        float Z;
    };

    struct Camera
    {
        float Position[3];
        float Up[3];
        float Forward[3];
        float Right[3];
        float TanX;
        float TanY;
        float Near;
        float Padding;
    };

    struct ActorBox
    {
        Vec3 Min;
        Vec3 Max;
    };

    class RenderCore
    {
    public:
        static void Init();
        static void InstallHooks();
        static void SetModuleEnabled(bool Value);
        static bool AnyEnabled();
        static void AttachEarlyOrigin(void* Value);
        static void SwapEarly(void* Value);
        static void OnLevelRendererPlayer(Address Value);
        static void RefreshCamera();

        static bool GetCamera(Camera& Out);
        static bool ToView(const Camera& Cam, const Vec3& World, Vec3& Out);
        static bool ViewToScreen(const Camera& Cam, const Vec3& View, float& OutX, float& OutY, float Width, float Height);
        static bool ProjectPoint(const Camera& Cam, const Vec3& World, float& OutX, float& OutY, float Width, float Height);
        static bool WorldToScreen(const Vec3& World, float& OutX, float& OutY, float Width, float Height);

        static bool ActorPosition(void* Actor, Vec3& Out);
        static bool ActorHead(void* Actor, Vec3& Out);
        static bool ActorFeet(void* Actor, Vec3& Out);
        static bool ActorCenter(void* Actor, Vec3& Out);
        static bool GetActorBox(void* Actor, Vec3& Min, Vec3& Max);
        static bool TracerAnchor(float Width, float Height, float& OutX, float& OutY);

        static unsigned int Rainbow(float Hue);
        static unsigned int ColorFromMode(int Mode, unsigned int Color, float Hue);
        static unsigned int SwapChannels(unsigned int Color);

        static bool CollectTargets(std::vector<void*>& Out, int Mask, float Range);
    };
}
