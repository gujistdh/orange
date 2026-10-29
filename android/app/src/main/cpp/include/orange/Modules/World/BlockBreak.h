#pragma once

#include "orange/Common.h"

namespace Orange::Modules::World
{
    class BlockBreak
    {
    public:
        static void Init();
        static bool SyncBlock(void* Level, const void* Position, int Face);
        static bool SendAction(void* Level, int ActionType, int X, int Y, int Z, int Face);
        static int NextPhase(int Count);
        static int BudgetLeft();
        static bool SentThisTick();
        static void ResetBudget();
        static void SetEnabled(bool Value);
        static bool IsEnabled();
        static int GetBudget();
        static void SetBudget(int Value);
        static int GetReach();
        static void SetReach(int Value);
        static void ProcessTick(void* LocalPlayer);
        static const void* PlayerPos(void* Actor);
        static int PickFace(float X, float Y, float Z, int BlockX, int BlockY, int BlockZ);
        static bool BreakBlock(void* Level, void* Actor, int X, int Y, int Z, int Face, bool CheckReach);
    };
}
