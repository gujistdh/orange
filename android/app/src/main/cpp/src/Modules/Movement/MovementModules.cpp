#include "orange/Modules/Movement/MovementModules.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/RenderCore.h"
#include "orange/Core/Main.h"

#include <cmath>

namespace Orange::Modules::Movement
{
    using namespace Orange::Game;

    namespace
    {
        float ClampFloat(float Value, float Min, float Max)
        {
            if (Value < Min)
            {
                return Min;
            }
            if (Value > Max)
            {
                return Max;
            }
            return Value;
        }

        bool gScaffoldEnabled = false;
        bool gScaffoldAutoBlock = false;
        bool gScaffoldLockY = false;

        bool gJumpHackEnabled = false;
        float gJumpHackHeight = 1.0f;

        bool gClickTpEnabled = false;
        int gClickTpSerial = 0;

        bool gAutoJumpEnabled = false;
        bool gAutoJumpAirborne = false;

        bool gAutoGlideEnabled = false;
        bool gAutoGlideActive = false;

        bool gAutoSwimEnabled = false;
        bool gAutoSwimActive = false;

        bool gWaterWalkingEnabled = false;
        bool gWaterWalkingActive = false;

        bool gAutoForwardEnabled = false;
        bool gAutoSprintEnabled = false;
        float gAutoSprintSpeed = 1.0f;

        bool gPlayerMomentumEnabled = false;
        float gPlayerMomentumSpeed = 1.0f;
        float gPlayerMomentumFactor = 1.0f;

        bool gVoidReboundEnabled = false;
        int gVoidReboundDistance = 5;
        float gVoidReboundSpeed = 1.0f;

        bool gMountFlightEnabled = false;
        float gMountFlightMoveSpeed = 1.0f;
        float gMountFlightUpDownSpeed = 1.0f;
        bool gMountFlightDown = false;

        bool gRideRootFlightEnabled = false;
        float gRideRootFlightMoveSpeed = 1.0f;
        float gRideRootFlightUpDownSpeed = 1.0f;
        bool gRideRootFlightDown = false;
    }

    void Scaffold::SetEnabled(bool Value)
    {
        gScaffoldEnabled = Value;
    }

    void Scaffold::SetAutoBlock(bool Value)
    {
        gScaffoldAutoBlock = Value;
    }

    void Scaffold::SetLockY(bool Value)
    {
        gScaffoldLockY = Value;
        if (!Value)
        {
            gScaffoldAutoBlock = false;
        }
    }

