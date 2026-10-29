#include "orange/Modules/Player.h"
#include "orange/Modules/State.h"

namespace Orange::Modules::Player
{

void BindAutoApple(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AutoApple", Module->IsOn());
    SetModuleValue("AutoApple", 0, static_cast<float>(Module->GetNum("血量")));
}

void BindAutoTool(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AutoTool", Module->IsOn());
}

void BindAuxEffect(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AuxEffect", Module->IsOn());
    SetModuleValue("AuxEffect", 0, static_cast<float>(Module->GetModeIndex("药水")));
}

void BindBed(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Bed", Module->IsOn());
    SetModuleValue("Bed", 0, static_cast<float>(Module->GetModeIndex("床")));
}

void BindBigTop(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("BigTop", Module->IsOn());
    SetModuleValue("BigTop", 0, static_cast<float>(Module->GetNum("速度")));
}

void BindEffectEditor(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("EffectEditor", Module->IsOn());
}

void BindEnchantEditor(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Enchant", Module->IsOn());
}

void BindFlag(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Flag", Module->IsOn());
    SetModuleValue("Flag", 0, static_cast<float>(Module->GetModeIndex("旗帜")));
}

void BindGlobalNightVision(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("GlobalNightVision", Module->IsOn());
}

void BindGoatHorn(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("GoatHorn", Module->IsOn());
    SetModuleValue("GoatHorn", 0, static_cast<float>(Module->GetModeIndex("羊角")));
}

void BindInventory(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Inventory", Module->IsOn());
    SetModuleValue("Inventory", 0, static_cast<float>(Module->GetNum("动作间隔")));
}

void BindItemEditor(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("ItemSet", Module->IsOn());
}

void BindNbtEditor(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Nbt", Module->IsOn());
}

void BindOminousPotion(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("OminousPotion", Module->IsOn());
    SetModuleValue("OminousPotion", 0, static_cast<float>(Module->GetModeIndex("不祥药水")));
}

void BindParticleEditor(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("ParticleEditor", Module->IsOn());
}

void BindPlayerAbility(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("PlayerAbility", Module->IsOn());
    SetModuleValue("PlayerAbility", 0, Module->GetBool("建造方块") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 1, Module->GetBool("破坏方块") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 2, Module->GetBool("使用门和开关") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 3, Module->GetBool("打开容器") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 4, Module->GetBool("攻击玩家") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 5, Module->GetBool("攻击实体") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 6, Module->GetBool("操作员") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 7, Module->GetBool("使用指令") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 8, Module->GetBool("无敌") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 9, Module->GetBool("可飞行") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 10, Module->GetBool("飞行状态") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 11, Module->GetBool("穿墙") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 12, Module->GetBool("瞬间破坏") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 13, Module->GetBool("闪电等级") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 14, Module->GetBool("是否被禁言") ? 1.0f : 0.0f);
    SetModuleValue("PlayerAbility", 15, Module->GetBool("世界建设者") ? 1.0f : 0.0f);
}

void BindQuickbuild(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Quickbuild", Module->IsOn());
    SetModuleValue("Quickbuild", 0, static_cast<float>(Module->GetModeIndex("类型")));
    SetModuleValue("Quickbuild", 1, static_cast<float>(Module->GetNum("操作距离")));
    SetModuleValue("Quickbuild", 2, static_cast<float>(Module->GetNum("半径")));
    SetModuleValue("Quickbuild", 3, static_cast<float>(Module->GetNum("高度")));
    SetModuleValue("Quickbuild", 4, Module->GetBool("终点") ? 1.0f : 0.0f);
    SetModuleValue("Quickbuild", 5, Module->GetBool("描点") ? 1.0f : 0.0f);
}

void BindSetDurability(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Damage", Module->IsOn());
    SetModuleValue("Damage", 0, static_cast<float>(Module->GetNum("耐久")));
}

void BindSoundEditor(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("SoundEditor", Module->IsOn());
}

void BindTeleport(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Teleport", Module->IsOn());
    SetModuleValue("Teleport", 0, static_cast<float>(Module->GetModeIndex("方式")));
    SetModuleValue("Teleport", 1, Module->GetBool("坐骑") ? 1.0f : 0.0f);
    SetModuleValue("Teleport", 2, static_cast<float>(Module->GetNum("速度")));
}

void BindXrayCam(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("XrayCam", Module->IsOn());
}

}
