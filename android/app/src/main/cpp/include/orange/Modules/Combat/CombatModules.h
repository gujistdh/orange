#pragma once

#include "orange/Common.h"

namespace Orange::Modules::Combat
{
    class AttackCount
    {
    public:
        static void SetConfig(bool Enabled, int Count, int TargetMask, bool Flag, float Delay);
        static void ProcessTick(void* LocalPlayer);
        static void OnAttackEvent(void* Target, bool IsPlayer);
    };

    class AttackDamage
    {
    public:
        static void SetConfig(bool Enabled, int Amount, int TargetMask);
        static void ProcessTick(void* LocalPlayer);
        static void OnAttackEvent(void* Target, bool IsPlayer);
    };

    class Knockback
    {
    public:
        static void Init();
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
    };

    class KillMessage
    {
    public:
        static void SetEnabled(bool Enabled, const std::string& Text);
        static void ProcessTick(void* LocalPlayer);
        static void OnAttackEvent(void* Target);
    };

    class AttackMessage
    {
    public:
        static void SetEnabled(bool Enabled, const std::string& Text);
        static void ProcessTick(void* LocalPlayer);
        static void OnAttackEvent(void* Target, bool IsPlayer);
    };

    class AuraRing
    {
    public:
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class AutoBow
    {
    public:
        static void SetEnabled(bool Value);
        static bool IsEnabled();
        static void ProcessTick(void* LocalPlayer);
    };

    class Counterattack
    {
    public:
        static void SetConfig(bool Enabled, int Count, int TargetMask, bool Flag, float Delay);
        static void ProcessTick(void* LocalPlayer);
    };

    class Hitbox
    {
    public:
        static void SetConfig(bool Enabled, int TargetMask, float Scale);
        static void ProcessTick(void* LocalPlayer);
    };

    class Spacing
    {
    public:
        static void SetEnabled(bool Enabled, float Distance, bool Flag, bool Extra);
        static void ProcessTick(void* LocalPlayer);
    };

    class Circle
    {
    public:
        static void SetConfig(float Speed, float Range, float Radius, int TargetMask, bool Flag1, bool Flag2, bool Flag3);
        static void ProcessTick(void* LocalPlayer);
    };

    class Trap
    {
    public:
        static void SetEnabled(bool Enabled, float Range, bool Flag, int Mode);
        static void ProcessTick(void* LocalPlayer);
    };

    class SelfTrap
    {
    public:
        static void SetEnabled(bool Enabled, int Mode);
        static void ProcessTick(void* LocalPlayer);
    };

    class Killis
    {
    public:
        static void SetEnabled(bool Enabled, int Mode);
        static void ProcessTick(void* LocalPlayer);
    };

    class InfiniteAura
    {
    public:
        static void SetEnabled(int TargetMode, int Range, int Count, float Delay, int AttackTimes, int PullBack, bool Fixed, bool Swing, bool Riding);
        static void ProcessTick(void* LocalPlayer);
    };

    class AutoFollow
    {
    public:
        static void SetEnabled(float OffsetX, float OffsetY, float OffsetZ, int Range, float Speed, bool Flag1, bool Flag2, bool Flag3);
        static void ProcessTick(void* LocalPlayer);
    };

    class AttackEffects
    {
    public:
        static void SetConfig(bool Enabled, int TargetMask, float Size, int Mode, float Duration, int Count, int Style);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
        static void OnAttackEvent(void* Target, bool IsPlayer);
    };
}
