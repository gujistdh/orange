#include "orange/Modules/ModuleRegistry.h"
#include "orange/Modules/State.h"

#include "orange/Modules/Combat.h"
#include "orange/Modules/Movement.h"
#include "orange/Modules/Player.h"
#include "orange/Modules/Render.h"
#include "orange/Modules/World.h"
#include "orange/Modules/Script.h"
#include "orange/Modules/Misc.h"

namespace Orange::Modules
{
    namespace
    {
        struct BinderEntry
        {
            const char* Id;
            void (*Function)(Gui::Module*);
        };

        const BinderEntry kBinders[] = {
            { "attackCount", &Combat::BindAttackCount },
            { "attackDamage", &Combat::BindAttackDamage },
            { "attackEffects", &Combat::BindAttackEffects },
            { "attackMessage", &Combat::BindAttackMessage },
            { "auraRing", &Combat::BindAuraRing },
            { "autoBow", &Combat::BindAutoBow },
            { "autoFollow", &Combat::BindAutoFollow },
            { "circle", &Combat::BindCircle },
            { "counterattack", &Combat::BindCounterattack },
            { "entityRiding", &Combat::BindEntityRiding },
            { "hitbox", &Combat::BindHitbox },
            { "infiniteAura", &Combat::BindInfiniteAura },
            { "killAura", &Combat::BindKillAura },
            { "killMessage", &Combat::BindKillMessage },
            { "killis", &Combat::BindKillis },
            { "knockback", &Combat::BindKnockback },
            { "selfTrap", &Combat::BindSelfTrap },
            { "spacing", &Combat::BindSpacing },
            { "trap", &Combat::BindTrap },
            { "autoForward", &Movement::BindAutoForward },
            { "autoGlide", &Movement::BindAutoGlide },
            { "autoJump", &Movement::BindAutoJump },
            { "autoSprint", &Movement::BindAutoSprint },
            { "autoSwim", &Movement::BindAutoSwim },
            { "clickTP", &Movement::BindClickTP },
            { "jumpHack", &Movement::BindJumpHack },
            { "playerMomentum", &Movement::BindPlayerMomentum },
            { "scaffold", &Movement::BindScaffold },
            { "voidRebound", &Movement::BindVoidRebound },
            { "waterWalking", &Movement::BindWaterWalking },
            { "autoApple", &Player::BindAutoApple },
            { "autoTool", &Player::BindAutoTool },
            { "auxEffect", &Player::BindAuxEffect },
            { "bed", &Player::BindBed },
            { "bigTop", &Player::BindBigTop },
            { "effectEditor", &Player::BindEffectEditor },
            { "enchantEditor", &Player::BindEnchantEditor },
            { "flag", &Player::BindFlag },
            { "globalNightVision", &Player::BindGlobalNightVision },
            { "goatHorn", &Player::BindGoatHorn },
            { "inventory", &Player::BindInventory },
            { "itemEditor", &Player::BindItemEditor },
            { "nbtEditor", &Player::BindNbtEditor },
            { "ominousPotion", &Player::BindOminousPotion },
            { "particleEditor", &Player::BindParticleEditor },
            { "playerAbility", &Player::BindPlayerAbility },
            { "quickbuild", &Player::BindQuickbuild },
            { "setDurability", &Player::BindSetDurability },
            { "soundEditor", &Player::BindSoundEditor },
            { "teleport", &Player::BindTeleport },
            { "xrayCam", &Player::BindXrayCam },
            { "blockOutline", &Render::BindBlockOutline },
            { "blockRender", &Render::BindBlockRender },
            { "directionSign", &Render::BindDirectionSign },
            { "dropped3D", &Render::BindDropped3D },
            { "effectRender", &Render::BindEffectRender },
            { "entityTrace", &Render::BindEntityTrace },
            { "esp", &Render::BindEsp },
            { "featureList", &Render::BindFeatureList },
            { "floatingText", &Render::BindFloatingText },
            { "fog", &Render::BindFog },
            { "fov", &Render::BindFov },
            { "funcNotif", &Render::BindFuncNotif },
            { "handheldRotation", &Render::BindHandheldRotation },
            { "headHalo", &Render::BindHeadHalo },
            { "helditeminfo", &Render::BindHelditeminfo },
            { "highlightInfo", &Render::BindHighlightInfo },
            { "landingAura", &Render::BindLandingAura },
            { "placeRender", &Render::BindPlaceRender },
            { "radarWarning", &Render::BindRadarWarning },
            { "selfAura", &Render::BindSelfAura },
            { "setTime", &Render::BindSetTime },
            { "setWeather", &Render::BindSetWeather },
            { "showCoords", &Render::BindShowCoords },
            { "tagRender", &Render::BindTagRender },
            { "trail", &Render::BindTrail },
            { "viewCommandBlock", &Render::BindViewCommandBlock },
            { "watermark", &Render::BindWatermark },
            { "worldImprisonment", &Render::BindWorldImprisonment },
            { "worldRender", &Render::BindWorldRender },
            { "autoBreak", &World::BindAutoBreak },
            { "breakBlock", &World::BindBreakBlock },
            { "chestStealer", &World::BindChestStealer },
            { "clickBreak", &World::BindClickBreak },
            { "gameMode", &World::BindGameMode },
            { "gameSpeed", &World::BindGameSpeed },
            { "packetResending", &World::BindPacketResending },
            { "posText", &World::BindPosText },
            { "rideRootFlight", &World::BindRideRootFlight },
            { "stopPackets", &World::BindStopPackets },
            { "summonEntity", &World::BindSummonEntity },
            { "tpMod", &World::BindTpMod },
            { "buildingTools", &Script::BindBuildingTools },
            { "cardKeyTime", &Script::BindCardKeyTime },
            { "chatEffects", &Script::BindChatEffects },
            { "chatroom", &Script::BindChatroom },
            { "loadConfig", &Script::BindLoadConfig },
            { "message", &Script::BindMessage },
            { "pyLoader", &Script::BindPyLoader },
            { "runCommand", &Script::BindRunCommand },
            { "saveConfig", &Script::BindSaveConfig },
            { "viewUser", &Script::BindViewUser },
            { "watermarkText", &Script::BindWatermarkText },
            { "guiRender", &Misc::BindGuiRender },
            { "musicPlayer", &Misc::BindMusicPlayer },
            { "rocketModel", &Misc::BindRocketModel },
        };
    }

    namespace
    {
        std::map<std::string, Gui::Panel*> gCategories;
        std::map<std::string, Gui::Module*> gModules;
    }

    void RegisterCategory(const std::string& Name, Gui::Panel* Target)
    {
        gCategories[Name] = Target;
    }

    Gui::Module* Find(const std::string& Id)
    {
        auto It = gModules.find(Id);
        return It == gModules.end() ? nullptr : It->second;
    }

    std::size_t Count()
    {
        return gModules.size();
    }

    void RegisterAll()
    {
    }

    void ApplyBinder(const std::string& Id, Gui::Module* Module)
    {
        for (const BinderEntry& Entry : kBinders)
        {
            if (Id == Entry.Id)
            {
                Entry.Function(Module);
                return;
            }
        }
    }

    std::size_t BinderCount()
    {
        return sizeof(kBinders) / sizeof(kBinders[0]);
    }
}
