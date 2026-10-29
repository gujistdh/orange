#include "orange/Modules/Script.h"
#include "orange/Modules/State.h"

namespace Orange::Modules::Script
{

void BindBuildingTools(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("BuildingTools", Module->IsOn());
    SetModuleValue("BuildingTools", 0, static_cast<float>(Module->GetModeIndex("类型")));
}

void BindCardKeyTime(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("CardKeyTime", Module->IsOn());
}

void BindChatEffects(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("ChatEffects", Module->IsOn());
    SetModuleValue("ChatEffects", 0, static_cast<float>(Module->GetModeIndex("方式")));
    SetModuleValue("ChatEffects", 1, Module->GetBool("乱码") ? 1.0f : 0.0f);
    SetModuleValue("ChatEffects", 2, Module->GetBool("加粗") ? 1.0f : 0.0f);
    SetModuleValue("ChatEffects", 3, Module->GetBool("斜体") ? 1.0f : 0.0f);
}

void BindChatroom(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Chatroom", Module->IsOn());
}

void BindLoadConfig(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("LoadConfig", Module->IsOn());
}

void BindMessage(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Text", Module->IsOn());
    SetModuleValue("Text", 0, static_cast<float>(Module->GetNum("间隔")));
    SetModuleValue("Text", 1, static_cast<float>(Module->GetModeIndex("方式")));
}

void BindPyLoader(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Python", Module->IsOn());
}

void BindRunCommand(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("RunCommand", Module->IsOn());
    SetModuleValue("RunCommand", 0, static_cast<float>(Module->GetNum("速度")));
    SetModuleValue("RunCommand", 1, Module->GetBool("文件执行") ? 1.0f : 0.0f);
}

void BindSaveConfig(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("SaveConfig", Module->IsOn());
}

void BindViewUser(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Viewuser", Module->IsOn());
    SetModuleValue("Viewuser", 0, static_cast<float>(Module->GetNum("Gui大小")));
    SetModuleValue("Viewuser", 1, static_cast<float>(Module->GetNum("Gui圆角")));
}

void BindWatermarkText(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("WatermarkText", Module->IsOn());
    SetModuleValue("WatermarkText", 0, static_cast<float>(Module->GetNum("大小")));
    SetModuleValue("WatermarkText", 1, static_cast<float>(Module->GetNum("不透明")));
}

}
