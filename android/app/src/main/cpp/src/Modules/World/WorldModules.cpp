#include "orange/Modules/World/WorldModules.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/Infra.h"
#include "orange/Game/RenderCore.h"
#include "orange/Core/Main.h"

#include <cmath>

namespace Orange::Modules::World
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

        bool gGlobalspeedEnabled = false;
        int gGlobalspeedValue = 1;
        float gGlobalspeedScale = 1.0f;

        bool gSpawnEnabled = false;
        std::string gSpawnEntity;
        int gSpawnCount = 1;
        int gSpawnSpeed = 1;
        bool gSpawnPending = false;

        bool gChestStealerEnabled = false;
        int gChestStealerSpeed = 5;
        bool gChestStealerActive = false;
        bool gChestStealerInstalled = false;
        bool gChestStealerDone = false;
        int gChestStealerSlot = 0;
        void* gChestStealerLast = nullptr;

        bool gPacketResendingEnabled = false;
        int gPacketResendingCount = 1;

        bool gStopPacketsEnabled = false;

        bool gAutoBreakEnabled = false;
        int gAutoBreakMode = 0;

        bool gClickBreakEnabled = false;
        int gClickBreakMode = 0;

        bool gBreakBlockEnabled = false;
        int gBreakBlockMode = 0;
        int gBreakBlockFace = 0;
        int gBreakBlockCount = 1;
        int gBreakBlockBudget = 0;

        bool gGameModeEnabled = false;
        int gGameModeValue = 0;

        bool gPosEnabled = false;
        int gPosMode = 0;
        bool gPosCopy = false;
    }

    void Globalspeed::Init()
    {
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        gGlobalspeedScale = 1.0f;
    }

    void Globalspeed::SetEnabled(bool Enabled, int Speed)
    {
        gGlobalspeedEnabled = Enabled;
        gGlobalspeedValue = ClampInt(Speed, 0, 100);
    }

    int Globalspeed::GetSpeed()
    {
        return gGlobalspeedValue;
    }

    float Globalspeed::GetScale()
    {
        return gGlobalspeedScale;
    }

    void Globalspeed::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gGlobalspeedEnabled)
        {
            return;
        }
        gGlobalspeedScale = 1.0f + static_cast<float>(gGlobalspeedValue) * 0.01f;
    }

    bool SpawnEntity::IsEnabled()
    {
        return gSpawnEnabled;
    }

    void SpawnEntity::SetEnabled(bool Value)
    {
        if (gSpawnEnabled != Value)
        {
            gSpawnEnabled = Value;
            gSpawnPending = Value;
        }
    }

    void SpawnEntity::SetEntity(const std::string& Name)
    {
        gSpawnEntity = Name;
    }

    void SpawnEntity::SetCount(int Value)
    {
        gSpawnCount = Value <= 0 ? 1 : Value;
    }

    void SpawnEntity::SetSpeed(int Value)
    {
        gSpawnSpeed = Value < 0 ? 0 : Value;
    }

    void SpawnEntity::SpawnHorseNow()
    {
        void* Local = Actor::GetPrimaryLocalPlayer();
        if (!Local)
        {
            return;
        }
        for (int Index = 0; Index < gSpawnCount; ++Index)
        {
            Game::CommandUtils::SpawnEntityAt(Local, gSpawnEntity);
        }
    }

    void SpawnEntity::ProcessTick(void* LocalPlayer)
    {
        if (!LocalPlayer)
        {
            return;
        }
        if (!gSpawnEnabled || !gSpawnPending)
        {
            gSpawnPending = gSpawnEnabled;
            return;
        }
        SpawnHorseNow();
        gSpawnPending = false;
    }

    void ChestStealer::Init()
    {
        gChestStealerInstalled = false;
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        gChestStealerInstalled = true;
        gChestStealerSlot = 0;
        gChestStealerDone = false;
        gChestStealerLast = nullptr;
    }

    bool ChestStealer::IsEnabled()
    {
        return gChestStealerEnabled;
    }

    void ChestStealer::SetEnabled(bool Value)
    {
        if (gChestStealerEnabled != Value)
        {
            gChestStealerEnabled = Value;
            gChestStealerSlot = 0;
            gChestStealerDone = false;
            gChestStealerLast = nullptr;
        }
    }

    void ChestStealer::SetSpeed(int Value)
    {
        gChestStealerSpeed = ClampInt(Value, 0, 10);
    }

    void ChestStealer::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gChestStealerEnabled && gChestStealerActive)
        {
            gChestStealerActive = false;
            gChestStealerSlot = 0;
            gChestStealerDone = false;
            gChestStealerLast = nullptr;
        }
    }

    void ChestStealer::ProcessControllerTick(void* Controller)
    {
        if (!gChestStealerEnabled || !Controller)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        intptr_t Expected = static_cast<intptr_t>(Orange::Core::BaseAddress + Orange::Offsets::Load(Orange::Offsets::Slot::ChestScreenController_vtable));
        if (Expected <= 0)
        {
            return;
        }
        void* Vtable = *reinterpret_cast<void**>(Controller);
        if (Vtable != reinterpret_cast<void*>(Expected))
        {
            return;
        }
        if (gChestStealerLast != Controller)
        {
            gChestStealerSlot = 0;
            gChestStealerDone = false;
            gChestStealerLast = Controller;
        }
        gChestStealerActive = true;
        if (gChestStealerDone)
        {
            return;
        }
        int SlotCount = static_cast<int>(Orange::Offsets::Load(Orange::Offsets::Slot::ChestStealer_slotCount));
        if (SlotCount <= 0)
        {
            SlotCount = 27;
        }
        for (int Index = gChestStealerSpeed; Index >= 1; --Index)
        {
            if (gChestStealerSlot >= SlotCount)
            {
                break;
            }
            Game::ContainerScreen::MoveStack(Controller, gChestStealerSlot);
            ++gChestStealerSlot;
        }
        if (gChestStealerSlot >= SlotCount)
        {
            gChestStealerDone = true;
        }
    }

    bool PacketResending::IsEnabled()
    {
        return gPacketResendingEnabled;
    }

    void PacketResending::SetConfig(bool Enabled, int Count)
    {
        gPacketResendingEnabled = Enabled;
        gPacketResendingCount = Count;
    }

    int PacketResending::GetRepeatCount()
    {
        if (!gPacketResendingEnabled)
        {
            return 1;
        }
        return ClampInt(gPacketResendingCount, 1, 100);
    }

    void PacketResending::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gPacketResendingEnabled)
        {
            return;
        }
        if (gPacketResendingCount < 1)
        {
            gPacketResendingCount = 1;
        }
    }

    bool StopPackets::IsEnabled()
    {
        return gStopPacketsEnabled;
    }

    void StopPackets::SetEnabled(bool Value)
    {
        gStopPacketsEnabled = Value;
    }

    void StopPackets::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gStopPacketsEnabled)
        {
            return;
        }
    }

    bool AutoBreak::IsEnabled()
    {
        return gAutoBreakEnabled;
    }

    void AutoBreak::SetEnabled(bool Value, int Mode)
    {
        gAutoBreakEnabled = Value;
        gAutoBreakMode = Mode;
    }

    void AutoBreak::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gAutoBreakEnabled)
        {
            return;
        }
    }

    bool ClickBreak::IsEnabled()
    {
        return gClickBreakEnabled;
    }

    void ClickBreak::SetEnabled(bool Value, int Mode)
    {
        gClickBreakEnabled = Value;
        gClickBreakMode = Mode;
    }

    void ClickBreak::ProcessTick(void* LocalPlayer)
    {
        if (!gClickBreakEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        int Click = BlockClickTracker::GetClick(gClickBreakMode);
        if (Click == 0)
        {
            return;
        }
        Orange::Game::Vec3 Position;
        if (!Orange::Game::RenderCore::ActorPosition(LocalPlayer, Position))
        {
            return;
        }
        void* GameModePtr = Actor::GetGameMode(LocalPlayer);
        if (!GameModePtr)
        {
            return;
        }
        float Target[3] = { std::floor(Position.X), std::floor(Position.Y) - 1.0f, std::floor(Position.Z) };
        GameMode::DestroyBlock(GameModePtr, Target, 1);
    }

    void BreakBlock::Init()
    {
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
    }

    void BreakBlock::SetConfig(bool Enabled, int Mode, int Face, int Count)
    {
        gBreakBlockEnabled = Enabled;
        gBreakBlockMode = ClampInt(Mode, 5000, 10000);
        gBreakBlockFace = ClampInt(Face, 1, 12);
        gBreakBlockCount = ClampInt(Count, 1, 10000);
    }

    int BreakBlock::BudgetLeft()
    {
        return gBreakBlockBudget;
    }

    void BreakBlock::ResetBudget()
    {
        gBreakBlockBudget = 0;
    }

    void BreakBlock::BreakBlockAt(void* LocalPlayer, int X, int Y, int Z)
    {
        if (!LocalPlayer)
        {
            return;
        }
        void* GameModePtr = Actor::GetGameMode(LocalPlayer);
        if (!GameModePtr)
        {
            return;
        }
        float Position[3] = { static_cast<float>(X), static_cast<float>(Y), static_cast<float>(Z) };
        GameMode::DestroyBlock(GameModePtr, Position, gBreakBlockFace);
        ++gBreakBlockBudget;
    }

    void BreakBlock::ProcessTick(void* LocalPlayer)
    {
        if (!gBreakBlockEnabled || !LocalPlayer)
        {
            return;
        }
        Vec3 Position;
        if (!RenderCore::ActorPosition(LocalPlayer, Position))
        {
            return;
        }
        int BaseX = static_cast<int>(std::floor(Position.X));
        int BaseY = static_cast<int>(std::floor(Position.Y)) - 1;
        int BaseZ = static_cast<int>(std::floor(Position.Z));
        BreakBlockAt(LocalPlayer, BaseX, BaseY, BaseZ);
    }

    void GameModeFeature::SetEnabled(bool Enabled, int Mode)
    {
        gGameModeEnabled = Enabled;
        gGameModeValue = Mode;
    }

    void GameModeFeature::ProcessTick(void* LocalPlayer)
    {
        if (!gGameModeEnabled || !LocalPlayer)
        {
            return;
        }
        Player::SetPlayerGameType(LocalPlayer, gGameModeValue);
    }

    void Pos::SetConfig(bool Enabled, int Mode, bool Copy)
    {
        gPosEnabled = Enabled;
        gPosMode = Mode;
        gPosCopy = Copy;
    }

    void Pos::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gPosEnabled)
        {
            return;
        }
    }
}
