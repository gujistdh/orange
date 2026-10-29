#include "orange/Game/Infra.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/RenderCore.h"
#include "orange/Core/Main.h"
#include "orange/Core/ArmMemHook.h"
#include "orange/Game/ItemIconPak.h"

#include <GLES3/gl3.h>

#include <mutex>
#include <map>

namespace Orange::Game
{
    namespace
    {
        std::mutex gEntityMutex;
        std::vector<void*> gEntities;

        float gScreenWidth = 1920.0f;
        float gScreenHeight = 1080.0f;
        void* gClientInstance = nullptr;
        void* gGuiData = nullptr;
        int gHookStatus = 0;

        int gBlockClickSerial = 0;
        bool gBlockClickInstalled = false;
        int gBlockClickValues[8] = { 0 };
    }

    void EntityListStore::Init()
    {
        std::lock_guard<std::mutex> Lock(gEntityMutex);
        gEntities.clear();
    }

    void EntityListStore::OnActorInit(void* Actor)
    {
        if (!Actor)
        {
            return;
        }
        std::lock_guard<std::mutex> Lock(gEntityMutex);
        for (void* Existing : gEntities)
        {
            if (Existing == Actor)
            {
                return;
            }
        }
        gEntities.push_back(Actor);
    }

    void EntityListStore::OnActorDestroy(void* Actor)
    {
        if (!Actor)
        {
            return;
        }
        std::lock_guard<std::mutex> Lock(gEntityMutex);
        gEntities.erase(std::remove(gEntities.begin(), gEntities.end(), Actor), gEntities.end());
    }

    void EntityListStore::GetEntities(std::vector<void*>& Out)
    {
        std::lock_guard<std::mutex> Lock(gEntityMutex);
        Out = gEntities;
    }

    std::size_t EntityListStore::Size()
    {
        std::lock_guard<std::mutex> Lock(gEntityMutex);
        return gEntities.size();
    }

    void EntityListStore::Clear()
    {
        std::lock_guard<std::mutex> Lock(gEntityMutex);
        gEntities.clear();
    }

    void ScreenMetrics::Init()
    {
        gHookStatus = 0;
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        gHookStatus = 1;
    }

    float ScreenMetrics::Width()
    {
        return gScreenWidth;
    }

    float ScreenMetrics::Height()
    {
        return gScreenHeight;
    }

    void ScreenMetrics::SetSize(float NewWidth, float NewHeight)
    {
        if (NewWidth > 0.0f && NewHeight > 0.0f)
        {
            gScreenWidth = NewWidth;
            gScreenHeight = NewHeight;
        }
    }

    void* ScreenMetrics::ClientInstance()
    {
        return gClientInstance;
    }

    void* ScreenMetrics::GuiDataInstance()
    {
        return gGuiData;
    }

    int ScreenMetrics::HookStatus()
    {
        return gHookStatus;
    }

    namespace
    {
        struct IconEntry
        {
            unsigned int Texture = 0;
            uint64_t LastUse = 0;
            double Cooldown = 0.0;
        };

        std::mutex gIconMutexReal;
        std::unordered_map<int, IconEntry> gIconCache;
        uint64_t gIconClock = 0;
        int gIconResident = 0;
        bool gIconReady = false;

        constexpr int kIconMaxResident = 256;
        constexpr double kIconCooldownSeconds = 5.0;

        unsigned int CreateIconTexture(int ItemId)
        {
            GLuint Texture = 0;
            glGenTextures(1, &Texture);
            if (!Texture)
            {
                return 0;
            }
            const int kIconSize = 32;
            unsigned int Pixels[kIconSize * kIconSize];
            unsigned int Base = static_cast<unsigned int>((ItemId * 2654435761u) >> 8) | 0xFF000000u;
            for (int Index = 0; Index < kIconSize * kIconSize; ++Index)
            {
                int X = Index % kIconSize;
                int Y = Index / kIconSize;
                unsigned int Shade = ((X ^ Y) & 0x18u) << 2;
                Pixels[Index] = Base ^ (Shade << 16);
            }
            glBindTexture(GL_TEXTURE_2D, Texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, kIconSize, kIconSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, Pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindTexture(GL_TEXTURE_2D, 0);
            return Texture;
        }

        void DestroyIconTexture(unsigned int Texture)
        {
            if (!Texture)
            {
                return;
            }
            GLuint Handle = Texture;
            glDeleteTextures(1, &Handle);
        }

        void EvictOldest()
        {
            if (gIconCache.empty())
            {
                return;
            }
            auto Oldest = gIconCache.begin();
            for (auto It = gIconCache.begin(); It != gIconCache.end(); ++It)
            {
                if (It->second.LastUse < Oldest->second.LastUse)
                {
                    Oldest = It;
                }
            }
            DestroyIconTexture(Oldest->second.Texture);
            gIconCache.erase(Oldest);
            if (gIconResident > 0)
            {
                --gIconResident;
            }
        }
    }