    void Scaffold::ProcessTick(void* LocalPlayer)
    {
        if (!gScaffoldEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        Vec3 Position;
        if (!RenderCore::ActorPosition(LocalPlayer, Position))
        {
            return;
        }
        void* GameModePtr = Actor::GetGameMode(LocalPlayer);
        if (!GameModePtr)
        {
            return;
        }
        float BaseX = std::floor(Position.X);
        float BaseY = std::floor(Position.Y) - 1.0f;
        float BaseZ = std::floor(Position.Z);
        if (gScaffoldLockY)
        {
            BaseY = std::floor(Position.Y);
        }
        (void)BaseX;
        (void)BaseY;
        (void)BaseZ;
    }

    void JumpHack::Init()
    {
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
    }

    void JumpHack::SetEnabled(bool Enabled, float Height)
    {
        gJumpHackEnabled = Enabled;
        if (Height >= 0.0f && Height <= 10.0f)
        {
            gJumpHackHeight = Height;
        }
    }

    void JumpHack::ProcessTick(void* LocalPlayer)
    {
        if (!gJumpHackEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Actor::IsOnGround(LocalPlayer))
        {
            return;
        }
        Actor::LerpMotion(LocalPlayer, 0.0f, gJumpHackHeight * 0.42f, 0.0f);
    }

    void ClickTP::SetEnabled(bool Value)
    {
        gClickTpEnabled = Value;
        if (Value)
        {
            gClickTpSerial = 0;
        }
    }

    void ClickTP::ProcessTick(void* LocalPlayer)
    {
        if (!gClickTpEnabled || !LocalPlayer)
        {
            return;
        }
    }

    void AutoJump::SetEnabled(bool Value)
    {
        gAutoJumpEnabled = Value;
        if (!Value)
        {
            gAutoJumpAirborne = false;
        }
    }

    void AutoJump::ProcessTick(void* LocalPlayer)
    {
        if (!gAutoJumpEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        if (Actor::IsOnGround(LocalPlayer))
        {
            if (gAutoJumpAirborne)
            {
                gAutoJumpAirborne = false;
            }
            return;
        }
        gAutoJumpAirborne = true;
        Actor::LerpMotion(LocalPlayer, 0.0f, 0.42f, 0.0f);
    }

    bool AutoGlide::IsEnabled()
    {
        return gAutoGlideEnabled;
    }

    void AutoGlide::SetEnabled(bool Value)
    {
        if (gAutoGlideEnabled != Value)
        {
            gAutoGlideEnabled = Value;
            if (!Value)
            {
                gAutoGlideActive = false;
            }
        }
    }

    void AutoGlide::ProcessTick(void* LocalPlayer)
    {
        if (!gAutoGlideEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        void* GameModePtr = Actor::GetGameMode(LocalPlayer);
        if (!GameModePtr)
        {
            return;
        }
        if (Actor::IsOnGround(LocalPlayer))
        {
            return;
        }
        Player::StartGliding(LocalPlayer);
        gAutoGlideActive = true;
    }

    bool AutoSwim::IsEnabled()
    {
        return gAutoSwimEnabled;
    }

    void AutoSwim::SetEnabled(bool Value)
    {
        if (gAutoSwimEnabled != Value)
        {
            gAutoSwimEnabled = Value;
            if (!Value)
            {
                gAutoSwimActive = false;
            }
        }
    }

    void AutoSwim::ProcessTick(void* LocalPlayer)
    {
        if (!gAutoSwimEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        void* GameModePtr = Actor::GetGameMode(LocalPlayer);
        if (!GameModePtr)
        {
            return;
        }
        Actor::SetStatusFlag(LocalPlayer, Orange::Offsets::Slot::ActorFlags_SWIMMING, true);
        gAutoSwimActive = true;
    }

    bool WaterWalking::IsEnabled()
    {
        return gWaterWalkingEnabled;
    }

    void WaterWalking::SetEnabled(bool Value)
    {
        if (gWaterWalkingEnabled != Value)
        {
            gWaterWalkingEnabled = Value;
            if (!Value)
            {
                gWaterWalkingActive = false;
            }
        }
    }

    void WaterWalking::ProcessTick(void* LocalPlayer)
    {
        if (!gWaterWalkingEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        Vec3 Position;
        if (!RenderCore::ActorPosition(LocalPlayer, Position))
        {
            return;
        }
        Actor::LerpMotion(LocalPlayer, 0.0f, 0.02f, 0.0f);
        gWaterWalkingActive = true;
    }

    bool AutoForward::IsEnabled()
    {
        return gAutoForwardEnabled;
    }

    void AutoForward::SetEnabled(bool Value)
    {
        gAutoForwardEnabled = Value;
    }

    void AutoForward::ProcessTick(void* LocalPlayer)
    {
        if (!gAutoForwardEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        Vec3 Position;
        if (!RenderCore::ActorPosition(LocalPlayer, Position))
        {
            return;
        }
        Actor::LerpMotion(LocalPlayer, 0.0f, 0.0f, -0.15f);
    }

    bool AutoSprint::IsEnabled()
    {
        return gAutoSprintEnabled;
    }

    void AutoSprint::SetConfig(float Speed, bool Value)
    {
        gAutoSprintSpeed = ClampFloat(Speed, 1.0f, 10.0f);
        gAutoSprintEnabled = Value;
    }

    void AutoSprint::ProcessTick(void* LocalPlayer)
    {
        if (!gAutoSprintEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        if (!Actor::IsOnGround(LocalPlayer))
        {
            return;
        }
        Actor::SetStatusFlag(LocalPlayer, Orange::Offsets::Slot::ActorFlags_SPRINTING, true);
    }

    void PlayerMomentum::SetConfig(float Speed, float Factor, bool Value)
    {
        gPlayerMomentumSpeed = Speed;
        gPlayerMomentumFactor = Factor;
        gPlayerMomentumEnabled = Value;
    }

    void PlayerMomentum::ProcessTick(void* LocalPlayer)
    {
        if (!gPlayerMomentumEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        Vec3 Position;
        if (!RenderCore::ActorPosition(LocalPlayer, Position))
        {
            return;
        }
        Actor::LerpMotion(LocalPlayer, 0.0f, 0.0f, -gPlayerMomentumSpeed * gPlayerMomentumFactor * 0.01f);
    }

    void VoidRebound::SetConfig(bool Enabled, int Distance, float Speed, bool Value)
    {
        (void)Value;
        gVoidReboundEnabled = Enabled;
        gVoidReboundDistance = Distance;
        gVoidReboundSpeed = Speed;
    }

    void VoidRebound::ProcessTick(void* LocalPlayer)
    {
        if (!gVoidReboundEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        Vec3 Position;
        if (!RenderCore::ActorPosition(LocalPlayer, Position))
        {
            return;
        }
        if (Position.Y < static_cast<float>(gVoidReboundDistance) * -1.0f)
        {
            Actor::LerpMotion(LocalPlayer, 0.0f, gVoidReboundSpeed, 0.0f);
        }
    }

    bool MountFlight::IsEnabled()
    {
        return gMountFlightEnabled;
    }

    void MountFlight::SetConfig(float MoveSpeed, float UpDownSpeed, bool Value)
    {
        gMountFlightMoveSpeed = MoveSpeed;
        gMountFlightUpDownSpeed = UpDownSpeed;
        gMountFlightEnabled = Value;
    }

    void MountFlight::SetDownPressed(bool Value)
    {
        gMountFlightDown = Value;
    }

    void MountFlight::ProcessTick(void* LocalPlayer)
    {
        if (!gMountFlightEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        void* Vehicle = Actor::GetVehicle(LocalPlayer);
        if (!Vehicle)
        {
            return;
        }
        float Vertical = gMountFlightDown ? -gMountFlightUpDownSpeed : gMountFlightUpDownSpeed;
        Actor::LerpMotion(Vehicle, 0.0f, Vertical * 0.05f, 0.0f);
    }

    bool RideRootFlight::IsEnabled()
    {
        return gRideRootFlightEnabled;
    }

    void RideRootFlight::SetConfig(float MoveSpeed, float UpDownSpeed, bool Value)
    {
        gRideRootFlightMoveSpeed = MoveSpeed;
        gRideRootFlightUpDownSpeed = UpDownSpeed;
        gRideRootFlightEnabled = Value;
    }

    void RideRootFlight::SetDownPressed(bool Value)
    {
        gRideRootFlightDown = Value;
    }

    void RideRootFlight::ProcessTick(void* LocalPlayer)
    {
        if (!gRideRootFlightEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        void* RideRoot = Actor::GetRideRoot(LocalPlayer);
        if (!RideRoot)
        {
            return;
        }
        float Vertical = gRideRootFlightDown ? -gRideRootFlightUpDownSpeed : gRideRootFlightUpDownSpeed;
        Actor::LerpMotion(RideRoot, 0.0f, Vertical * 0.05f, 0.0f);
    }
}
