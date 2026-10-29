#include "orange/Game/Sdk.h"
#include "orange/Game/Infra.h"
#include "orange/Game/Packet.h"
#include "orange/Gui/Glow.h"
#include "orange/Game/RenderCore.h"
#include "orange/Game/Offsets.h"
#include "orange/Core/Main.h"

namespace Orange::Game
{
    namespace
    {
        template <typename Fn>
        Fn Resolve(Orange::Offsets::Slot Index)
        {
            intptr_t Offset = Orange::Offsets::Load(Index);
            if (!Offset || !Orange::Core::BaseAddress)
            {
                return nullptr;
            }
            return reinterpret_cast<Fn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        }

        template <typename Fn, typename... Args>
        auto Invoke(Orange::Offsets::Slot Index, Args... Values)
            -> decltype(static_cast<Fn>(nullptr)(Values...))
        {
            Fn Target = Resolve<Fn>(Index);
            if (!Target)
            {
                return decltype(static_cast<Fn>(nullptr)(Values...))();
            }
            return Target(Values...);
        }
    }

    namespace
    {
        void* gLocalPlayer = nullptr;
    }

    void* Actor::GetLocalPlayerInstance()
    {
        return gLocalPlayer;
    }

    void* Actor::GetPrimaryLocalPlayer()
    {
        return gLocalPlayer;
    }

    void Actor::SetLocalPlayer(void* Value)
    {
        gLocalPlayer = Value;
    }

    void* Actor::GetVehicle(void* Self)
    {
        return Invoke<void* (*)(void*)>(Orange::Offsets::Slot::Actor_getVehicle, Self);
    }

    void* Actor::GetRideRoot(void* Self)
    {
        return Invoke<void* (*)(void*)>(Orange::Offsets::Slot::Actor_getRideRoot, Self);
    }

    void* Actor::GetLevel(void* Self)
    {
        return Invoke<void* (*)(void*)>(Orange::Offsets::Slot::Actor_getLevel, Self);
    }

    void* Actor::GetDimensionBlockSource(void* Self)
    {
        return Invoke<void* (*)(void*)>(Orange::Offsets::Slot::Actor_getDimensionBlockSource, Self);
    }

    void* Actor::GetSupplies(void* Self)
    {
        return Invoke<void* (*)(void*)>(Orange::Offsets::Slot::Actor_getSupplies, Self);
    }

    void Actor::SetPersistent(void* Self, bool Value)
    {
        Invoke<void (*)(void*, bool)>(Orange::Offsets::Slot::Actor_setPersistent, Self, Value);
    }

    void Actor::LerpMotion(void* Self, float X, float Y, float Z)
    {
        Invoke<void (*)(void*, float, float, float)>(Orange::Offsets::Slot::Actor_lerpMotion, Self, X, Y, Z);
    }

    void Actor::SetPos(void* Self, const void* Pos)
    {
        Invoke<void (*)(void*, const void*)>(Orange::Offsets::Slot::Actor_setPos, Self, Pos);
    }

    void Actor::Swing(void* Self)
    {
        Invoke<void (*)(void*)>(Orange::Offsets::Slot::LocalPlayer_swing, Self);
    }

    bool Actor::IsOnGround(void* Self)
    {
        return Invoke<bool (*)(void*)>(Orange::Offsets::Slot::Actor_isOnGround, Self);
    }

    float Actor::GetHealth(void* Self)
    {
        return Invoke<float (*)(void*)>(Orange::Offsets::Slot::Actor_getHealth, Self);
    }

    void* Player::GetCarriedItem(void* Self)
    {
        return Invoke<void* (*)(void*)>(Orange::Offsets::Slot::Player_getCarriedItem, Self);
    }

    void* Player::GetAbilities(void* Self)
    {
        return Invoke<void* (*)(void*)>(Orange::Offsets::Slot::Player_GameMode, Self);
    }

