#include "orange/Modules/Combat/KillAura.h"
#include "orange/Modules/State.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Core/Main.h"
#include "orange/Game/ModelFace.h"
#include "orange/Game/RenderCore.h"

#include <cstdlib>

namespace Orange::Modules::Combat
{
    using namespace Orange::Game;

    namespace
    {
        bool gEnabled = false;
        float gCps = 20.0f;
        float gRange = 5.0f;
        float gTargetCount = 3.0f;
        int gAttackMode = 0;
        bool gSwing = true;
        bool gFixed = false;
        int gPredict = 1;
        int gPriority = 0;
        bool gWallAttack = true;
        bool gTurnHead = false;
        float gTurnSpeed = 30.0f;
        int gTurnMode = 0;
        bool gInfoDisplay = true;
        bool gRangeAura = true;
        int gRangeAuraColorMode = 0;
        unsigned int gRangeAuraColor = 0xFFFFFFFFu;

        float gAttackAccumulator = 0.0f;
        void* gLastTarget = nullptr;
        void* gCurrentTarget = nullptr;
        float gTargetPos[3] = { 0.0f, 0.0f, 0.0f };
        std::vector<void*> gTargets;

        constexpr int kStateHookReady = 0x201;
    }

    bool KillAura::IsEnabled()
    {
        return gEnabled;
    }

    void KillAura::Reset()
    {
        gAttackAccumulator = 0.0f;
        gLastTarget = nullptr;
        gCurrentTarget = nullptr;
        gTargets.clear();
    }

    void KillAura::SetEnabled(float Cps, float Range, float TargetCount, int AttackMode,
                              bool Swing, bool Fixed, int Predict, int Priority,
                              int WallAttack, bool TurnHead, float TurnSpeed, int TurnMode)
    {
        gEnabled = static_cast<int>(Cps) != 0;
        gCps = Cps;
        gRange = Range;
        gTargetCount = TargetCount;
        gAttackMode = AttackMode;
        gSwing = Swing;
        gFixed = Fixed;
        gPredict = Predict;
        gPriority = Priority;
        gWallAttack = WallAttack != 0;
        gTurnHead = TurnHead;
        gTurnSpeed = TurnSpeed;
        gTurnMode = TurnMode;
    }

    void KillAura::SetAutoWeapon(bool Value) { (void)Value; }
    void KillAura::SetWallAttack(bool Value) { gWallAttack = Value; }
    void KillAura::SetViewMode(int Value) { gTurnMode = Value; }
    void KillAura::SetTurnHead(bool Value) { gTurnHead = Value; }
    void KillAura::SetTurnHeadMode(int Value) { gTurnMode = Value; }
    void KillAura::SetInfoDisplay(bool Value) { gInfoDisplay = Value; }
    void KillAura::SetRangeAura(bool Value) { gRangeAura = Value; }
    void KillAura::SetRangeAuraColor(int Mode, unsigned int Color)
    {
        gRangeAuraColorMode = Mode;
        gRangeAuraColor = Color;
    }

    bool KillAura::IsValidTarget(void* Actor)
    {
        if (!Actor)
        {
            return false;
        }
        if (!Game::Actor::IsAlive(Actor))
        {
            return false;
        }
        float Distance = DistanceTo(Actor);
        if (Distance > gRange)
        {
            return false;
        }
        if (!gWallAttack && !Game::Actor::CanSee(Actor))
        {
            return false;
        }
        return true;
    }

    float KillAura::DistanceTo(void* Actor)
    {
        float Pos[3] = { 0.0f, 0.0f, 0.0f };
        Game::Actor::GetPosition(Actor, Pos);
        float DeltaX = Pos[0] - gTargetPos[0];
        float DeltaY = Pos[1] - gTargetPos[1];
        float DeltaZ = Pos[2] - gTargetPos[2];
        return std::sqrt(DeltaX * DeltaX + DeltaY * DeltaY + DeltaZ * DeltaZ);
    }

    bool KillAura::CollectTargets()
    {
        gTargets.clear();
        std::vector<void*> Entities;
        Game::EntityList::GetEntities(Entities);
        for (void* Entity : Entities)
        {
            if (!Entity)
            {
                continue;
            }
            if (Entity == gCurrentTarget)
            {
                continue;
            }
            if (Game::TargetMask::IsDrop(Entity))
            {
                continue;
            }
            if (!IsValidTarget(Entity))
            {
                continue;
            }
            gTargets.push_back(Entity);
        }

        if (gPriority == 1)
        {
            std::stable_sort(gTargets.begin(), gTargets.end(), [](void* Left, void* Right)
            {
                return Game::Actor::GetHealth(Left) < Game::Actor::GetHealth(Right);
            });
        }
        else
        {
            std::stable_sort(gTargets.begin(), gTargets.end(), [](void* Left, void* Right)
            {
                return DistanceTo(Left) < DistanceTo(Right);
            });
        }
        return !gTargets.empty();
    }

