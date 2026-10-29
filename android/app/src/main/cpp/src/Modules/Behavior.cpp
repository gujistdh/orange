#include "orange/Modules/Behavior.h"
#include "orange/Modules/State.h"
#include "orange/Game/Offsets.h"
#include "orange/Core/Main.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/ModelFace.h"
#include "orange/Modules/Combat/KillAura.h"

#include <chrono>

namespace Orange::Modules
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

        bool Enabled(int Index)
        {
            return GetFlag(Index);
        }

        enum : int
        {
            kAutoToolState = 0x10,
            kEntityRidingState = 0x20,
            kBigTopState = 0x30,
            kEnchantState = 0x40,
            kNightVisionState = 0x50,
            kInventoryState = 0x60,
            kViewCommandBlockState = 0x70,
            kItemSetState = 0x80,
            kModelFaceState = 0x90,
            kAutoAppleState = 0xA0,
            kQuickbuildState = 0xB0,
            kPlayerAbilityState = 0xC0,
            kNbtState = 0xD0,
            kParticleEditorState = 0xE0,
            kSoundEditorState = 0xF0,
            kEffectEditorState = 0x100,
        };
    }

    namespace AutoTool
    {
        namespace
        {
            void* gLastBlock = nullptr;
            std::chrono::steady_clock::time_point gLastSwap;
        }

        void ProcessTick(void* Actor)
        {
            if (!Enabled(kAutoToolState) || !Actor)
            {
                return;
            }
            void* MiningBlock = Game::HighlightInfo::GetMiningBlock();
            if (!MiningBlock)
            {
                gLastBlock = nullptr;
                return;
            }
            if (MiningBlock == gLastBlock)
            {
                return;
            }
            auto Now = std::chrono::steady_clock::now();
            auto Elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(Now - gLastSwap).count();
            if (Elapsed <= 299)
            {
                return;
            }
            gLastSwap = Now;
            gLastBlock = MiningBlock;

            void* Inventory = Game::Actor::GetSupplies(Actor);
            if (!Inventory)
            {
                return;
            }
            int Current = Game::Supplies::GetSelectedSlot(Inventory);
            if (Current < 0 || Current >= 9)
            {
                return;
            }
            float Best = Game::Block::GetDestroyProgress(Actor, MiningBlock);
            int BestSlot = Current;
            for (int Index = 0; Index <= 8; ++Index)
            {
                if (Index == Current)
                {
                    continue;
                }
                Game::Supplies::SelectSlot(Inventory, Index);
                float Progress = Game::Block::GetDestroyProgress(Actor, MiningBlock);
                if (Progress > Best + 0.0001f)
                {
                    Best = Progress;
                    BestSlot = Index;
                }
            }
            Game::Supplies::SelectSlot(Inventory, BestSlot);
        }
    }

    namespace EntityRiding
    {
        void ProcessTick(void* Actor)
        {
            if (!Enabled(kEntityRidingState) || !Actor)
            {
                return;
            }
            void* Vehicle = Game::Actor::GetVehicle(Actor);
            if (!Vehicle)
            {
                return;
            }
            Game::Actor::SetPersistent(Vehicle, true);
        }
    }

    namespace BigTop
    {
        namespace
        {
            float gAngle = 0.0f;
        }

        void ProcessTick(void* Actor)
        {
            (void)Actor;
            if (!Enabled(kBigTopState))
            {
                return;
            }
            int Speed = GetCounter(kBigTopState + 1);
            if (Speed < 0)
            {
                Speed = 0;
            }
            if (Speed > 100)
            {
                Speed = 100;
            }
            float Step = Speed > 0 ? 360.0f / static_cast<float>(Speed) : 0.0f;
            gAngle += Step;
            if (gAngle >= 360.0f)
            {
                gAngle -= 360.0f;
            }
            float Wobble = std::sin(gAngle * 0.017453f);
            Game::ModelFace::Request(1, gAngle, 90.0f * Wobble);
        }
    }

    namespace Enchant
    {
        void ProcessTick(void* Actor)
        {
            if (!Enabled(kEnchantState) || !Actor)
            {
                return;
            }
            Game::Render::InstallHooks();
            bool First = GetFlag(kEnchantState + 1);
            bool Second = GetFlag(kEnchantState + 2);
            if (!First && !Second)
            {
                return;
            }
            if (Second)
            {
                Game::Enchant::ApplyFromHand(Actor, GetFlag(kEnchantState + 3));
            }
            if (First)
            {
                Game::Enchant::ApplyFromSlot(Actor, GetFlag(kEnchantState + 3));
            }
        }
    }

    namespace GlobalNightVision
    {
        void ProcessTick(void* Actor)
        {
            if (!Enabled(kNightVisionState))
            {
                return;
            }
            int Ticks = AdvanceCounter(kNightVisionState + 1, 1);
            if (Ticks % 20 != 0)
            {
                return;
            }
            Game::GlobalNightVision::Apply(Actor);
        }
    }

    namespace Inventory
    {
        void ProcessTick(void* Actor)
        {
            (void)Actor;
            if (Enabled(kInventoryState))
            {
                return;
            }
            if (GetFlag(kInventoryState + 1))
            {
                SetFlag(kInventoryState + 1, false);
                SetCounter(kInventoryState + 2, 0);
            }
        }
    }

    namespace ViewCommandBlock
    {
        void ProcessTick(void* Actor)
        {
            if (!Enabled(kViewCommandBlockState) || !Actor)
            {
                return;
            }
            Game::Render::InstallHooks();
            Game::ViewCommandBlock::ScanNearby(Actor);
        }
    }

    namespace ItemSet
    {
        void ProcessTick(void* Actor)
        {
            if (!Enabled(kItemSetState) || !Actor)
            {
                return;
            }
            if (GetFlag(kItemSetState + 1))
            {
                SetText(kItemSetState + 3, Game::ItemSet::CaptureCurrent(Actor));
                SetMode(kItemSetState + 4, Game::ItemSet::CaptureCount(Actor));
            }
            if (GetFlag(kItemSetState + 2))
            {
                Game::ItemSet::ApplyStored(Actor, GetText(kItemSetState + 3));
                SetFlag(kItemSetState + 1, true);
            }
        }
    }

    namespace ModelFace
    {
        void ProcessTick()
        {
            SetFlag(kModelFaceState + 1, false);
            if (!Enabled(kModelFaceState))
            {
                Game::ModelFace::InstallHook();
            }
        }
    }

    namespace AutoApple
    {
        void ProcessTick(void* Actor)
        {
            if (!Enabled(kAutoAppleState) || !Actor)
            {
                return;
            }
            void* Carried = Game::Player::GetCarriedItem(Actor);
            if (!Carried)
            {
                return;
            }
            if (!Game::ItemStack::IsNull(Carried))
            {
                return;
            }
            int Slot = GetMode(kAutoAppleState + 1);
            Game::Player::StartUsingItem(Actor, Slot);
        }
    }

    namespace Quickbuild
    {
        void ProcessTick(void* Actor)
        {
            if (!Enabled(kQuickbuildState) || !Actor)
            {
                return;
            }
            Game::Quickbuild::StepBuild(Actor);
        }
    }

    namespace PlayerAbility
    {
        void ProcessTick(void* Actor)
        {
            if (!Actor || !Enabled(kPlayerAbilityState))
            {
                return;
            }
            void* Abilities = Game::Player::GetAbilities(Actor);
            if (!Abilities)
            {
                return;
            }
            for (int Index = 0; Index < 16; ++Index)
            {
                Game::Abilities::SetBool(Abilities, Index, GetFlag(kPlayerAbilityState + 1 + Index));
            }
        }
    }

    namespace Nbt
    {
        void ProcessTick(void* Actor)
        {
            if (!Enabled(kNbtState) || !Actor)
            {
                return;
            }
            int Result = Game::ServerSync::LastResult();
            if (Result == 2 || Result == 3 || Result == 4)
            {
                SetFlag(kNbtState + 2, false);
            }
            if (GetFlag(kNbtState + 1))
            {
                std::string Path = GetText(kNbtState + 3);
                if (!Path.empty())
                {
                    Game::Nbt::LoadFromPath(Actor, Path);
                }
            }
        }
    }

    namespace ParticleEditor
    {
        void ProcessTick(void* Actor)
        {
            if (!Actor)
            {
                return;
            }
            if (GetFlag(kParticleEditorState + 1))
            {
                int Selected = 0;
                for (int Index = 0; Index <= 179; ++Index)
                {
                    if (GetFlag(kParticleEditorState + 2 + Index))
                    {
                        ++Selected;
                    }
                }
                SetFlag(kParticleEditorState + 3, Selected == 0);
            }
            else if (GetFlag(kParticleEditorState + 3))
            {
                Game::ParticleEditor::Emit(Actor);
            }
        }
    }

    namespace SoundEditor
    {
        void ProcessTick(void* Actor)
        {
            if (!Enabled(kSoundEditorState) || !Actor)
            {
                return;
            }
            Game::Render::InstallHooks();
            float Volume = 1.0f;
            float Pitch = 1.0f;
            int Index = Game::SoundEditor::PickSound(Volume, Pitch);
            if (Index >= 0 && Index < 1590)
            {
                Game::SoundEditor::Play(Actor, Index, Volume, Pitch);
            }
        }
    }

    namespace EffectEditor
    {
        void ProcessTick(void* Actor)
        {
            if (!Enabled(kEffectEditorState) || !Actor)
            {
                return;
            }
            Game::Render::InstallHooks();
            if (GetFlag(kEffectEditorState + 1))
            {
                Game::EffectEditor::ApplyToLocalPlayer(Actor);
            }
        }
    }

    namespace
    {
}
}
