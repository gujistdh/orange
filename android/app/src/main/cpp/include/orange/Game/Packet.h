#pragma once

#include "orange/Common.h"

namespace Orange::Game
{
    enum class PlayerAction : int
    {
        Unknown = 0,
        StartDestroyBlock = 1,
        AbortDestroyBlock = 2,
        StopDestroyBlock = 3,
        PredictDestroyBlock = 18,
        ContinueDestroyBlock = 19,
        StartItemUseOn = 32,
        StopItemUseOn = 33,
        AttackBlock = 34,
        CreativeDestroyBlock = 35,
    };

    enum class BlockFace : int
    {
        Down = 0,
        Up = 1,
        North = 2,
        South = 3,
        West = 4,
        East = 5,
    };

    struct BlockPos
    {
        int X = 0;
        int Y = 0;
        int Z = 0;
    };

    class Packet
    {
    public:
        static bool IsNetworkReady();
        static void* LocalPlayer();
        static bool SendPlayerAction(PlayerAction Action, const BlockPos& Position, int Face);
        static bool SendCommandRequest(const std::string& Command);
        static bool SendText(const std::string& Text);
        static BlockFace PickFace(float X, float Y, float Z, const BlockPos& Position);
        static void* GetRuntimeId(void* Actor);
    };
}
