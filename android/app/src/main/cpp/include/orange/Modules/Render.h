#pragma once

#include "orange/Modules/ModuleRegistry.h"

namespace Orange::Modules::Render
{
    void BindBlockOutline(Gui::Module* Module);
    void BindBlockRender(Gui::Module* Module);
    void BindDirectionSign(Gui::Module* Module);
    void BindDropped3D(Gui::Module* Module);
    void BindEffectRender(Gui::Module* Module);
    void BindEntityTrace(Gui::Module* Module);
    void BindEsp(Gui::Module* Module);
    void BindFeatureList(Gui::Module* Module);
    void BindFloatingText(Gui::Module* Module);
    void BindFog(Gui::Module* Module);
    void BindFov(Gui::Module* Module);
    void BindFuncNotif(Gui::Module* Module);
    void BindHandheldRotation(Gui::Module* Module);
    void BindHeadHalo(Gui::Module* Module);
    void BindHelditeminfo(Gui::Module* Module);
    void BindHighlightInfo(Gui::Module* Module);
    void BindLandingAura(Gui::Module* Module);
    void BindPlaceRender(Gui::Module* Module);
    void BindRadarWarning(Gui::Module* Module);
    void BindSelfAura(Gui::Module* Module);
    void BindSetTime(Gui::Module* Module);
    void BindSetWeather(Gui::Module* Module);
    void BindShowCoords(Gui::Module* Module);
    void BindTagRender(Gui::Module* Module);
    void BindTrail(Gui::Module* Module);
    void BindViewCommandBlock(Gui::Module* Module);
    void BindWatermark(Gui::Module* Module);
    void BindWorldImprisonment(Gui::Module* Module);
    void BindWorldRender(Gui::Module* Module);
}
