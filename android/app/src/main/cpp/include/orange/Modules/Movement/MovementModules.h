#pragma once

#include "orange/Common.h"

namespace Orange::Modules::Movement
{
    class Scaffold
    {
    public:
        static void SetEnabled(bool Value);
        static void SetAutoBlock(bool Value);
        static void SetLockY(bool Value);
        static void ProcessTick(void* LocalPlayer);
    };

    class JumpHack
    {
    public:
        static void Init();
        static void SetEnabled(bool Enabled, float Height);
        static void ProcessTick(void* LocalPlayer);
    };

    class ClickTP
    {
    public:
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
    };

    class AutoJump
    {
    public:
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
    };

    class AutoGlide
    {
    public:
        static bool IsEnabled();
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
    };

    class AutoSwim
    {
    public:
        static bool IsEnabled();
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
    };

    class WaterWalking
    {
    public:
        static bool IsEnabled();
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
    };

    class AutoForward
    {
    public:
        static bool IsEnabled();
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
    };

    class AutoSprint
    {
    public:
        static bool IsEnabled();
        static void SetConfig(float Speed, bool Value);
        static void ProcessTick(void* LocalPlayer);
    };

    class PlayerMomentum
    {
    public:
        static void SetConfig(float Speed, float Factor, bool Value);
        static void ProcessTick(void* LocalPlayer);
    };

    class VoidRebound
    {
    public:
        static void SetConfig(bool Enabled, int Distance, float Speed, bool Value);
        static void ProcessTick(void* LocalPlayer);
    };

    class MountFlight
    {
    public:
        static bool IsEnabled();
        static void SetConfig(float MoveSpeed, float UpDownSpeed, bool Value);
        static void ProcessTick(void* LocalPlayer);
        static void SetDownPressed(bool Value);
    };

    class RideRootFlight
    {
    public:
        static bool IsEnabled();
        static void SetConfig(float MoveSpeed, float UpDownSpeed, bool Value);
        static void ProcessTick(void* LocalPlayer);
        static void SetDownPressed(bool Value);
    };
}
