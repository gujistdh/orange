#include "orange/Modules/Combat.h"
#include "orange/Modules/State.h"

namespace Orange::Modules::Combat
{

void BindAttackCount(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AttackCount", Module->IsOn());
    SetModuleValue("AttackCount", 0, static_cast<float>(Module->GetNum("次数")));
    SetModuleValue("AttackCount", 1, static_cast<float>(Module->GetCheckMask("目标")));
    SetModuleValue("AttackCount", 2, Module->GetBool("挥手") ? 1.0f : 0.0f);
    SetModuleValue("AttackCount", 3, static_cast<float>(Module->GetNum("距离")));
}

void BindAttackDamage(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AttackDamage", Module->IsOn());
    SetModuleValue("AttackDamage", 0, static_cast<float>(Module->GetNum("数量")));
    SetModuleValue("AttackDamage", 1, static_cast<float>(Module->GetCheckMask("目标")));
}

void BindAttackEffects(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AttackEffects", Module->IsOn());
    SetModuleValue("AttackEffects", 0, static_cast<float>(Module->GetCheckMask("目标")));
    SetModuleValue("AttackEffects", 1, static_cast<float>(Module->GetNum("线条大小")));
    SetModuleValue("AttackEffects", 2, static_cast<float>(Module->GetNum("显示时常")));
    SetModuleValue("AttackEffects", 3, static_cast<float>(Module->GetModeIndex("颜色")));
}

void BindAttackMessage(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AttackMessage", Module->IsOn());
}

void BindAuraRing(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AuraRing", Module->IsOn());
    SetModuleValue("AuraRing", 0, static_cast<float>(Module->GetModeIndex("颜色")));
    SetModuleValue("AuraRing", 1, static_cast<float>(Module->GetNum("距离")));
}

void BindAutoBow(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AutoBow", Module->IsOn());
}

void BindAutoFollow(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AutoFollow", Module->IsOn());
    SetModuleValue("AutoFollow", 0, static_cast<float>(Module->GetNum("偏移X")));
    SetModuleValue("AutoFollow", 1, static_cast<float>(Module->GetNum("偏移Y")));
    SetModuleValue("AutoFollow", 2, static_cast<float>(Module->GetNum("偏移Z")));
    SetModuleValue("AutoFollow", 3, static_cast<float>(Module->GetNum("距离")));
    SetModuleValue("AutoFollow", 4, Module->GetBool("固定") ? 1.0f : 0.0f);
    SetModuleValue("AutoFollow", 5, Module->GetBool("坐骑") ? 1.0f : 0.0f);
}

void BindCircle(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Circle", Module->IsOn());
    SetModuleValue("Circle", 0, static_cast<float>(Module->GetNum("移动速度")));
    SetModuleValue("Circle", 1, static_cast<float>(Module->GetNum("距离")));
    SetModuleValue("Circle", 2, static_cast<float>(Module->GetNum("半径")));
    SetModuleValue("Circle", 3, Module->GetBool("固定") ? 1.0f : 0.0f);
    SetModuleValue("Circle", 4, Module->GetBool("坐骑") ? 1.0f : 0.0f);
}

void BindCounterattack(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Counterattack", Module->IsOn());
    SetModuleValue("Counterattack", 0, static_cast<float>(Module->GetNum("次数")));
    SetModuleValue("Counterattack", 1, static_cast<float>(Module->GetCheckMask("目标")));
    SetModuleValue("Counterattack", 2, Module->GetBool("挥手") ? 1.0f : 0.0f);
    SetModuleValue("Counterattack", 3, static_cast<float>(Module->GetNum("距离")));
}

void BindEntityRiding(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("EntityRiding", Module->IsOn());
    SetModuleValue("EntityRiding", 0, static_cast<float>(Module->GetCheckMask("目标")));
}

void BindHitbox(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Hitbox", Module->IsOn());
    SetModuleValue("Hitbox", 0, static_cast<float>(Module->GetCheckMask("目标")));
    SetModuleValue("Hitbox", 1, static_cast<float>(Module->GetNum("碰撞大小")));
}

void BindInfiniteAura(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("InfiniteAura", Module->IsOn());
    SetModuleValue("InfiniteAura", 0, static_cast<float>(Module->GetModeIndex("攻击目标")));
    SetModuleValue("InfiniteAura", 1, static_cast<float>(Module->GetNum("数量")));
    SetModuleValue("InfiniteAura", 2, static_cast<float>(Module->GetNum("攻击距离")));
    SetModuleValue("InfiniteAura", 3, static_cast<float>(Module->GetNum("攻击次数")));
    SetModuleValue("InfiniteAura", 4, static_cast<float>(Module->GetNum("攻击延迟")));
    SetModuleValue("InfiniteAura", 5, static_cast<float>(Module->GetNum("攻击拉回")));
    SetModuleValue("InfiniteAura", 6, Module->GetBool("固定") ? 1.0f : 0.0f);
    SetModuleValue("InfiniteAura", 7, Module->GetBool("挥手") ? 1.0f : 0.0f);
    SetModuleValue("InfiniteAura", 8, Module->GetBool("坐骑") ? 1.0f : 0.0f);
}

void BindKillAura(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("KillAura", Module->IsOn());
    SetModuleValue("KillAura", 0, static_cast<float>(Module->GetNum("CPS")));
    SetModuleValue("KillAura", 1, Module->GetBool("挥手") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 2, static_cast<float>(Module->GetModeIndex("攻击模式")));
    SetModuleValue("KillAura", 3, static_cast<float>(Module->GetCheckMask("攻击目标")));
    SetModuleValue("KillAura", 4, static_cast<float>(Module->GetNum("数量")));
    SetModuleValue("KillAura", 5, Module->GetBool("固定") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 6, static_cast<float>(Module->GetNum("预测")));
    SetModuleValue("KillAura", 7, static_cast<float>(Module->GetModeIndex("优先")));
    SetModuleValue("KillAura", 8, Module->GetBool("武器切换") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 9, Module->GetBool("隔墙攻击") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 10, static_cast<float>(Module->GetModeIndex("视角范围")));
    SetModuleValue("KillAura", 11, Module->GetBool("转头") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 12, static_cast<float>(Module->GetModeIndex("转头类型")));
    SetModuleValue("KillAura", 13, Module->GetBool("信息显示") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 14, Module->GetBool("范围光环") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 15, static_cast<float>(Module->GetModeIndex("光环颜色")));
    SetModuleValue("KillAura", 16, Module->GetBool("挥手") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 17, static_cast<float>(Module->GetModeIndex("攻击模式")));
    SetModuleValue("KillAura", 18, static_cast<float>(Module->GetCheckMask("攻击目标")));
    SetModuleValue("KillAura", 19, static_cast<float>(Module->GetNum("数量")));
    SetModuleValue("KillAura", 20, Module->GetBool("固定") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 21, static_cast<float>(Module->GetNum("预测")));
    SetModuleValue("KillAura", 22, static_cast<float>(Module->GetModeIndex("优先")));
    SetModuleValue("KillAura", 23, Module->GetBool("武器切换") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 24, Module->GetBool("隔墙攻击") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 25, static_cast<float>(Module->GetModeIndex("视角范围")));
    SetModuleValue("KillAura", 26, Module->GetBool("转头") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 27, static_cast<float>(Module->GetModeIndex("转头类型")));
    SetModuleValue("KillAura", 28, Module->GetBool("信息显示") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 29, Module->GetBool("范围光环") ? 1.0f : 0.0f);
    SetModuleValue("KillAura", 30, static_cast<float>(Module->GetModeIndex("光环颜色")));
}

void BindKillMessage(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("KillMessage", Module->IsOn());
}

void BindKillis(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Killis", Module->IsOn());
    SetModuleValue("Killis", 0, static_cast<float>(Module->GetNum("CPS")));
    SetModuleValue("Killis", 1, Module->GetBool("挥手") ? 1.0f : 0.0f);
}

void BindKnockback(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Knockback", Module->IsOn());
}

void BindSelfTrap(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("SelfTrap", Module->IsOn());
    SetModuleValue("SelfTrap", 0, static_cast<float>(Module->GetNum("高度")));
    SetModuleValue("SelfTrap", 1, Module->GetBool("挥手") ? 1.0f : 0.0f);
}

void BindSpacing(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Spacing", Module->IsOn());
    SetModuleValue("Spacing", 0, static_cast<float>(Module->GetNum("距离")));
    SetModuleValue("Spacing", 1, Module->GetBool("坐骑") ? 1.0f : 0.0f);
}

void BindTrap(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Trap", Module->IsOn());
    SetModuleValue("Trap", 0, static_cast<float>(Module->GetNum("范围")));
    SetModuleValue("Trap", 1, Module->GetBool("挥手") ? 1.0f : 0.0f);
    SetModuleValue("Trap", 2, static_cast<float>(Module->GetCheckMask("目标")));
}

}