    void ItemIconStore::Init()
    {
        ItemIconPak::Load("/storage/emulated/0/Android/data/com.netease.x19/files/Orange/textures/itemicons.pak");
        std::lock_guard<std::mutex> Lock(gIconMutexReal);
        gIconCache.clear();
        gIconClock = 0;
        gIconResident = 0;
        gIconReady = true;
    }

    void* ItemIconStore::Texture(int ItemId, int Variant)
    {
        (void)Variant;
        if (!gIconReady)
        {
            return nullptr;
        }
        std::lock_guard<std::mutex> Lock(gIconMutexReal);
        auto It = gIconCache.find(ItemId);
        if (It != gIconCache.end() && It->second.Texture != 0)
        {
            It->second.LastUse = ++gIconClock;
            return reinterpret_cast<void*>(static_cast<uintptr_t>(It->second.Texture));
        }
        if (It != gIconCache.end())
        {
            double Now = static_cast<double>(gIconClock) * 0.05;
            if (Now < It->second.Cooldown)
            {
                return nullptr;
            }
            It->second.Cooldown = Now + kIconCooldownSeconds;
        }
        if (gIconResident >= kIconMaxResident)
        {
            EvictOldest();
        }
        unsigned int Created = 0;
        if (ItemIconPak::IsLoaded() && ItemId >= 0 && ItemId < ItemIconPak::Count())
        {
            Created = ItemIconPak::EnsureTexture(ItemId);
        }
        if (!Created)
        {
            Created = CreateIconTexture(ItemId);
        }
        if (!Created)
        {
            return nullptr;
        }
        IconEntry Entry;
        Entry.Texture = Created;
        Entry.LastUse = ++gIconClock;
        Entry.Cooldown = 0.0;
        gIconCache[ItemId] = Entry;
        ++gIconResident;
        return reinterpret_cast<void*>(static_cast<uintptr_t>(Created));
    }

    void* ItemIconStore::Find(int ItemId)
    {
        std::lock_guard<std::mutex> Lock(gIconMutexReal);
        auto It = gIconCache.find(ItemId);
        if (It == gIconCache.end() || It->second.Texture == 0)
        {
            return nullptr;
        }
        return reinterpret_cast<void*>(static_cast<uintptr_t>(It->second.Texture));
    }

    int ItemIconStore::VariantCount(int ItemId)
    {
        std::lock_guard<std::mutex> Lock(gIconMutexReal);
        auto It = gIconCache.find(ItemId);
        return It == gIconCache.end() ? 0 : 1;
    }

    void ItemIconStore::ReleaseAll()
    {
        std::lock_guard<std::mutex> Lock(gIconMutexReal);
        for (auto& Entry : gIconCache)
        {
            DestroyIconTexture(Entry.second.Texture);
        }
        gIconCache.clear();
        gIconResident = 0;
    }

    int ItemIconStore::ResidentCount()
    {
        std::lock_guard<std::mutex> Lock(gIconMutexReal);
        return gIconResident;
    }

    namespace
    {
        std::mutex gSyncMutex;
        bool gSyncInstalled = false;
        bool gSyncPending = false;
        int gSyncResult = 0;
        int gSyncSlot = 0;
        void* gSyncHookInventory = nullptr;
        void* gSyncHookPacket = nullptr;
        void* gSyncOriginalInventory = nullptr;
        void* gSyncOriginalPacket = nullptr;
        void* gSyncPendingTransaction = nullptr;
        uint64_t gSyncTimestamp = 0;
        uint64_t gSyncClock = 0;

        constexpr int kSyncResultPending = 1;
        constexpr int kSyncResultFailed = 3;
    }

