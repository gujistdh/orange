#pragma once

#include "orange/Common.h"
#include "orange/Game/Offsets.h"

namespace Orange::Game
{
    class Actor
    {
    public:
        static void* GetLocalPlayerInstance();
        static void* GetPrimaryLocalPlayer();
        static void SetLocalPlayer(void* Value);
        static void* GetVehicle(void* Self);
        static void* GetRideRoot(void* Self);
        static void* GetLevel(void* Self);
        static void* GetDimensionBlockSource(void* Self);
        static void* GetSupplies(void* Self);
        static void SetPersistent(void* Self, bool Value);
        static void LerpMotion(void* Self, float X, float Y, float Z);
        static void SetPos(void* Self, const void* Pos);
        static void Swing(void* Self);
        static bool IsOnGround(void* Self);
        static float GetHealth(void* Self);
        static void* GetGameMode(void* Self);
        static bool IsAlive(void* Self);
        static bool CanSee(void* Self);
        static void GetPosition(void* Self, float* Out);
        static void SetStatusFlag(void* Self, Orange::Offsets::Slot Flag, bool Value);
        static bool GetStatusFlag(void* Self, Orange::Offsets::Slot Flag);
    };

    class Player
    {
    public:
        static void* GetCarriedItem(void* Self);
        static void* GetAbilities(void* Self);
        static void StartUsingItem(void* Self, int Slot);
        static void StopUsingItem(void* Self);
        static void StartGliding(void* Self);
        static void StopGliding(void* Self);
        static void SetPlayerGameType(void* Self, int Value);
    };

    class Supplies
    {
    public:
        static int GetSelectedSlot(void* Self);
        static void SelectSlot(void* Self, int Slot);
        static void* GetItemStack(void* Self, int Slot);
    };

    class Block
    {
    public:
        static float GetDestroyProgress(void* Actor, void* Block);
        static bool IsAir(void* Self);
    };

    class ItemStack
    {
    public:
        static bool IsNull(void* Self);
        static int GetCount(void* Self);
        static int GetDamageValue(void* Self);
        static void SetDamageValue(void* Self, int Value);
        static int GetMaxStackSize(void* Self);
        static void* Copy(void* Self);
    };

    class Abilities
    {
    public:
        static void SetBool(void* Self, int Index, bool Value);
    };

    class HighlightInfo
    {
    public:
        static void* GetMiningBlock();
        static bool IsMiningActive();
    };

    class Render
    {
    public:
        static void InstallHooks();
        static void Flush();
        static unsigned int RainbowColor();
    };

    class Enchant
    {
    public:
        static void ApplyFromHand(void* Actor, bool Enabled);
        static void ApplyFromSlot(void* Actor, bool Enabled);
    };

    class GlobalNightVision
    {
    public:
        static void Apply(void* Actor);
    };

    class ViewCommandBlock
    {
    public:
        static void ScanNearby(void* Actor);
    };

    class ItemSet
    {
    public:
        static std::string CaptureCurrent(void* Actor);
        static int CaptureCount(void* Actor);
        static void ApplyStored(void* Actor, const std::string& Data);
    };

    class Quickbuild
    {
    public:
        static void StepBuild(void* Actor);
    };

    class ServerSync
    {
    public:
        static int LastResult();
    };

    class Nbt
    {
    public:
        static void LoadFromPath(void* Actor, const std::string& Path);
    };

    class ParticleEditor
    {
    public:
        static void Emit(void* Actor);
    };

    class SoundEditor
    {
    public:
        static int PickSound(float& Volume, float& Pitch);
        static void Play(void* Actor, int Index, float Volume, float Pitch);
    };

    class EffectEditor
    {
    public:
        static void ApplyToLocalPlayer(void* Actor);
    };

    class ContainerScreen
    {
    public:
        static bool MoveStack(void* Controller, int Slot);
        static bool HandleTakeAll(void* Controller);
        static bool HandlePlaceAll(void* Controller);
        static bool HandleDropItem(void* Controller);
        static bool HandleSwap(void* Controller, int From, int To);
        static void* GetStackFromModel(void* Controller, int Slot);
    };

    class CommandUtils
    {
    public:
        static void SpawnEntityAt(void* Level, const std::string& Name);
    };

    class Text
    {
    public:
        static void SendChat(const std::string& Message);
        static void SendCommand(const std::string& Command);
        static bool IsNetworkReady();
    };

    class GameMode
    {
    public:
        static void Attack(void* Self, void* Target);
        static void DestroyBlock(void* Self, void* Pos, int Face);
        static void BuildBlock(void* Self, void* Pos, int Face);
    };

    class EntityList
    {
    public:
        static void GetEntities(std::vector<void*>& Out);
    };

    class TargetMask
    {
    public:
        static bool IsDrop(void* Actor);
        static int Normalize(int Mask);
        static bool Allow(int Mask, bool IsPlayer, void* Actor);
        static bool WantMobs(int Mask);
        static float HealthOf(void* Actor);
    };

    class Glow
    {
    public:
        static void Rect(float X, float Y, float W, float H, unsigned int Color);
        static void RectFilled(float X, float Y, float W, float H, unsigned int Color);
        static void Line(float X1, float Y1, float X2, float Y2, unsigned int Color, float Thickness);
        static void Polyline(const void* Points, int Count, unsigned int Color, float Thickness);
        static void Circle(float X, float Y, float Radius, unsigned int Color, float Thickness);
        static void CircleFilled(float X, float Y, float Z, float Radius, unsigned int Color);
        static void QuadFilled(float X1, float Y1, float X2, float Y2, float X3, float Y3, float X4, float Y4, unsigned int Color);
        static unsigned int Clamp(unsigned int Color);
    };
}
