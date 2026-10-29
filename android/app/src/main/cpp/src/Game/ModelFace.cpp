#include "orange/Game/ModelFace.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Core/Main.h"

#include <cmath>

namespace Orange::Game
{
    namespace
    {
        constexpr float kRadToDeg = 57.29577951308232f;
        constexpr float kMinDistance = 0.0001f;

        float gYaw[3] = { 0.0f, 0.0f, 0.0f };
        float gPitch[3] = { 0.0f, 0.0f, 0.0f };
        int gPending = 0;
        bool gDirty = false;
        float gAppliedYaw = 0.0f;
        float gAppliedPitch = 0.0f;
        bool gHasRequest = false;
        bool gHooked = false;
    }

    float ModelFace::NormalizeAngle(float Degrees)
    {
        float Wrapped = std::fmod(Degrees, 360.0f);
        if (Wrapped < 0.0f)
        {
            Wrapped += 360.0f;
        }
        if (Wrapped > 180.0f)
        {
            Wrapped -= 360.0f;
        }
        return Wrapped;
    }

    float ModelFace::YawTo(float X1, float Y1, float X2, float Y2)
    {
        return std::atan2(-(Y2 - Y1), X2 - X1) * kRadToDeg;
    }

    float ModelFace::PitchTo(float X1, float Y1, float Z1, float Distance)
    {
        float Safe = Distance < kMinDistance ? kMinDistance : Distance;
        (void)X1;
        (void)Y1;
        return NormalizeAngle(-std::atan2(Z1, Safe) * kRadToDeg);
    }

    void ModelFace::Init()
    {
        for (int Index = 0; Index < 3; ++Index)
        {
            gYaw[Index] = 0.0f;
            gPitch[Index] = 0.0f;
        }
        gPending = 0;
        gDirty = false;
        gAppliedYaw = 0.0f;
        gAppliedPitch = 0.0f;
        gHasRequest = false;
    }

    void ModelFace::Request(int Slot, float Yaw, float Pitch)
    {
        if (Slot <= 0 || Slot >= 3)
        {
            return;
        }
        gYaw[Slot] = NormalizeAngle(Yaw);
        gPitch[Slot] = NormalizeAngle(Pitch);
        gPending |= (Slot == 1) ? 2 : 4;
        gDirty = true;
        gHasRequest = true;
    }

    void ModelFace::Clear(int Slot)
    {
        if (Slot <= 0 || Slot >= 3)
        {
            return;
        }
        gPending = 0;
        gDirty = true;
        gHasRequest = false;
    }

    void ModelFace::ApplyToLocalPlayer(void* LocalPlayer)
    {
        if (!LocalPlayer)
        {
            return;
        }
        if (!gHasRequest)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::Actor_getRotation);
        if (!Offset)
        {
            return;
        }
        float* Rotation = reinterpret_cast<float*>(reinterpret_cast<Byte*>(LocalPlayer) + Offset);
        if (!Rotation)
        {
            return;
        }
        Rotation[0] = gAppliedYaw;
        Rotation[1] = gAppliedPitch;
    }

    void ModelFace::OnRenderFrame()
    {
        if (!gDirty)
        {
            return;
        }
        int Slot = 0;
        if ((gPending & 4) != 0)
        {
            Slot = 2;
        }
        else if ((gPending & 2) != 0)
        {
            Slot = 1;
        }
        if (Slot == 0)
        {
            gDirty = false;
            return;
        }
        gAppliedYaw = gYaw[Slot];
        gAppliedPitch = gPitch[Slot];
        ApplyToLocalPlayer(Actor::GetLocalPlayerInstance());
        gDirty = false;
    }

    void ModelFace::InstallHook()
    {
        if (gHooked)
        {
            return;
        }
        gHooked = true;
    }

    float ModelFace::CurrentYaw()
    {
        return gAppliedYaw;
    }

    float ModelFace::CurrentPitch()
    {
        return gAppliedPitch;
    }

    bool ModelFace::HasRequest()
    {
        return gHasRequest;
    }
}
