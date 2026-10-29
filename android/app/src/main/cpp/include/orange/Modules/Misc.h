#pragma once

#include "orange/Modules/ModuleRegistry.h"

namespace Orange::Modules::Misc
{
    void BindGuiRender(Gui::Module* Module);
    void BindMusicPlayer(Gui::Module* Module);
    void BindRocketModel(Gui::Module* Module);
}
