#include "orange/Modules/World/BlockBreak.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Packet.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/RenderCore.h"
#include "orange/Core/Main.h"

#include <cmath>

namespace Orange::Modules::World
{
    using namespace Orange::Game;

    namespace
    {
        bool gEnabled = false;
        int gBudget = 1;
        int gReach = 6;
        int gBudgetLeft = 0;
        int gPhase = 0;
        bool gSentThisTick = false;
        bool gInstalled = false;
    }

    void BlockBreak::Init()
    {
        gInstalled = false;
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        gInstalled = true;
        gBudgetLeft = 0;
        gPhase = 0;
    }

    bool BlockBreak::IsEnabled()
    {
        return gEnabled;
    }

    void BlockBreak::SetEnabled(bool Value)
    {
        gEnabled = Value;
        if (!Value)
        {
            gBudgetLeft = 0;
        }
    }

    int BlockBreak::GetBudget()
    {
        return gBudget;
    }

    void BlockBreak::SetBudget(int Value)
    {
        gBudget = Value > 0 ? Value : 1;
    }

    int BlockBreak::GetReach()
    {
        return gReach;
    }

    void BlockBreak::SetReach(int Value)
    {
        gReach = Value > 0 ? Value : 6;
    }

    int BlockBreak::BudgetLeft()
    {
        return gBudgetLeft;
    }

    bool BlockBreak::SentThisTick()
    {
        return gSentThisTick;
    }

    void BlockBreak::ResetBudget()
    {
        gBudgetLeft = gBudget;
        gSentThisTick = false;
    }

    int BlockBreak::NextPhase(int Count)
    {
        if (Count <= 0)
        {
            return 0;
        }
        int Current = gPhase;
        if (Current < 0 || Current >= Count)
        {
            Current = 0;
        }
        int Step = gBudget > 0 ? gBudget : 1;
        int Next = Current + Step;
        while (Next >= Count)
        {
            Next -= Count;
        }
        gPhase = Next;
        return Current;
    }

    const void* BlockBreak::PlayerPos(void* Actor)
    {
        if (!Actor)
        {
            return nullptr;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::Actor_PosPtr);
        if (!Offset)
        {
            return nullptr;
        }
        return *reinterpret_cast<void**>(reinterpret_cast<Byte*>(Actor) + Offset);
    }

    int BlockBreak::PickFace(float X, float Y, float Z, int BlockX, int BlockY, int BlockZ)
    {
        float DeltaX = X - (static_cast<float>(BlockX) + 0.5f);
        float DeltaY = Y - (static_cast<float>(BlockY) + 0.5f);
        float DeltaZ = Z - (static_cast<float>(BlockZ) + 0.5f);
        float AbsX = std::fabs(DeltaX);
        float AbsY = std::fabs(DeltaY);
        float AbsZ = std::fabs(DeltaZ);
        if (AbsY < AbsX || AbsY < AbsZ)
        {
            if (AbsX < AbsZ)
            {
                return DeltaZ < 0.0f ? 2 : 3;
            }
            return DeltaX < 0.0f ? 4 : 5;
        }
        return DeltaY >= 0.0f ? 1 : 0;
    }

    bool BlockBreak::SendAction(void* Level, int ActionType, int X, int Y, int Z, int Face)
    {
        (void)Level;
        (void)ActionType;
        (void)X;
        (void)Y;
        (void)Z;
        (void)Face;
        if (!Packet::IsNetworkReady())
        {
            return false;
        }
        return false;
    }

    bool BlockBreak::SyncBlock(void* Level, const void* Position, int Face)
    {
        if (!gEnabled)
        {
            return false;
        }
        if (!Packet::IsNetworkReady())
        {
            return false;
        }
        if (gBudgetLeft <= 0)
        {
            return false;
        }
        const int* Coordinates = static_cast<const int*>(Position);
        if (!Coordinates)
        {
            return false;
        }
        int Sent = 0;
        if (SendAction(Level, static_cast<int>(PlayerAction::StartDestroyBlock), Coordinates[0], Coordinates[1], Coordinates[2], Face))
        {
            ++Sent;
        }
        if (SendAction(Level, static_cast<int>(PlayerAction::PredictDestroyBlock), Coordinates[0], Coordinates[1], Coordinates[2], Face))
        {
            ++Sent;
        }
        if (SendAction(Level, static_cast<int>(PlayerAction::StopDestroyBlock), Coordinates[0], Coordinates[1], Coordinates[2], Face))
        {
            ++Sent;
        }
        if (Sent > 0)
        {
            --gBudgetLeft;
            gSentThisTick = true;
            return true;
        }
        return false;
    }

    bool BlockBreak::BreakBlock(void* Level, void* Actor, int X, int Y, int Z, int Face, bool CheckReach)
    {
        if (!Orange::Core::BaseAddress)
        {
            return false;
        }
        if (X < -30000000 || X >= 30000001)
        {
            return false;
        }
        if (Z < -30000000 || Z >= 30000001)
        {
            return false;
        }
        if (Y < -512 || Y >= 2049)
        {
            return false;
        }
        int TargetFace = Face;
        if (TargetFace < 0 || TargetFace >= 6)
        {
            TargetFace = 1;
        }
        if (CheckReach)
        {
            int ReachValue = gReach > 0 ? gReach : 6;
            float Limit = (static_cast<float>(ReachValue) + 0.5f) * (static_cast<float>(ReachValue) + 0.5f);
            const float* Position = static_cast<const float*>(PlayerPos(Actor));
            if (Position)
            {
                float DeltaX = Position[0] - (static_cast<float>(X) + 0.5f);
                float DeltaY = Position[1] - (static_cast<float>(Y) + 0.5f);
                float DeltaZ = Position[2] - (static_cast<float>(Z) + 0.5f);
                float Distance = DeltaX * DeltaX + DeltaY * DeltaY + DeltaZ * DeltaZ;
                if (Distance > Limit)
                {
                    return false;
                }
            }
        }
        BlockPos Position;
        Position.X = X;
        Position.Y = Y;
        Position.Z = Z;
        return SyncBlock(Level, &Position, TargetFace);
    }

    void BlockBreak::ProcessTick(void* LocalPlayer)
    {
        if (!gEnabled)
        {
            return;
        }
        if (gBudgetLeft > 0)
        {
            ResetBudget();
            return;
        }
        if (Packet::IsNetworkReady())
        {
            ResetBudget();
        }
        (void)LocalPlayer;
    }
}
