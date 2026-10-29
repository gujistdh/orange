#include "orange/Game/RenderCore.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/ModelFace.h"
#include "orange/Core/Main.h"

#include <cmath>

namespace Orange::Game
{
    namespace
    {
        Camera gCamera;
        bool gCameraValid = false;
        Address gEarlyOrigin = 0;
        Address gLevelRenderer = 0;
        int gEnabledCount = 0;
        bool gHooksInstalled = false;

        float Dot(const float* Left, const float* Right)
        {
            return Left[0] * Right[0] + Left[1] * Right[1] + Left[2] * Right[2];
        }

        bool IsValidFloat(float Value)
        {
            return std::isfinite(Value) && std::fabs(Value) > 0.0f;
        }
    }

    void RenderCore::Init()
    {
        gCameraValid = false;
        gEnabledCount = 0;
    }

    void RenderCore::InstallHooks()
    {
        if (gHooksInstalled)
        {
            return;
        }
        if (!Orange::Offsets::IsReady())
        {
            return;
        }
        gHooksInstalled = true;
    }

    void RenderCore::SetModuleEnabled(bool Value)
    {
        if (Value)
        {
            ++gEnabledCount;
            return;
        }
        if (gEnabledCount > 0)
        {
            --gEnabledCount;
        }
    }

    bool RenderCore::AnyEnabled()
    {
        return gEnabledCount > 0;
    }

    void RenderCore::AttachEarlyOrigin(void* Value)
    {
        gEarlyOrigin = reinterpret_cast<Address>(Value);
    }

    void RenderCore::SwapEarly(void* Value)
    {
        (void)Value;
        Address Origin = gEarlyOrigin;
        gEarlyOrigin = reinterpret_cast<Address>(Value);
        if (Origin == 0)
        {
            gEarlyOrigin = reinterpret_cast<Address>(Value);
        }
    }

    void RenderCore::OnLevelRendererPlayer(Address Value)
    {
        if (Value < 0x1000)
        {
            return;
        }
        gLevelRenderer = Value;
        ModelFace::OnRenderFrame();
    }

    void RenderCore::RefreshCamera()
    {
        if (!gLevelRenderer)
        {
            return;
        }
        gCameraValid = false;
    }

    bool RenderCore::GetCamera(Camera& Out)
    {
        if (!gCameraValid)
        {
            return false;
        }
        Out = gCamera;
        return true;
    }

    bool RenderCore::ToView(const Camera& Cam, const Vec3& World, Vec3& Out)
    {
        float Delta[3];
        Delta[0] = World.X - Cam.Position[0];
        Delta[1] = World.Y - Cam.Position[1];
        Delta[2] = World.Z - Cam.Position[2];
        Out.X = Dot(Delta, Cam.Forward);
        Out.Y = Dot(Delta, Cam.Right);
        Out.Z = Dot(Delta, Cam.Up);
        return true;
    }

    bool RenderCore::ViewToScreen(const Camera& Cam, const Vec3& View, float& OutX, float& OutY, float Width, float Height)
    {
        if (View.Z < Cam.Near)
        {
            return false;
        }
        if (Cam.TanX <= 0.000001f || Cam.TanY <= 0.000001f)
        {
            return false;
        }
        float NormalizedX = View.X / (View.Z * Cam.TanX);
        float NormalizedY = View.Y / (View.Z * Cam.TanY);
        if (!IsValidFloat(NormalizedX) || !IsValidFloat(NormalizedY))
        {
            return false;
        }
        OutX = (Width * 0.5f) * (NormalizedX + 1.0f);
        OutY = (Height * 0.5f) * (1.0f - NormalizedY);
        return true;
    }

    bool RenderCore::ProjectPoint(const Camera& Cam, const Vec3& World, float& OutX, float& OutY, float Width, float Height)
    {
        Vec3 View;
        if (!ToView(Cam, World, View))
        {
            return false;
        }
        return ViewToScreen(Cam, View, OutX, OutY, Width, Height);
    }

    bool RenderCore::WorldToScreen(const Vec3& World, float& OutX, float& OutY, float Width, float Height)
    {
        if (!gCameraValid)
        {
            return false;
        }
        return ProjectPoint(gCamera, World, OutX, OutY, Width, Height);
    }

    bool RenderCore::ActorPosition(void* Actor, Vec3& Out)
    {
        if (!Actor)
        {
            return false;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::Actor_PosPtr);
        if (!Offset)
        {
            return false;
        }
        void* PosPtr = *reinterpret_cast<void**>(reinterpret_cast<Byte*>(Actor) + Offset);
        if (!PosPtr)
        {
            return false;
        }
        const float* Source = static_cast<const float*>(PosPtr);
        Out.X = Source[0];
        Out.Y = Source[1];
        Out.Z = Source[2];
        return IsValidFloat(Out.X) && IsValidFloat(Out.Y) && IsValidFloat(Out.Z);
    }

    bool RenderCore::GetActorBox(void* Actor, Vec3& Min, Vec3& Max)
    {
        if (!Actor)
        {
            return false;
        }
        intptr_t ComponentOffset = Orange::Offsets::Load(Orange::Offsets::Slot::Actor_AABBComponent);
        intptr_t MinOffset = Orange::Offsets::Load(Orange::Offsets::Slot::AABBComponent_Min);
        intptr_t MaxOffset = Orange::Offsets::Load(Orange::Offsets::Slot::AABBComponent_Max);
        intptr_t DimOffset = Orange::Offsets::Load(Orange::Offsets::Slot::AABBComponent_Dim);
        if (!ComponentOffset || !MinOffset || !MaxOffset)
        {
            return false;
        }
        void* Component = *reinterpret_cast<void**>(reinterpret_cast<Byte*>(Actor) + ComponentOffset);
        if (!Component)
        {
            return false;
        }
        const float* MinSource = reinterpret_cast<const float*>(reinterpret_cast<Byte*>(Component) + MinOffset);
        const float* MaxSource = reinterpret_cast<const float*>(reinterpret_cast<Byte*>(Component) + MaxOffset);
        Min.X = MinSource[0];
        Min.Y = MinSource[1];
        Min.Z = MinSource[2];
        Max.X = MaxSource[0];
        Max.Y = MaxSource[1];
        Max.Z = MaxSource[2];
        (void)DimOffset;
        return true;
    }

