#pragma once

#include "orange/Modules/ModuleRegistry.h"

namespace Orange::Modules::Script
{
    void BindBuildingTools(Gui::Module* Module);
    void BindCardKeyTime(Gui::Module* Module);
    void BindChatEffects(Gui::Module* Module);
    void BindChatroom(Gui::Module* Module);
    void BindLoadConfig(Gui::Module* Module);
    void BindMessage(Gui::Module* Module);
    void BindPyLoader(Gui::Module* Module);
    void BindRunCommand(Gui::Module* Module);
    void BindSaveConfig(Gui::Module* Module);
    void BindViewUser(Gui::Module* Module);
    void BindWatermarkText(Gui::Module* Module);
}