    void Player::StartUsingItem(void* Self, int Slot)
    {
        Invoke<void (*)(void*, int)>(Orange::Offsets::Slot::Player_startUsingItem, Self, Slot);
    }

    void Player::StopUsingItem(void* Self)
    {
        Invoke<void (*)(void*)>(Orange::Offsets::Slot::Player_stopUsingItem, Self);
    }

    void Player::StartGliding(void* Self)
    {
        Invoke<void (*)(void*)>(Orange::Offsets::Slot::Player_startGliding, Self);
    }

    void Player::StopGliding(void* Self)
    {
        Invoke<void (*)(void*)>(Orange::Offsets::Slot::Player_stopGliding, Self);
    }

    void Player::SetPlayerGameType(void* Self, int Value)
    {
        Invoke<void (*)(void*, int)>(Orange::Offsets::Slot::Player_setPlayerGameType, Self, Value);
    }

    int Supplies::GetSelectedSlot(void* Self)
    {
        return Invoke<int (*)(void*)>(Orange::Offsets::Slot::Supplies_SelectedSlot, Self);
    }

    void Supplies::SelectSlot(void* Self, int Slot)
    {
        Invoke<void (*)(void*, int)>(Orange::Offsets::Slot::Supplies_selectSlot, Self, Slot);
    }

    void* Supplies::GetItemStack(void* Self, int Slot)
    {
        return Invoke<void* (*)(void*, int)>(Orange::Offsets::Slot::Supplies_getItemStack, Self, Slot);
    }

    float Block::GetDestroyProgress(void* ActorValue, void* BlockValue)
    {
        return Invoke<float (*)(void*, void*)>(Orange::Offsets::Slot::Block_getDestroyProgress, ActorValue, BlockValue);
    }

    bool Block::IsAir(void* Self)
    {
        return Invoke<bool (*)(void*)>(Orange::Offsets::Slot::Block_isAir, Self);
    }

    bool ItemStack::IsNull(void* Self)
    {
        return Invoke<bool (*)(void*)>(Orange::Offsets::Slot::ItemStack_isNull, Self);
    }

    int ItemStack::GetCount(void* Self)
    {
        return Invoke<int (*)(void*)>(Orange::Offsets::Slot::ItemStack_getCount, Self);
    }

    int ItemStack::GetDamageValue(void* Self)
    {
        return Invoke<int (*)(void*)>(Orange::Offsets::Slot::ItemStackBase_getDamageValue, Self);
    }

    void ItemStack::SetDamageValue(void* Self, int Value)
    {
        Invoke<void (*)(void*, int)>(Orange::Offsets::Slot::ItemStackBase_setDamageValue, Self, Value);
    }

    int ItemStack::GetMaxStackSize(void* Self)
    {
        return Invoke<int (*)(void*)>(Orange::Offsets::Slot::ItemStack_getMaxStackSize, Self);
    }

    void* ItemStack::Copy(void* Self)
    {
        return Invoke<void* (*)(void*)>(Orange::Offsets::Slot::ItemStack_copy, Self);
    }

    void Abilities::SetBool(void* Self, int Index, bool Value)
    {
        Invoke<void (*)(void*, int, bool)>(Orange::Offsets::Slot::Abilities_setBool, Self, Index, Value);
    }

    void* HighlightInfo::GetMiningBlock()
    {
        return Invoke<void* (*)()>(Orange::Offsets::Slot::GameMode_DestroyProgressField);
    }

    bool HighlightInfo::IsMiningActive()
    {
        return Invoke<void* (*)()>(Orange::Offsets::Slot::GameMode_DestroyProgressField) != nullptr;
    }

    void Render::InstallHooks()
    {
    }

    void Render::Flush()
    {
    }