    bool RenderCore::ActorHead(void* Actor, Vec3& Out)
    {
        Vec3 Min;
        Vec3 Max;
        if (!GetActorBox(Actor, Min, Max))
        {
            return false;
        }
        Out.X = (Min.X + Max.X) * 0.5f;
        Out.Y = Max.Y;
        Out.Z = (Min.Z + Max.Z) * 0.5f;
        return true;
    }

    bool RenderCore::ActorFeet(void* Actor, Vec3& Out)
    {
        Vec3 Min;
        Vec3 Max;
        if (!GetActorBox(Actor, Min, Max))
        {
            return false;
        }
        Out.X = (Min.X + Max.X) * 0.5f;
        Out.Y = Min.Y;
        Out.Z = (Min.Z + Max.Z) * 0.5f;
        return true;
    }

    bool RenderCore::ActorCenter(void* Actor, Vec3& Out)
    {
        Vec3 Min;
        Vec3 Max;
        if (!GetActorBox(Actor, Min, Max))
        {
            return false;
        }
        Out.X = (Min.X + Max.X) * 0.5f;
        Out.Y = (Min.Y + Max.Y) * 0.5f;
        Out.Z = (Min.Z + Max.Z) * 0.5f;
        return true;
    }

    bool RenderCore::TracerAnchor(float Width, float Height, float& OutX, float& OutY)
    {
        OutX = Width * 0.5f;
        OutY = Height * 0.5f;
        return true;
    }

    unsigned int RenderCore::Rainbow(float Hue)
    {
        float Wrapped = std::fmod(Hue, 360.0f);
        if (Wrapped < 0.0f)
        {
            Wrapped += 360.0f;
        }
        float Value = (1.0f - std::fabs(-2.0f * (Wrapped / 360.0f) + 1.0f)) * 70.0f + 215.0f;
        float Saturation = 1.0f - std::fabs(std::fmod(Value / 60.0f, 2.0f) - 1.0f);

        float Red = 0.0f;
        float Green = 0.0f;
        float Blue = 0.0f;
        if (Value >= 300.0f)
        {
            Red = 1.0f;
            Green = 0.0f;
            Blue = Saturation;
        }
        else if (Value >= 240.0f)
        {
            Red = Saturation;
            Green = 0.0f;
            Blue = 1.0f;
        }
        else if (Value >= 180.0f)
        {
            Red = 0.0f;
            Green = Saturation;
            Blue = 1.0f;
        }
        else if (Value >= 120.0f)
        {
            Red = 0.0f;
            Green = 1.0f;
            Blue = Saturation;
        }
        else if (Value >= 60.0f)
        {
            Red = Saturation;
            Green = 1.0f;
            Blue = 0.0f;
        }
        else
        {
            Red = 1.0f;
            Green = Saturation;
            Blue = 0.0f;
        }

        unsigned int A = 0xFFu;
        unsigned int R = static_cast<unsigned int>(Red * 255.0f);
        unsigned int G = static_cast<unsigned int>(Green * 255.0f);
        unsigned int B = static_cast<unsigned int>(Blue * 255.0f);
        return (A << 24) | (R << 16) | (G << 8) | B;
    }

    unsigned int RenderCore::SwapChannels(unsigned int Color)
    {
        unsigned int A = 0xFF000000u;
        unsigned int R = ((Color >> 16) & 0xFFu) << 16;
        unsigned int G = ((Color >> 8) & 0xFFu) << 8;
        unsigned int B = Color & 0xFFu;
        return A | (B << 16) | G | (R >> 16);
    }

    unsigned int RenderCore::ColorFromMode(int Mode, unsigned int Color, float Hue)
    {
        if (Mode == 1)
        {
            return Rainbow(Hue);
        }
        if (Mode == 2)
        {
            if (Hue > 20.0f)
            {
                return 0xFF0000FFu;
            }
            if (Hue > 10.0f)
            {
                return 0xFF00FFFFu;
            }
            return 0xFF00FF00u;
        }
        return SwapChannels(Color);
    }

    bool RenderCore::CollectTargets(std::vector<void*>& Out, int Mask, float Range)
    {
        Out.clear();
        std::vector<void*> Entities;
        EntityList::GetEntities(Entities);
        Vec3 Origin;
        void* Local = Actor::GetLocalPlayerInstance();
        if (!Local || !ActorPosition(Local, Origin))
        {
            return false;
        }
        for (void* Entity : Entities)
        {
            if (!Entity || Entity == Local)
            {
                continue;
            }
            if (!TargetMask::Allow(Mask, false, Entity))
            {
                continue;
            }
            Vec3 Position;
            if (!ActorPosition(Entity, Position))
            {
                continue;
            }
            float DeltaX = Position.X - Origin.X;
            float DeltaY = Position.Y - Origin.Y;
            float DeltaZ = Position.Z - Origin.Z;
            float Distance = std::sqrt(DeltaX * DeltaX + DeltaY * DeltaY + DeltaZ * DeltaZ);
            if (Distance > Range)
            {
                continue;
            }
            Out.push_back(Entity);
        }
        return !Out.empty();
    }
}
