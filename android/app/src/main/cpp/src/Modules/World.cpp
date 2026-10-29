#include "orange/Modules/World.h"
#include "orange/Modules/State.h"

namespace Orange::Modules::World
{

void BindAutoBreak(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AutoBreak", Module->IsOn());
    SetModuleValue("AutoBreak", 0, static_cast<float>(Module->GetNum("破坏范围")));
}

void BindBreakBlock(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("BreakBlock", Module->IsOn());
    SetModuleValue("BreakBlock", 0, static_cast<float>(Module->GetNum("速度")));
    SetModuleValue("BreakBlock", 1, static_cast<float>(Module->GetNum("范围")));
    SetModuleValue("BreakBlock", 2, static_cast<float>(Module->GetNum("数量")));
}

void BindChestStealer(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("ChestStealer", Module->IsOn());
    SetModuleValue("ChestStealer", 0, static_cast<float>(Module->GetNum("速度")));
}

void BindClickBreak(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("ClickBreak", Module->IsOn());
    SetModuleValue("ClickBreak", 0, static_cast<float>(Module->GetNum("破坏范围")));
}

void BindGameMode(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("GameModeFeature", Module->IsOn());
    SetModuleValue("GameModeFeature", 0, static_cast<float>(Module->GetModeIndex("游戏模式")));
}

void BindGameSpeed(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Globalspeed", Module->IsOn());
    SetModuleValue("Globalspeed", 0, static_cast<float>(Module->GetNum("速度")));
}

void BindPacketResending(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("PacketResending", Module->IsOn());
    SetModuleValue("PacketResending", 0, static_cast<float>(Module->GetNum("数量")));
}

void BindPosText(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Pos", Module->IsOn());
    SetModuleValue("Pos", 0, static_cast<float>(Module->GetModeIndex("方式")));
    SetModuleValue("Pos", 1, static_cast<float>(Module->GetCheckMask("目标")));
    SetModuleValue("Pos", 2, static_cast<float>(Module->GetNum("范围")));
}

void BindRideRootFlight(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("RideRootFlight", Module->IsOn());
    SetModuleValue("RideRootFlight", 0, static_cast<float>(Module->GetNum("移动速度")));
    SetModuleValue("RideRootFlight", 1, static_cast<float>(Module->GetNum("上下速度")));
}

void BindStopPackets(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("StopPackets", Module->IsOn());
}

void BindSummonEntity(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("SpawnEntity", Module->IsOn());
    SetModuleValue("SpawnEntity", 0, static_cast<float>(Module->GetNum("数量")));
    SetModuleValue("SpawnEntity", 1, static_cast<float>(Module->GetNum("召唤速度")));
}

void BindTpMod(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("TpMod", Module->IsOn());
    SetModuleValue("TpMod", 0, static_cast<float>(Module->GetModeIndex("传送方式")));
    SetModuleValue("TpMod", 1, static_cast<float>(Module->GetNum("移动速度")));
    SetModuleValue("TpMod", 2, Module->GetBool("坐骑") ? 1.0f : 0.0f);
    SetModuleValue("TpMod", 3, static_cast<float>(Module->GetModeIndex("传送方式")));
    SetModuleValue("TpMod", 4, static_cast<float>(Module->GetNum("移动速度")));
    SetModuleValue("TpMod", 5, Module->GetBool("坐骑") ? 1.0f : 0.0f);
}

}
