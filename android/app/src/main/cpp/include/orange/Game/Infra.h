#pragma once

#include "orange/Common.h"

namespace Orange::Game
{
    class EntityListStore
    {
    public:
        static void Init();
        static void OnActorInit(void* Actor);
        static void OnActorDestroy(void* Actor);
        static void GetEntities(std::vector<void*>& Out);
        static std::size_t Size();
        static void Clear();
    };

    class ScreenMetrics
    {
    public:
        static void Init();
        static float Width();
        static float Height();
        static void SetSize(float NewWidth, float NewHeight);
        static void* ClientInstance();
        static void* GuiDataInstance();
        static int HookStatus();
    };

    class ItemIconStore
    {
    public:
        static void Init();
        static void* Texture(int ItemId, int Variant);
        static void* Find(int ItemId);
        static int VariantCount(int ItemId);
        static void ReleaseAll();
        static int ResidentCount();
    };

    class ServerSyncState
    {
    public:
        static void Install();
        static void Uninstall();
        static int LastResult();
        static int LastSlot();
        static void Commit(int Slot, int Result);
        static int CommitTransaction(void* Source, void* Target, int Slot);
        static bool HasPending();
        static void ConsumePending();
        static uint64_t Timestamp();
    };

    class BlockClickTracker
    {
    public:
        static void Install();
        static void ProcessTick(void* LocalPlayer);
        static void Clear();
        static int GetClick(int Index);
        static int Serial();
        static bool Installed();
    };
}
