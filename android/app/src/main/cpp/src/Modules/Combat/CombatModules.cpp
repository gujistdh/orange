#include "orange/Modules/Combat/CombatModules.h"
#include "orange/Modules/Combat/KillAura.h"
#include "orange/Modules/State.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/RenderCore.h"
#include "orange/Core/Main.h"

#include <chrono>
#include <cmath>
#include <cstdlib>

namespace Orange::Modules::Combat
{
    using namespace Orange::Game;

    namespace
    {
        int ClampInt(int Value, int Min, int Max)
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

        float DistanceBetween(const Orange::Game::Vec3& Left, const Orange::Game::Vec3& Right)
        {
            float DeltaX = Left.X - Right.X;
            float DeltaY = Left.Y - Right.Y;
            float DeltaZ = Left.Z - Right.Z;
            return std::sqrt(DeltaX * DeltaX + DeltaY * DeltaY + DeltaZ * DeltaZ);
        }
    }

    namespace
    {
        bool gAttackCountEnabled = false;
        int gAttackCountValue = 1;
        int gAttackCountMask = 0;
        bool gAttackCountFlag = false;
        float gAttackCountDelay = 0.0f;
        void* gAttackCountLastTarget = nullptr;

        bool gAttackDamageEnabled = false;
        int gAttackDamageAmount = 0;
        int gAttackDamageMask = 0;
        void* gAttackDamagePending = nullptr;

        bool gKnockbackEnabled = false;
        float gKnockbackHealth = -1.0f;
        bool gKnockbackReady = false;

        bool gKillMessageEnabled = false;
        std::string gKillMessageText;
        bool gAttackMessageEnabled = false;
        std::string gAttackMessageText;

        bool gAuraRingEnabled = false;
        Orange::Game::Vec3 gAuraRingPosition{ 0.0f, 0.0f, 0.0f };
        bool gAuraRingValid = false;

        bool gAutoBowEnabled = false;
        bool gCounterattackEnabled = false;
        int gCounterattackCount = 1;
        int gCounterattackMask = 0;
        bool gCounterattackFlag = false;
        float gCounterattackDelay = 0.0f;

        bool gHitboxEnabled = false;
        int gHitboxMask = 0;
        float gHitboxScale = 1.0f;

        bool gSpacingEnabled = false;
        float gSpacingDistance = 3.0f;

        float gCircleSpeed = 1.0f;
        float gCircleRange = 5.0f;
        float gCircleRadius = 3.0f;
        int gCircleMask = 0;
        bool gCircleFlag1 = false;
        bool gCircleFlag2 = false;
        bool gCircleFlag3 = false;
        float gCircleAngle = 0.0f;

        bool gTrapEnabled = false;
        float gTrapRange = 3.0f;
        bool gTrapFlag = false;
        int gTrapMode = 0;

        bool gSelfTrapEnabled = false;
        int gSelfTrapMode = 0;

        bool gKillisEnabled = false;
        int gKillisMode = 0;

        bool gInfiniteAuraEnabled = false;
        int gInfiniteAuraTargetMode = 0;
        int gInfiniteAuraRange = 100;
        int gInfiniteAuraCount = 40;
        float gInfiniteAuraDelay = 1000.0f;
        int gInfiniteAuraTimes = 10;
        int gInfiniteAuraPullBack = 500;
        bool gInfiniteAuraFixed = false;
        bool gInfiniteAuraSwing = true;
        bool gInfiniteAuraRiding = false;

        bool gAutoFollowEnabled = false;
        float gAutoFollowOffset[3] = { 0.0f, 0.0f, 0.0f };
        int gAutoFollowRange = 100;
        float gAutoFollowSpeed = 1.0f;

        bool gAttackEffectsEnabled = false;
        int gAttackEffectsMask = 0;
        float gAttackEffectsSize = 1.0f;
        int gAttackEffectsMode = 0;
        float gAttackEffectsDuration = 1.0f;
        int gAttackEffectsCount = 1;
        int gAttackEffectsStyle = 0;
        void* gAttackEffectsTarget = nullptr;
        float gAttackEffectsTimer = 0.0f;
    }

    void AttackCount::SetConfig(bool Enabled, int Count, int TargetMask, bool Flag, float Delay)
    {
        gAttackCountEnabled = Enabled;
        gAttackCountValue = ClampInt(Count, 0, 10);
        gAttackCountMask = TargetMask::Normalize(TargetMask);
        gAttackCountFlag = Flag;
        gAttackCountDelay = Delay;
    }

