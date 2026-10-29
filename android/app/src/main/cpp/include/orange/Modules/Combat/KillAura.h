#pragma once

#include "orange/Common.h"

namespace Orange::Modules::Combat
{
    class KillAura
    {
    public:
        static void SetEnabled(float Cps, float Range, float TargetCount, int AttackMode,
                               bool Swing, bool Fixed, int Predict, int Priority,
                               int WallAttack, bool TurnHead, float TurnSpeed, int TurnMode);
        static void SetAutoWeapon(bool Value);
        static void SetWallAttack(bool Value);
        static void SetViewMode(int Value);
        static void SetTurnHead(bool Value);
        static void SetTurnHeadMode(int Value);
        static void SetInfoDisplay(bool Value);
        static void SetRangeAura(bool Value);
        static void SetRangeAuraColor(int Mode, unsigned int Color);

        static void ProcessTick(void* LocalPlayer);
        static void OnAttack(void* LocalPlayer, void* Target);
        static bool IsEnabled();
        static void Reset();

    private:
        static bool CollectTargets();
        static void AttackSingle(void* LocalPlayer);
        static void AttackMulti(void* LocalPlayer);
        static void AttackRotate(void* LocalPlayer);
        static bool IsValidTarget(void* Actor);
        static float DistanceTo(void* Actor);
        static void RenderRangeAura();
        static void TurnToTarget(void* LocalPlayer, void* Target);
    };
}
