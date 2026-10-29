#pragma once

#include "orange/Modules/ModuleRegistry.h"

namespace Orange::Modules::Movement
{
    void BindAutoForward(Gui::Module* Module);
    void BindAutoGlide(Gui::Module* Module);
    void BindAutoJump(Gui::Module* Module);
    void BindAutoSprint(Gui::Module* Module);
    void BindAutoSwim(Gui::Module* Module);
    void BindClickTP(Gui::Module* Module);
    void BindJumpHack(Gui::Module* Module);
    void BindPlayerMomentum(Gui::Module* Module);
    void BindScaffold(Gui::Module* Module);
    void BindVoidRebound(Gui::Module* Module);
    void BindWaterWalking(Gui::Module* Module);
}
