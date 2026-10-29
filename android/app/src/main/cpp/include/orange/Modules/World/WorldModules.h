#pragma once

#include "orange/Common.h"

namespace Orange::Modules::World
{
    class Globalspeed
    {
    public:
        static void Init();
        static void SetEnabled(bool Enabled, int Speed);
        static int GetSpeed();
        static float GetScale();
        static void ProcessTick(void* LocalPlayer);
    };

    class SpawnEntity
    {
    public:
        static bool IsEnabled();
        static void SetEnabled(bool Value);
        static void SetEntity(const std::string& Name);
        static void SetCount(int Value);
        static void SetSpeed(int Value);
        static void SpawnHorseNow();
        static void ProcessTick(void* LocalPlayer);
    };

    class ChestStealer
    {
    public:
        static void Init();
        static bool IsEnabled();
        static void SetEnabled(bool Value);
        static void SetSpeed(int Value);
        static void ProcessTick(void* LocalPlayer);
        static void ProcessControllerTick(void* Controller);
    };

    class PacketResending
    {
    public:
        static bool IsEnabled();
        static void SetConfig(bool Enabled, int Count);
        static int GetRepeatCount();
        static void ProcessTick(void* LocalPlayer);
    };

    class StopPackets
    {
    public:
        static bool IsEnabled();
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
    };

    class AutoBreak
    {
    public:
        static bool IsEnabled();
        static void SetEnabled(bool Value, int Mode);
        static void ProcessTick(void* LocalPlayer);
    };

    class ClickBreak
    {
    public:
        static bool IsEnabled();
        static void SetEnabled(bool Value, int Mode);
        static void ProcessTick(void* LocalPlayer);
    };

    class BreakBlock
    {
    public:
        static void Init();
        static void SetConfig(bool Enabled, int Mode, int Face, int Count);
        static void ProcessTick(void* LocalPlayer);
        static void BreakBlockAt(void* LocalPlayer, int X, int Y, int Z);
        static int BudgetLeft();
        static void ResetBudget();
    };

    class GameModeFeature
    {
    public:
        static void SetEnabled(bool Enabled, int Mode);
        static void ProcessTick(void* LocalPlayer);
    };

    class Pos
    {
    public:
        static void SetConfig(bool Enabled, int Mode, bool Copy);
        static void ProcessTick(void* LocalPlayer);
    };
}