    void KillAura::OnAttack(void* LocalPlayer, void* Target)
    {
        if (!LocalPlayer || !Target)
        {
            return;
        }
        Game::GameMode::Attack(LocalPlayer, Target);
        if (gSwing)
        {
            Game::Actor::Swing(LocalPlayer);
        }
    }

    void KillAura::AttackSingle(void* LocalPlayer)
    {
        if (gTargets.empty())
        {
            return;
        }
        void* Target = gTargets.front();
        if (gLastTarget && gLastTarget != Target)
        {
            gLastTarget = Target;
        }
        gCurrentTarget = Target;
        OnAttack(LocalPlayer, Target);
    }

    void KillAura::AttackMulti(void* LocalPlayer)
    {
        int Limit = static_cast<int>(gTargetCount);
        for (std::size_t Index = 0; Index < gTargets.size() && static_cast<int>(Index) < Limit; ++Index)
        {
            void* Target = gTargets[Index];
            if (!IsValidTarget(Target))
            {
                continue;
            }
            gCurrentTarget = Target;
            OnAttack(LocalPlayer, Target);
        }
    }

    void KillAura::AttackRotate(void* LocalPlayer)
    {
        if (gTargets.empty())
        {
            return;
        }
        int Count = static_cast<int>(gTargetCount);
        for (int Index = 0; Index < Count; ++Index)
        {
            std::size_t Pick = static_cast<std::size_t>(std::rand()) % gTargets.size();
            void* Target = gTargets[Pick];
            if (!IsValidTarget(Target))
            {
                continue;
            }
            gCurrentTarget = Target;
            OnAttack(LocalPlayer, Target);
        }
    }

    void KillAura::TurnToTarget(void* LocalPlayer, void* Target)
    {
        if (!gTurnHead || !LocalPlayer || !Target)
        {
            return;
        }
        Orange::Game::Vec3 Origin;
        Orange::Game::Vec3 Destination;
        if (!Orange::Game::RenderCore::ActorPosition(LocalPlayer, Origin))
        {
            return;
        }
        if (!Orange::Game::RenderCore::ActorCenter(Target, Destination))
        {
            return;
        }
        float DeltaX = Destination.X - Origin.X;
        float DeltaZ = Destination.Z - Origin.Z;
        float DeltaY = Destination.Y - (Origin.Y + 1.62f);
        float Horizontal = std::sqrt(DeltaX * DeltaX + DeltaZ * DeltaZ);
        float Yaw = Orange::Game::ModelFace::YawTo(0.0f, 0.0f, DeltaX, DeltaZ);
        float Pitch = Orange::Game::ModelFace::PitchTo(0.0f, 0.0f, DeltaY, Horizontal);
        if (gTurnMode == 0)
        {
            Orange::Game::ModelFace::Request(1, Yaw, Pitch);
        }
        else
        {
            Orange::Game::ModelFace::Request(2, Yaw, Pitch);
        }
    }

    void KillAura::RenderRangeAura()
    {
        if (!gRangeAura)
        {
            return;
        }
        unsigned int Color = gRangeAuraColorMode == 0 ? gRangeAuraColor : Game::Render::RainbowColor();
        Game::Glow::CircleFilled(gTargetPos[0], gTargetPos[1], gTargetPos[2], gRange, Color);
    }

    void KillAura::ProcessTick(void* LocalPlayer)
    {
        if (!gEnabled)
        {
            return;
        }
        if (!LocalPlayer)
        {
            return;
        }
        if (!Game::Actor::IsAlive(LocalPlayer))
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }

        gCurrentTarget = LocalPlayer;
        Game::Actor::GetPosition(LocalPlayer, gTargetPos);

        if (!GetFlag(kStateHookReady))
        {
            Game::Render::InstallHooks();
            SetFlag(kStateHookReady, true);
        }

        Reset();
        CollectTargets();
        RenderRangeAura();

        gAttackAccumulator += (gCps * (static_cast<float>(std::rand() % 31) / 100.0f + 0.85f)) / 20.0f;
        int AttackCount = static_cast<int>(gAttackAccumulator);
        gAttackAccumulator -= static_cast<float>(AttackCount);
        if (AttackCount <= 0)
        {
            return;
        }
        if (gTargets.empty())
        {
            return;
        }

        switch (gAttackMode)
        {
        case 1:
            AttackMulti(LocalPlayer);
            break;
        case 2:
            AttackRotate(LocalPlayer);
            break;
        default:
            AttackSingle(LocalPlayer);
            break;
        }

        if (gTurnHead && !gTargets.empty())
        {
            TurnToTarget(LocalPlayer, gTargets.front());
        }
    }
}
