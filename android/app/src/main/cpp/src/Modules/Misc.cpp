#include "orange/Modules/Misc.h"
#include "orange/Modules/State.h"

namespace Orange::Modules::Misc
{

void BindGuiRender(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("LiquidGlass", Module->IsOn());
    SetModuleValue("LiquidGlass", 0, static_cast<float>(Module->GetNum("BlurRadius")));
    SetModuleValue("LiquidGlass", 1, static_cast<float>(Module->GetNum("RefractionHeight")));
    SetModuleValue("LiquidGlass", 2, static_cast<float>(Module->GetNum("RefractionAmount")));
    SetModuleValue("LiquidGlass", 3, static_cast<float>(Module->GetNum("ChromaticAberration")));
}

void BindMusicPlayer(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("MusicPlayer", Module->IsOn());
}

void BindRocketModel(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("RocketModel", Module->IsOn());
}

}
