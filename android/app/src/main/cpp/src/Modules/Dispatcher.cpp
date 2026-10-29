#include "orange/Modules/Behavior.h"
#include "orange/Modules/State.h"
#include "orange/Modules/Combat/CombatModules.h"
#include "orange/Modules/Combat/KillAura.h"
#include "orange/Modules/Movement/MovementModules.h"
#include "orange/Modules/Render/RenderModules.h"
#include "orange/Modules/World/WorldModules.h"
#include "orange/Modules/World/TPMod.h"
#include "orange/Modules/World/BlockBreak.h"
#include "orange/Modules/Script/ScriptModules.h"
#include "orange/Modules/Editors/Editors.h"
#include "orange/Gui/Gui.h"
#include "orange/Gui/PanelRender.h"
#include "orange/Game/IconAnimation.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/Infra.h"
#include "orange/Game/RenderCore.h"

namespace Orange::Modules
{
    using namespace Orange::Game;

    namespace
    {
        struct TickEntry
        {
            void (*Function)(void*);
        };

        const TickEntry kCombatTicks[] = {
            { &Combat::AttackCount::ProcessTick },
            { &Combat::AttackDamage::ProcessTick },
            { &Combat::Knockback::ProcessTick },
            { &Combat::KillMessage::ProcessTick },
            { &Combat::AttackMessage::ProcessTick },
            { &Combat::AuraRing::ProcessTick },
            { &Combat::AutoBow::ProcessTick },
            { &Combat::Counterattack::ProcessTick },
            { &Combat::Hitbox::ProcessTick },
            { &Combat::Spacing::ProcessTick },
            { &Combat::Circle::ProcessTick },
            { &Combat::Trap::ProcessTick },
            { &Combat::SelfTrap::ProcessTick },
            { &Combat::Killis::ProcessTick },
            { &Combat::InfiniteAura::ProcessTick },
            { &Combat::AutoFollow::ProcessTick },
            { &Combat::AttackEffects::ProcessTick },
        };

        const TickEntry kMovementTicks[] = {
            { &Movement::Scaffold::ProcessTick },
            { &Movement::JumpHack::ProcessTick },
            { &Movement::ClickTP::ProcessTick },
            { &Movement::AutoJump::ProcessTick },
            { &Movement::AutoGlide::ProcessTick },
            { &Movement::AutoSwim::ProcessTick },
            { &Movement::WaterWalking::ProcessTick },
            { &Movement::AutoForward::ProcessTick },
            { &Movement::AutoSprint::ProcessTick },
            { &Movement::PlayerMomentum::ProcessTick },
            { &Movement::VoidRebound::ProcessTick },
            { &Movement::MountFlight::ProcessTick },
            { &Movement::RideRootFlight::ProcessTick },
        };

        const TickEntry kRenderTicks[] = {
            { &Render::Esp::ProcessTick },
            { &Render::EntityTrace::ProcessTick },
            { &Render::TagRender::ProcessTick },
            { &Render::HeadHalo::ProcessTick },
            { &Render::Trail::ProcessTick },
            { &Render::FloatingText::ProcessTick },
            { &Render::Fog::ProcessTick },
            { &Render::Fov::ProcessTick },
            { &Render::HandheldRotation::ProcessTick },
            { &Render::Helditeminfo::ProcessTick },
            { &Render::BlockOutline::ProcessTick },
            { &Render::BlockRender::ProcessTick },
            { &Render::PlaceRender::ProcessTick },
            { &Render::LandingAura::ProcessTick },
            { &Render::SelfAura::ProcessTick },
            { &Render::WorldImprisonment::ProcessTick },
            { &Render::WorldRender::ProcessTick },
            { &Render::EffectRender::ProcessTick },
            { &Render::ShowCoordinates::ProcessTick },
            { &Render::RadarWarning::ProcessTick },
            { &Render::HighlightInfo::ProcessTick },
            { &Render::DirectionSign::ProcessTick },
            { &Render::Dropped3D::ProcessTick },
            { &Render::SetTime::ProcessTick },
            { &Render::SetWeather::ProcessTick },
        };

        const TickEntry kWorldTicks[] = {
            { &World::Globalspeed::ProcessTick },
            { &World::SpawnEntity::ProcessTick },
            { &World::ChestStealer::ProcessTick },
            { &World::PacketResending::ProcessTick },
            { &World::StopPackets::ProcessTick },
            { &World::AutoBreak::ProcessTick },
            { &World::ClickBreak::ProcessTick },
            { &World::BreakBlock::ProcessTick },
            { &World::GameModeFeature::ProcessTick },
            { &World::Pos::ProcessTick },
            { &World::TPMod::ProcessTick },
            { &World::BlockBreak::ProcessTick },
        };

