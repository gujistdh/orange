#pragma once

#include "orange/Modules/ModuleRegistry.h"

namespace Orange::Modules::World
{
    void BindAutoBreak(Gui::Module* Module);
    void BindBreakBlock(Gui::Module* Module);
    void BindChestStealer(Gui::Module* Module);
    void BindClickBreak(Gui::Module* Module);
    void BindGameMode(Gui::Module* Module);
    void BindGameSpeed(Gui::Module* Module);
    void BindPacketResending(Gui::Module* Module);
    void BindPosText(Gui::Module* Module);
    void BindRideRootFlight(Gui::Module* Module);
    void BindStopPackets(Gui::Module* Module);
    void BindSummonEntity(Gui::Module* Module);
    void BindTpMod(Gui::Module* Module);
}