    void Enchant::ApplyFromHand(void* ActorValue, bool EnabledValue)
    {
        if (!ActorValue || !EnabledValue)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::EnchantUtils_applyEnchant);
        if (!Offset)
        {
            return;
        }
        using ApplyFn = void (*)(void*);
        ApplyFn Apply = reinterpret_cast<ApplyFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        if (Apply)
        {
            Apply(ActorValue);
        }
    }

    void Enchant::ApplyFromSlot(void* ActorValue, bool EnabledValue)
    {
        if (!ActorValue || !EnabledValue)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::EnchantValidate);
        if (!Offset)
        {
            return;
        }
        using ValidateFn = void (*)(void*);
        ValidateFn Apply = reinterpret_cast<ValidateFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        if (Apply)
        {
            Apply(ActorValue);
        }
    }

    void GlobalNightVision::Apply(void* ActorValue)
    {
        if (!ActorValue)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::Actor_getEffect);
        if (!Offset)
        {
            return;
        }
        using EffectFn = void* (*)(void*, int);
        EffectFn Query = reinterpret_cast<EffectFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        if (Query)
        {
            Query(ActorValue, 4);
        }
    }

    void ViewCommandBlock::ScanNearby(void* ActorValue)
    {
        if (!ActorValue)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::BlockSource_getBlockEntity);
        if (!Offset)
        {
            return;
        }
        using ScanFn = void* (*)(void*);
        ScanFn Scan = reinterpret_cast<ScanFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        if (Scan)
        {
            Scan(ActorValue);
        }
    }

    std::string ItemSet::CaptureCurrent(void* ActorValue)
    {
        if (!ActorValue)
        {
            return std::string();
        }
        void* Carried = Game::Player::GetCarriedItem(ActorValue);
        if (!Carried)
        {
            return std::string();
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::Item_getRawNameId);
        if (!Offset)
        {
            return std::string();
        }
        using NameFn = const char* (*)(void*);
        NameFn Query = reinterpret_cast<NameFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        void* Item = *reinterpret_cast<void**>(reinterpret_cast<Byte*>(Carried) + Orange::Offsets::Load(Orange::Offsets::Slot::ItemStack_getItem));
        if (!Item || !Query)
        {
            return std::string();
        }
        const char* Name = Query(Item);
        return Name ? std::string(Name) : std::string();
    }

    int ItemSet::CaptureCount(void* ActorValue)
    {
        if (!ActorValue)
        {
            return 0;
        }
        void* Carried = Game::Player::GetCarriedItem(ActorValue);
        if (!Carried)
        {
            return 0;
        }
        return Game::ItemStack::GetCount(Carried);
    }

    void ItemSet::ApplyStored(void* ActorValue, const std::string& Data)
    {
        if (!ActorValue || Data.empty())
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::PlayerInventory_setItem);
        if (!Offset)
        {
            return;
        }
        using ApplyFn = void (*)(void*, int, void*);
        ApplyFn Apply = reinterpret_cast<ApplyFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        if (Apply)
        {
            Apply(ActorValue, 0, nullptr);
        }
    }

    void Quickbuild::StepBuild(void* ActorValue)
    {
        if (!ActorValue)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::GameMode_buildBlock);
        if (!Offset)
        {
            return;
        }
        using BuildFn = void (*)(void*, void*, int);
        BuildFn Build = reinterpret_cast<BuildFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        if (Build)
        {
            Build(ActorValue, nullptr, 1);
        }
    }

    int ServerSync::LastResult()
    {
        return Orange::Game::ServerSyncState::LastResult();
    }

    void Nbt::LoadFromPath(void* ActorValue, const std::string& Path)
    {
        if (!ActorValue || Path.empty())
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::ItemStackBase_load);
        if (!Offset)
        {
            return;
        }
        using LoadFn = bool (*)(void*, const void*);
        LoadFn Load = reinterpret_cast<LoadFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        if (Load)
        {
            Load(ActorValue, Path.c_str());
        }
    }

    void ParticleEditor::Emit(void* ActorValue)
    {
        if (!ActorValue)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::ParticleSystemEngine_createParticleEmitter);
        if (!Offset)
        {
            return;
        }
        using EmitFn = void* (*)(void*, const char*);
        EmitFn Emit = reinterpret_cast<EmitFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        if (Emit)
        {
            Emit(ActorValue, "minecraft:basic_flame_particle");
        }
    }

    int SoundEditor::PickSound(float& Volume, float& Pitch)
    {
        (void)Volume;
        (void)Pitch;
        return -1;
    }

    void SoundEditor::Play(void* ActorValue, int Index, float Volume, float Pitch)
    {
        if (!ActorValue || Index < 0)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::SoundPlayerPlaySlot);
        if (!Offset)
        {
            return;
        }
        using PlayFn = void (*)(void*, int, float, float);
        PlayFn Play = reinterpret_cast<PlayFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        if (Play)
        {
            Play(ActorValue, Index, Volume, Pitch);
        }
    }

    void EffectEditor::ApplyToLocalPlayer(void* ActorValue)
    {
        if (!ActorValue)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::Actor_addEffect);
        if (!Offset)
        {
            return;
        }
        using AddFn = void (*)(void*, void*);
        AddFn Add = reinterpret_cast<AddFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        if (Add)
        {
            Add(ActorValue, nullptr);
        }
    }

    bool Actor::IsAlive(void* Self)
    {
        return Invoke<bool (*)(void*)>(Orange::Offsets::Slot::KillAura_isAlive, Self);
    }

    bool Actor::CanSee(void* Self)
    {
        return Invoke<bool (*)(void*)>(Orange::Offsets::Slot::KillAura_isAlive, Self);
    }

    void Actor::GetPosition(void* Self, float* Out)
    {
        if (!Self || !Out)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::Actor_PosPtr);
        if (!Offset)
        {
            return;
        }
        void* PosPtr = *reinterpret_cast<void**>(reinterpret_cast<Byte*>(Self) + Offset);
        if (!PosPtr)
        {
            return;
        }
        float* Source = static_cast<float*>(PosPtr);
        Out[0] = Source[0];
        Out[1] = Source[1];
        Out[2] = Source[2];
    }

    void GameMode::Attack(void* Self, void* Target)
    {
        Invoke<void (*)(void*, void*)>(Orange::Offsets::Slot::KillAura_attack, Self, Target);
    }

    void GameMode::DestroyBlock(void* Self, void* Pos, int Face)
    {
        if (!Self)
        {
            return;
        }
        Game::BlockPos Target;
        const int* Raw = static_cast<const int*>(Pos);
        if (Raw)
        {
            Target.X = Raw[0];
            Target.Y = Raw[1];
            Target.Z = Raw[2];
        }
        Game::Packet::SendPlayerAction(Game::PlayerAction::StartDestroyBlock, Target, Face);
    }

    void GameMode::BuildBlock(void* Self, void* Pos, int Face)
    {
        if (!Self)
        {
            return;
        }
        Game::BlockPos Target;
        const int* Raw = static_cast<const int*>(Pos);
        if (Raw)
        {
            Target.X = Raw[0];
            Target.Y = Raw[1];
            Target.Z = Raw[2];
        }
        Game::Packet::SendPlayerAction(Game::PlayerAction::StartItemUseOn, Target, Face);
    }

    bool TargetMask::IsDrop(void* ActorValue)
    {
        if (!ActorValue)
        {
            return false;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::Mob_Del);
        if (!Offset)
        {
            return false;
        }
        void* Vtable = *reinterpret_cast<void**>(ActorValue);
        if (!Vtable)
        {
            return false;
        }
        intptr_t Target = Orange::Core::BaseAddress + static_cast<Address>(Offset);
        return reinterpret_cast<intptr_t>(Vtable) == Target;
    }

    int TargetMask::Normalize(int Mask)
    {
        return Mask & 7;
    }

    bool TargetMask::Allow(int Mask, bool IsPlayer, void* ActorValue)
    {
        (void)ActorValue;
        if (IsPlayer)
        {
            return (Mask & 1) != 0;
        }
        return (Mask & 2) != 0;
    }

    bool TargetMask::WantMobs(int Mask)
    {
        return (Mask & 2) != 0;
    }

    float TargetMask::HealthOf(void* ActorValue)
    {
        return Actor::GetHealth(ActorValue);
    }

    void Glow::Rect(float X, float Y, float W, float H, unsigned int Color)
    {
        Orange::Gui::Glow::Rect(ImGui::GetBackgroundDrawList(), X, Y, W, H, Color, 0.0f);
    }

    void Glow::RectFilled(float X, float Y, float W, float H, unsigned int Color)
    {
        Orange::Gui::Glow::RectFilled(ImGui::GetBackgroundDrawList(), X, Y, W, H, Color, 0.0f);
    }

    void Glow::Line(float X1, float Y1, float X2, float Y2, unsigned int Color, float Thickness)
    {
        Orange::Gui::Glow::Line(ImGui::GetBackgroundDrawList(), X1, Y1, X2, Y2, Color, Thickness, 0.0f);
    }

    void Glow::Polyline(const void* Points, int Count, unsigned int Color, float Thickness)
    {
        Orange::Gui::Glow::Polyline(ImGui::GetBackgroundDrawList(), static_cast<const float*>(Points), Count, Color, Thickness, 0.0f);
    }

    void Glow::Circle(float X, float Y, float Radius, unsigned int Color, float Thickness)
    {
        Orange::Gui::Glow::Circle(ImGui::GetBackgroundDrawList(), X, Y, Radius, Color, Thickness, 0.0f);
    }

    void Glow::CircleFilled(float X, float Y, float Z, float Radius, unsigned int Color)
    {
        float ScreenX = 0.0f;
        float ScreenY = 0.0f;
        float Depth = 0.0f;
        Orange::Game::Vec3 World{ X, Y, Z };
        if (!Orange::Game::RenderCore::WorldToScreen(World, ScreenX, ScreenY, 1920.0f, 1080.0f))
        {
            return;
        }
        float PixelRadius = Radius * 40.0f;
        Orange::Gui::Glow::CircleFilled(ImGui::GetBackgroundDrawList(), ScreenX, ScreenY, PixelRadius, Color, 1.5f);
        (void)Depth;
    }

    void Glow::QuadFilled(float X1, float Y1, float X2, float Y2, float X3, float Y3, float X4, float Y4, unsigned int Color)
    {
        Orange::Gui::Glow::QuadFilled(ImGui::GetBackgroundDrawList(), X1, Y1, X2, Y2, X3, Y3, X4, Y4, Color, 0.0f);
    }

    unsigned int Glow::Clamp(unsigned int Color)
    {
        return Color;
    }

    unsigned int Render::RainbowColor()
    {
        return 0xFFFFFFFFu;
    }
    void Text::SendChat(const std::string& Message)
    {
        if (Message.empty())
        {
            return;
        }
        Game::Packet::SendText(Message);
    }

    void Text::SendCommand(const std::string& CommandValue)
    {
        if (CommandValue.empty())
        {
            return;
        }
        Game::Packet::SendCommandRequest(CommandValue);
    }

    bool Text::IsNetworkReady()
    {
        if (!Orange::Core::BaseAddress)
        {
            return false;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::SendToServer);
        if (!Offset)
        {
            return false;
        }
        void* Local = Game::Actor::GetLocalPlayerInstance();
        if (!Local)
        {
            return false;
        }
        intptr_t Slot = Orange::Offsets::Load(Orange::Offsets::Slot::LocalPlayer_sendPacketVtableOffset);
        if (!Slot)
        {
            return false;
        }
        return *reinterpret_cast<void**>(reinterpret_cast<Byte*>(Local) + Slot) != nullptr;
    }
    void Actor::SetStatusFlag(void* Self, Orange::Offsets::Slot Flag, bool Value)
    {
        if (!Self)
        {
            return;
        }
        intptr_t Base = Orange::Offsets::Load(Flag);
        if (!Base)
        {
            return;
        }
        using SetFlagFn = void (*)(void*, bool);
        SetFlagFn Apply = reinterpret_cast<SetFlagFn>(Orange::Core::BaseAddress + static_cast<Address>(Base));
        if (Apply)
        {
            Apply(Self, Value);
        }
    }

    bool Actor::GetStatusFlag(void* Self, Orange::Offsets::Slot Flag)
    {
        if (!Self)
        {
            return false;
        }
        intptr_t Base = Orange::Offsets::Load(Flag);
        if (!Base)
        {
            return false;
        }
        using GetFlagFn = bool (*)(void*);
        GetFlagFn Query = reinterpret_cast<GetFlagFn>(Orange::Core::BaseAddress + static_cast<Address>(Base));
        return Query ? Query(Self) : false;
    }
    void CommandUtils::SpawnEntityAt(void* Level, const std::string& Name)
    {
        if (Name.empty())
        {
            return;
        }
        std::string Command = "/summon ";
        Command += Name;
        Game::Packet::SendCommandRequest(Command);
    }
    bool ContainerScreen::MoveStack(void* Controller, int Slot)
    {
        if (!Controller || Slot < 0)
        {
            return false;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::ContainerScreenController_handleCoalesceToIndex);
        if (!Offset)
        {
            return false;
        }
        using CallFn = bool (*)(void*, int);
        CallFn Call = reinterpret_cast<CallFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        return Call ? Call(Controller, Slot) : false;
    }

    bool ContainerScreen::HandleTakeAll(void* Controller)
    {
        if (!Controller)
        {
            return false;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::ContainerScreenController_handleTakeAll);
        if (!Offset)
        {
            return false;
        }
        using CallFn = bool (*)(void*);
        CallFn Call = reinterpret_cast<CallFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        return Call ? Call(Controller) : false;
    }

    bool ContainerScreen::HandlePlaceAll(void* Controller)
    {
        if (!Controller)
        {
            return false;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::ContainerScreenController_handlePlaceAll);
        if (!Offset)
        {
            return false;
        }
        using CallFn = bool (*)(void*);
        CallFn Call = reinterpret_cast<CallFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        return Call ? Call(Controller) : false;
    }

    bool ContainerScreen::HandleDropItem(void* Controller)
    {
        if (!Controller)
        {
            return false;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::ContainerScreenController_handleDropItem);
        if (!Offset)
        {
            return false;
        }
        using CallFn = bool (*)(void*);
        CallFn Call = reinterpret_cast<CallFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        return Call ? Call(Controller) : false;
    }

    bool ContainerScreen::HandleSwap(void* Controller, int From, int To)
    {
        if (!Controller || From < 0 || To < 0)
        {
            return false;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::ContainerScreenController_handleSwap);
        if (!Offset)
        {
            return false;
        }
        using CallFn = bool (*)(void*, int, int);
        CallFn Call = reinterpret_cast<CallFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        return Call ? Call(Controller, From, To) : false;
    }

    void* ContainerScreen::GetStackFromModel(void* Controller, int Slot)
    {
        if (!Controller || Slot < 0)
        {
            return nullptr;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::ContainerScreenController_getStackFromModel);
        if (!Offset)
        {
            return nullptr;
        }
        using CallFn = void* (*)(void*, int);
        CallFn Call = reinterpret_cast<CallFn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        return Call ? Call(Controller, Slot) : nullptr;
    }
    void* Actor::GetGameMode(void* Self)
    {
        if (!Self)
        {
            return nullptr;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::Player_GameMode);
        if (!Offset)
        {
            return nullptr;
        }
        return *reinterpret_cast<void**>(reinterpret_cast<Byte*>(Self) + Offset);
    }
}