    void ServerSyncState::Install()
    {
        std::lock_guard<std::mutex> Lock(gSyncMutex);
        if (gSyncInstalled)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        intptr_t InventoryOffset = Orange::Offsets::Load(Orange::Offsets::Slot::ServerNetworkHandler_handleInventoryTransaction);
        if (InventoryOffset)
        {
            void* Target = reinterpret_cast<void*>(Orange::Core::BaseAddress + static_cast<Address>(InventoryOffset));
            if (Orange::Core::ArmMemHook::Hook(Target, nullptr, &gSyncOriginalInventory))
            {
                gSyncHookInventory = Target;
            }
        }
        intptr_t PacketOffset = Orange::Offsets::Load(Orange::Offsets::Slot::ServerPlayer_sendNetworkPacket);
        if (PacketOffset)
        {
            void* Target = reinterpret_cast<void*>(Orange::Core::BaseAddress + static_cast<Address>(PacketOffset));
            if (Orange::Core::ArmMemHook::Hook(Target, nullptr, &gSyncOriginalPacket))
            {
                gSyncHookPacket = Target;
            }
        }
        gSyncInstalled = true;
    }

    void ServerSyncState::Uninstall()
    {
        std::lock_guard<std::mutex> Lock(gSyncMutex);
        if (!gSyncInstalled)
        {
            return;
        }
        if (gSyncHookInventory)
        {
            Orange::Core::ArmMemHook::Unhook(gSyncHookInventory);
        }
        if (gSyncHookPacket)
        {
            Orange::Core::ArmMemHook::Unhook(gSyncHookPacket);
        }
        gSyncHookInventory = nullptr;
        gSyncHookPacket = nullptr;
        gSyncOriginalInventory = nullptr;
        gSyncOriginalPacket = nullptr;
        gSyncPendingTransaction = nullptr;
        gSyncInstalled = false;
        gSyncPending = false;
    }

    int ServerSyncState::LastResult()
    {
        std::lock_guard<std::mutex> Lock(gSyncMutex);
        return gSyncResult;
    }

    int ServerSyncState::LastSlot()
    {
        std::lock_guard<std::mutex> Lock(gSyncMutex);
        return gSyncSlot;
    }

    void ServerSyncState::Commit(int Slot, int Result)
    {
        std::lock_guard<std::mutex> Lock(gSyncMutex);
        gSyncSlot = Slot;
        gSyncResult = Result;
        gSyncPending = true;
    }

    int ServerSyncState::CommitTransaction(void* Source, void* Target, int Slot)
    {
        if (!Source || !Target)
        {
            std::lock_guard<std::mutex> Lock(gSyncMutex);
            gSyncSlot = Slot;
            gSyncResult = kSyncResultFailed;
            return kSyncResultFailed;
        }
        Install();
        std::lock_guard<std::mutex> Lock(gSyncMutex);
        gSyncPendingTransaction = Target;
        gSyncTimestamp = ++gSyncClock;
        gSyncSlot = Slot;
        gSyncResult = kSyncResultPending;
        gSyncPending = true;
        return kSyncResultPending;
    }

    bool ServerSyncState::HasPending()
    {
        std::lock_guard<std::mutex> Lock(gSyncMutex);
        return gSyncPending && gSyncPendingTransaction != nullptr;
    }

    void ServerSyncState::ConsumePending()
    {
        std::lock_guard<std::mutex> Lock(gSyncMutex);
        gSyncPendingTransaction = nullptr;
        gSyncPending = false;
    }

    uint64_t ServerSyncState::Timestamp()
    {
        std::lock_guard<std::mutex> Lock(gSyncMutex);
        return gSyncTimestamp;
    }

    void BlockClickTracker::Install()
    {
        gBlockClickSerial = 0;
        gBlockClickInstalled = true;
    }

    void BlockClickTracker::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        for (int Index = 0; Index < 8; ++Index)
        {
            if (gBlockClickValues[Index] > 0)
            {
                --gBlockClickValues[Index];
            }
        }
    }

    void BlockClickTracker::Clear()
    {
        for (int Index = 0; Index < 8; ++Index)
        {
            gBlockClickValues[Index] = 0;
        }
    }

    int BlockClickTracker::GetClick(int Index)
    {
        if (Index < 0 || Index >= 8)
        {
            return 0;
        }
        return gBlockClickValues[Index];
    }

    bool BlockClickTracker::Installed()
    {
        return gBlockClickInstalled;
    }

    int BlockClickTracker::Serial()
    {
        return ++gBlockClickSerial;
    }
    void EntityList::GetEntities(std::vector<void*>& Out)
    {
        EntityListStore::GetEntities(Out);
    }
}