    void AttackCount::OnAttackEvent(void* Target, bool IsPlayer)
    {
        if (!gAttackCountEnabled || !IsPlayer)
        {
            return;
        }
        if (gAttackCountFlag)
        {
            return;
        }
        if (!Target || Target == gAttackCountLastTarget)
        {
            return;
        }
        int Repeat = gAttackCountValue - 1;
        if (Repeat > 0)
        {
            void* Local = Actor::GetLocalPlayerInstance();
            for (int Index = 0; Index < Repeat; ++Index)
            {
                GameMode::Attack(Local, Target);
            }
        }
    }

    void AttackCount::ProcessTick(void* LocalPlayer)
    {
        if (!gAttackCountEnabled || !LocalPlayer)
        {
            return;
        }
        gAttackCountLastTarget = nullptr;
    }

    void AttackDamage::SetConfig(bool Enabled, int Amount, int TargetMask)
    {
        gAttackDamageEnabled = Enabled;
        gAttackDamageAmount = ClampInt(Amount, 0, 1000);
        gAttackDamageMask = TargetMask::Normalize(TargetMask);
        if (!Enabled)
        {
            gAttackDamagePending = nullptr;
        }
    }

    void AttackDamage::OnAttackEvent(void* Target, bool IsPlayer)
    {
        if (!gAttackDamageEnabled || !IsPlayer)
        {
            return;
        }
        if (gAttackDamagePending)
        {
            return;
        }
        if (Target)
        {
            gAttackDamagePending = Target;
        }
    }

    void AttackDamage::ProcessTick(void* LocalPlayer)
    {
        if (!gAttackDamageEnabled || !gAttackDamagePending)
        {
            return;
        }
        if (!LocalPlayer)
        {
            return;
        }
        for (int Index = 0; Index < gAttackDamageAmount; ++Index)
        {
            GameMode::Attack(LocalPlayer, gAttackDamagePending);
        }
        gAttackDamagePending = nullptr;
    }

    void Knockback::Init()
    {
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        gKnockbackReady = true;
    }

    void Knockback::SetEnabled(bool Value)
    {
        if (gKnockbackEnabled != Value)
        {
            gKnockbackHealth = -1.0f;
        }
        gKnockbackEnabled = Value;
    }