        const TickEntry kScriptTicks[] = {
            { &Script::Message::ProcessTick },
            { &Script::Text::ProcessTick },
            { &Script::RunCommand::ProcessTick },
            { &Script::Command::ProcessTick },
            { &Script::Chatroom::ProcessTick },
            { &Script::ChatEffects::ProcessTick },
            { &Script::BuildingTools::ProcessTick },
            { &Script::CardKeyTime::ProcessTick },
            { &Script::PythonBridge::ProcessTick },
        };

        const TickEntry kEditorTicks[] = {
            { &Editors::NbtEditor::ProcessTick },
            { &Editors::ItemSetEditor::ProcessTick },
            { &Editors::EnchantEditor::ProcessTick },
            { &Editors::ParticleEditor::ProcessTick },
            { &Editors::SoundEditor::ProcessTick },
            { &Editors::EffectEditor::ProcessTick },
            { &Editors::ViewUserEditor::ProcessTick },
            { &Editors::ViewCommandBlockEditor::ProcessTick },
        };

        void RunGroup(const TickEntry* Entries, std::size_t Count, void* LocalPlayer)
        {
            for (std::size_t Index = 0; Index < Count; ++Index)
            {
                if (Entries[Index].Function)
                {
                    Entries[Index].Function(LocalPlayer);
                }
            }
        }
    }

    void InstallAll()
    {
        Reset();
        EntityListStore::Init();
        ScreenMetrics::Init();
        ItemIconStore::Init();
        ServerSyncState::Install();
        BlockClickTracker::Install();
        RenderCore::Init();
        Combat::Knockback::Init();
        Movement::JumpHack::Init();
        World::Globalspeed::Init();
        World::ChestStealer::Init();
        World::BreakBlock::Init();
        Render::BlockRender::Init();
        Script::Command::Init();
        Script::BuildingTools::Init();
        Script::Chatroom::Init();
        Script::CardKeyTime::Init();
        Script::PythonBridge::Init();
        Script::Text::Init();
        Orange::Game::IconAnimation::Reset();
    }

    void TickAll()
    {
        Orange::Game::IconAnimation::Tick(0.05f);
        void* LocalPlayer = Actor::GetLocalPlayerInstance();
        ModelFace::ProcessTick();
        Combat::KillAura::ProcessTick(LocalPlayer);
        RunGroup(kCombatTicks, sizeof(kCombatTicks) / sizeof(kCombatTicks[0]), LocalPlayer);
        RunGroup(kMovementTicks, sizeof(kMovementTicks) / sizeof(kMovementTicks[0]), LocalPlayer);
        RunGroup(kRenderTicks, sizeof(kRenderTicks) / sizeof(kRenderTicks[0]), LocalPlayer);
        RunGroup(kWorldTicks, sizeof(kWorldTicks) / sizeof(kWorldTicks[0]), LocalPlayer);
        RunGroup(kScriptTicks, sizeof(kScriptTicks) / sizeof(kScriptTicks[0]), LocalPlayer);
        RunGroup(kEditorTicks, sizeof(kEditorTicks) / sizeof(kEditorTicks[0]), LocalPlayer);
    }

    void RenderAll()
    {
        Combat::AuraRing::Draw();
        Combat::AttackEffects::Draw();
        Render::Esp::Draw();
        Render::EntityTrace::Draw();
        Render::TagRender::Draw();
        Render::HeadHalo::Draw();
        Render::Trail::Draw();
        Render::FloatingText::Draw();
        Render::Helditeminfo::Draw();
        Render::BlockOutline::Draw();
        Render::BlockRender::Draw();
        Render::PlaceRender::Draw();
        Render::LandingAura::Draw();
        Render::SelfAura::Draw();
        Render::WorldImprisonment::Draw();
        Render::WorldRender::Draw();
        Render::EffectRender::Draw();
        Render::ShowCoordinates::Draw();
        Render::RadarWarning::Draw();
        Render::HighlightInfo::Draw();
        Render::DirectionSign::Draw();
        Render::Dropped3D::Draw();
        World::TPMod::Draw();
        Editors::NbtEditor::Draw();
        Editors::ItemSetEditor::Draw();
        Editors::EnchantEditor::Draw();
        Editors::ParticleEditor::Draw();
        Editors::SoundEditor::Draw();
        Editors::EffectEditor::Draw();
        Editors::ViewUserEditor::Draw();
        Editors::ViewCommandBlockEditor::Draw();
        Gui::DrawActionBar(ImGui::GetBackgroundDrawList());
        Gui::PanelRender::Draw(ImGui::GetBackgroundDrawList(), 1920.0f, 1080.0f);
    }
}
