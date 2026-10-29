#include "orange/Modules/Movement.h"
#include "orange/Modules/State.h"

namespace Orange::Modules::Movement
{

void BindAutoForward(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AutoForward", Module->IsOn());
}

void BindAutoGlide(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AutoGlide", Module->IsOn());
}

void BindAutoJump(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AutoJump", Module->IsOn());
}

void BindAutoSprint(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AutoSprint", Module->IsOn());
    SetModuleValue("AutoSprint", 0, static_cast<float>(Module->GetNum("速度")));
    SetModuleValue("AutoSprint", 1, Module->GetBool("额外速度") ? 1.0f : 0.0f);
}

void BindAutoSwim(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("AutoSwim", Module->IsOn());
}

void BindClickTP(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("ClickTP", Module->IsOn());
}

void BindJumpHack(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("JumpHack", Module->IsOn());
    SetModuleValue("JumpHack", 0, static_cast<float>(Module->GetNum("速度")));
}

void BindPlayerMomentum(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("PlayerMomentum", Module->IsOn());
    SetModuleValue("PlayerMomentum", 0, static_cast<float>(Module->GetNum("移动速度")));
    SetModuleValue("PlayerMomentum", 1, static_cast<float>(Module->GetNum("上下速度")));
}

void BindScaffold(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Scaffold", Module->IsOn());
    SetModuleValue("Scaffold", 0, Module->GetBool("自动换方块") ? 1.0f : 0.0f);
}

void BindVoidRebound(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("VoidRebound", Module->IsOn());
    SetModuleValue("VoidRebound", 0, static_cast<float>(Module->GetNum("方块")));
    SetModuleValue("VoidRebound", 1, static_cast<float>(Module->GetModeIndex("移动")));
    SetModuleValue("VoidRebound", 2, static_cast<float>(Module->GetNum("动量")));
}

void BindWaterWalking(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("WaterWalking", Module->IsOn());
}

}