    void Knockback::ProcessTick(void* LocalPlayer)
    {
        if (!gKnockbackEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        float Health = Actor::GetHealth(LocalPlayer);
        if (Health < 0.0f || Health > 20.0f)
        {
            return;
        }
        if (gKnockbackHealth >= 0.0f && Health < gKnockbackHealth - 0.01f)
        {
            Actor::LerpMotion(LocalPlayer, 0.0f, 0.0f, 0.0f);
        }
        gKnockbackHealth = Health;
    }

    void KillMessage::SetEnabled(bool Enabled, const std::string& Text)
    {
        gKillMessageEnabled = Enabled;
        gKillMessageText = Text;
    }

    void KillMessage::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gKillMessageEnabled)
        {
            return;
        }
    }

    void KillMessage::OnAttackEvent(void* Target)
    {
        if (!gKillMessageEnabled || !Target)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        if (Actor::GetHealth(Target) <= 0.0f)
        {
            Game::Text::SendChat(gKillMessageText);
        }
    }

    void AttackMessage::SetEnabled(bool Enabled, const std::string& Text)
    {
        gAttackMessageEnabled = Enabled;
        gAttackMessageText = Text;
    }

    void AttackMessage::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gAttackMessageEnabled)
        {
            return;
        }
    }

    void AttackMessage::OnAttackEvent(void* Target, bool IsPlayer)
    {
        if (!gAttackMessageEnabled || !IsPlayer || !Target)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        Game::Text::SendChat(gAttackMessageText);
    }

    void AuraRing::SetEnabled(bool Value)
    {
        gAuraRingEnabled = Value;
        if (!Value)
        {
            gAuraRingValid = false;
        }
    }

    void AuraRing::ProcessTick(void* LocalPlayer)
    {
        if (!gAuraRingEnabled || !LocalPlayer)
        {
            return;
        }
        Game::Render::InstallHooks();
        Orange::Game::Vec3 Feet;
        if (!Orange::Game::RenderCore::ActorFeet(LocalPlayer, Feet))
        {
            return;
        }
        gAuraRingPosition = Feet;
        gAuraRingValid = true;
    }

    void AuraRing::Draw()
    {
        if (!gAuraRingEnabled || !gAuraRingValid)
        {
            return;
        }
        Game::Glow::CircleFilled(gAuraRingPosition.X, gAuraRingPosition.Y, gAuraRingPosition.Z, 1.5f, 0xFFFFFFFFu);
    }

    void AutoBow::SetEnabled(bool Value) { gAutoBowEnabled = Value; }
    bool AutoBow::IsEnabled() { return gAutoBowEnabled; }

    void AutoBow::ProcessTick(void* LocalPlayer)
    {
        if (!gAutoBowEnabled || !LocalPlayer)
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
        void* Carried = Player::GetCarriedItem(LocalPlayer);
        if (!Carried || ItemStack::IsNull(Carried))
        {
            return;
        }
        Player::StartUsingItem(LocalPlayer, 0);
    }

    void Counterattack::SetConfig(bool Enabled, int Count, int TargetMask, bool Flag, float Delay)
    {
        gCounterattackEnabled = Enabled;
        gCounterattackCount = Count <= 0 ? 1 : ClampInt(Count, 1, 10);
        gCounterattackMask = TargetMask::Normalize(TargetMask);
        gCounterattackFlag = Flag;
        gCounterattackDelay = Delay;
    }

    void Counterattack::ProcessTick(void* LocalPlayer)
    {
        if (!gCounterattackEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        if (!Orange::Offsets::Load(Orange::Offsets::Slot::KillAura_attack))
        {
            return;
        }
        float Health = Actor::GetHealth(LocalPlayer);
        if (Health <= 0.0f)
        {
            return;
        }
    }

    void Hitbox::SetConfig(bool Enabled, int TargetMask, float Scale)
    {
        gHitboxEnabled = Enabled;
        gHitboxMask = TargetMask::Normalize(TargetMask);
        gHitboxScale = ClampFloat(Scale, 1.0f, 20.0f);
    }

    void Hitbox::ProcessTick(void* LocalPlayer)
    {
        if (!gHitboxEnabled || !LocalPlayer)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::Actor_setAABBDim);
        if (!Offset)
        {
            return;
        }
        using SetDimFn = void (*)(void*, float);
        SetDimFn SetDim = reinterpret_cast<SetDimFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        if (SetDim)
        {
            SetDim(LocalPlayer, gHitboxScale);
        }
    }

    void Spacing::SetEnabled(bool Enabled, float Distance, bool Flag, bool Extra)
    {
        (void)Flag;
        (void)Extra;
        gSpacingEnabled = Enabled;
        gSpacingDistance = Distance;
    }

    void Spacing::ProcessTick(void* LocalPlayer)
    {
        if (!gSpacingEnabled || !LocalPlayer)
        {
            return;
        }
        Orange::Game::Vec3 Origin;
        if (!Orange::Game::RenderCore::ActorPosition(LocalPlayer, Origin))
        {
            return;
        }
        std::vector<void*> Targets;
        Orange::Game::RenderCore::CollectTargets(Targets, gHitboxMask, 20.0f);
        for (void* Target : Targets)
        {
            Orange::Game::Vec3 Position;
            if (!Orange::Game::RenderCore::ActorPosition(Target, Position))
            {
                continue;
            }
            if (DistanceBetween(Origin, Position) < gSpacingDistance)
            {
                Actor::LerpMotion(LocalPlayer, 0.0f, 0.0f, 0.0f);
                break;
            }
        }
    }

    void Circle::SetConfig(float Speed, float Range, float Radius, int TargetMask, bool Flag1, bool Flag2, bool Flag3)
    {
        gCircleSpeed = Speed;
        gCircleRange = Range;
        gCircleRadius = Radius;
        gCircleMask = TargetMask::Normalize(TargetMask);
        gCircleFlag1 = Flag1;
        gCircleFlag2 = Flag2;
        gCircleFlag3 = Flag3;
        gCircleAngle = 0.0f;
    }

    void Circle::ProcessTick(void* LocalPlayer)
    {
        if (!LocalPlayer)
        {
            return;
        }
        gCircleAngle += gCircleSpeed;
        if (gCircleAngle >= 360.0f)
        {
            gCircleAngle -= 360.0f;
        }
        Orange::Game::Vec3 Origin;
        if (!Orange::Game::RenderCore::ActorPosition(LocalPlayer, Origin))
        {
            return;
        }
        float Radians = gCircleAngle * 0.017453f;
        Orange::Game::Vec3 Target;
        Target.X = Origin.X + std::cos(Radians) * gCircleRadius;
        Target.Y = Origin.Y;
        Target.Z = Origin.Z + std::sin(Radians) * gCircleRadius;
        (void)Target;
    }

    void Trap::SetEnabled(bool Enabled, float Range, bool Flag, int Mode)
    {
        gTrapEnabled = Enabled;
        gTrapRange = Range;
        gTrapFlag = Flag;
        gTrapMode = Mode;
    }

    void Trap::ProcessTick(void* LocalPlayer)
    {
        if (!gTrapEnabled || !LocalPlayer)
        {
            return;
        }
        Orange::Game::Vec3 Origin;
        if (!Orange::Game::RenderCore::ActorPosition(LocalPlayer, Origin))
        {
            return;
        }
        void* GameModePtr = Actor::GetGameMode(LocalPlayer);
        if (!GameModePtr)
        {
            return;
        }
        int Radius = static_cast<int>(gTrapRange);
        if (Radius < 1)
        {
            Radius = 1;
        }
        if (Radius > 5)
        {
            Radius = 5;
        }
        int BaseX = static_cast<int>(std::floor(Origin.X));
        int BaseY = static_cast<int>(std::floor(Origin.Y)) - 1;
        int BaseZ = static_cast<int>(std::floor(Origin.Z));
        for (int OffsetX = -Radius; OffsetX <= Radius; ++OffsetX)
        {
            for (int OffsetZ = -Radius; OffsetZ <= Radius; ++OffsetZ)
            {
                if (std::abs(OffsetX) != Radius && std::abs(OffsetZ) != Radius)
                {
                    continue;
                }
                float Target[3] = { static_cast<float>(BaseX + OffsetX), static_cast<float>(BaseY), static_cast<float>(BaseZ + OffsetZ) };
                GameMode::BuildBlock(GameModePtr, Target, 1);
            }
        }
    }

    void SelfTrap::SetEnabled(bool Enabled, int Mode)
    {
        gSelfTrapEnabled = Enabled;
        gSelfTrapMode = Mode;
    }

    void SelfTrap::ProcessTick(void* LocalPlayer)
    {
        if (!gSelfTrapEnabled || !LocalPlayer)
        {
            return;
        }
        Orange::Game::Vec3 Origin;
        if (!Orange::Game::RenderCore::ActorPosition(LocalPlayer, Origin))
        {
            return;
        }
        void* GameModePtr = Actor::GetGameMode(LocalPlayer);
        if (!GameModePtr)
        {
            return;
        }
        int BaseX = static_cast<int>(std::floor(Origin.X));
        int BaseY = static_cast<int>(std::floor(Origin.Y)) - 1;
        int BaseZ = static_cast<int>(std::floor(Origin.Z));
        for (int OffsetX = -1; OffsetX <= 1; ++OffsetX)
        {
            for (int OffsetZ = -1; OffsetZ <= 1; ++OffsetZ)
            {
                if (OffsetX == 0 && OffsetZ == 0)
                {
                    continue;
                }
                float Target[3] = { static_cast<float>(BaseX + OffsetX), static_cast<float>(BaseY), static_cast<float>(BaseZ + OffsetZ) };
                GameMode::BuildBlock(GameModePtr, Target, 1);
            }
        }
    }

    void Killis::SetEnabled(bool Enabled, int Mode)
    {
        gKillisEnabled = Enabled;
        gKillisMode = Mode;
    }

    void Killis::ProcessTick(void* LocalPlayer)
    {
        if (!gKillisEnabled || !LocalPlayer)
        {
            return;
        }
        GameMode::Attack(LocalPlayer, LocalPlayer);
    }

    void InfiniteAura::SetEnabled(int TargetMode, int Range, int Count, float Delay,
                                  int AttackTimes, int PullBack, bool Fixed, bool Swing, bool Riding)
    {
        gInfiniteAuraEnabled = Range > 0;
        gInfiniteAuraTargetMode = TargetMode;
        gInfiniteAuraRange = Range;
        gInfiniteAuraCount = Count;
        gInfiniteAuraDelay = Delay;
        gInfiniteAuraTimes = AttackTimes;
        gInfiniteAuraPullBack = PullBack;
        gInfiniteAuraFixed = Fixed;
        gInfiniteAuraSwing = Swing;
        gInfiniteAuraRiding = Riding;
    }

    void InfiniteAura::ProcessTick(void* LocalPlayer)
    {
        if (!gInfiniteAuraEnabled || !LocalPlayer)
        {
            return;
        }
        Orange::Game::Vec3 Origin;
        if (!Orange::Game::RenderCore::ActorPosition(LocalPlayer, Origin))
        {
            return;
        }
        std::vector<void*> Targets;
        Orange::Game::RenderCore::CollectTargets(Targets, gInfiniteAuraTargetMode, static_cast<float>(gInfiniteAuraRange));
        if (Targets.empty())
        {
            return;
        }
        int Limit = gInfiniteAuraTargetMode == 1 ? 1 : gInfiniteAuraCount;
        int Applied = 0;
        for (void* Target : Targets)
        {
            if (Applied >= Limit)
            {
                break;
            }
            for (int Index = 0; Index < gInfiniteAuraTimes; ++Index)
            {
                GameMode::Attack(LocalPlayer, Target);
            }
            if (gInfiniteAuraSwing)
            {
                Actor::Swing(LocalPlayer);
            }
            ++Applied;
        }
    }

    void AutoFollow::SetEnabled(float OffsetX, float OffsetY, float OffsetZ, int Range,
                                float Speed, bool Flag1, bool Flag2, bool Flag3)
    {
        (void)Flag1;
        (void)Flag2;
        (void)Flag3;
        gAutoFollowEnabled = Range > 0;
        gAutoFollowOffset[0] = OffsetX;
        gAutoFollowOffset[1] = OffsetY;
        gAutoFollowOffset[2] = OffsetZ;
        gAutoFollowRange = Range;
        gAutoFollowSpeed = Speed;
    }

    void AutoFollow::ProcessTick(void* LocalPlayer)
    {
        if (!gAutoFollowEnabled || !LocalPlayer)
        {
            return;
        }
        Orange::Game::Vec3 Origin;
        if (!Orange::Game::RenderCore::ActorPosition(LocalPlayer, Origin))
        {
            return;
        }
        std::vector<void*> Targets;
        Orange::Game::RenderCore::CollectTargets(Targets, 1, static_cast<float>(gAutoFollowRange));
        if (Targets.empty())
        {
            return;
        }
        Orange::Game::Vec3 Target;
        if (!Orange::Game::RenderCore::ActorPosition(Targets.front(), Target))
        {
            return;
        }
        Target.X += gAutoFollowOffset[0];
        Target.Y += gAutoFollowOffset[1];
        Target.Z += gAutoFollowOffset[2];
        float DeltaX = Target.X - Origin.X;
        float DeltaZ = Target.Z - Origin.Z;
        float Length = std::sqrt(DeltaX * DeltaX + DeltaZ * DeltaZ);
        if (Length <= 0.001f)
        {
            return;
        }
        Actor::LerpMotion(LocalPlayer, DeltaX / Length * gAutoFollowSpeed, 0.0f, DeltaZ / Length * gAutoFollowSpeed);
    }

    void AttackEffects::SetConfig(bool Enabled, int TargetMask, float Size, int Mode,
                                  float Duration, int Count, int Style)
    {
        gAttackEffectsEnabled = Enabled;
        gAttackEffectsMask = TargetMask::Normalize(TargetMask);
        gAttackEffectsSize = Size;
        gAttackEffectsMode = Mode;
        gAttackEffectsDuration = Duration;
        gAttackEffectsCount = Count;
        gAttackEffectsStyle = Style;
    }

    void AttackEffects::OnAttackEvent(void* Target, bool IsPlayer)
    {
        (void)IsPlayer;
        if (!gAttackEffectsEnabled || !Target)
        {
            return;
        }
        gAttackEffectsTarget = Target;
        gAttackEffectsTimer = gAttackEffectsDuration;
    }

    void AttackEffects::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gAttackEffectsEnabled)
        {
            return;
        }
        if (gAttackEffectsTimer > 0.0f)
        {
            gAttackEffectsTimer -= 0.05f;
        }
    }

    void AttackEffects::Draw()
    {
        if (!gAttackEffectsEnabled || !gAttackEffectsTarget || gAttackEffectsTimer <= 0.0f)
        {
            return;
        }
        Orange::Game::Vec3 Center;
        if (!Orange::Game::RenderCore::ActorCenter(gAttackEffectsTarget, Center))
        {
            return;
        }
        unsigned int Color = Orange::Game::RenderCore::ColorFromMode(gAttackEffectsMode, 0xFFFFFFFFu, 0.0f);
        Game::Glow::CircleFilled(Center.X, Center.Y, Center.Z, gAttackEffectsSize, Color);
    }
}
