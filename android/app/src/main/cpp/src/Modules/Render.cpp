#include "orange/Modules/Render.h"
#include "orange/Modules/State.h"

namespace Orange::Modules::Render
{

void BindBlockOutline(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("BlockOutline", Module->IsOn());
    SetModuleValue("BlockOutline", 0, static_cast<float>(Module->GetNum("线条大小")));
    SetModuleValue("BlockOutline", 1, static_cast<float>(Module->GetModeIndex("颜色")));
}

void BindBlockRender(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("BlockRender", Module->IsOn());
    SetModuleValue("BlockRender", 0, static_cast<float>(Module->GetNum("速度")));
    SetModuleValue("BlockRender", 1, static_cast<float>(Module->GetNum("范围")));
    SetModuleValue("BlockRender", 2, static_cast<float>(Module->GetNum("数量")));
    SetModuleValue("BlockRender", 3, static_cast<float>(Module->GetNum("渲染类型")));
    SetModuleValue("BlockRender", 4, static_cast<float>(Module->GetNum("颜色")));
    SetModuleValue("BlockRender", 5, static_cast<float>(Module->GetNum("线条")));
    SetModuleValue("BlockRender", 6, Module->GetBool("射线") ? 1.0f : 0.0f);
    SetModuleValue("BlockRender", 7, Module->GetBool("名称") ? 1.0f : 0.0f);
    SetModuleValue("BlockRender", 8, Module->GetBool("坐标") ? 1.0f : 0.0f);
    SetModuleValue("BlockRender", 9, Module->GetBool("距离") ? 1.0f : 0.0f);
    SetModuleValue("BlockRender", 10, static_cast<float>(Module->GetNum("Gui大小")));
    SetModuleValue("BlockRender", 11, static_cast<float>(Module->GetNum("圆角")));
    SetModuleValue("BlockRender", 12, static_cast<float>(Module->GetNum("不透明")));
}

void BindDirectionSign(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("DirectionSign", Module->IsOn());
    SetModuleValue("DirectionSign", 0, static_cast<float>(Module->GetNum("Gui大小")));
    SetModuleValue("DirectionSign", 1, static_cast<float>(Module->GetNum("圆角")));
    SetModuleValue("DirectionSign", 2, static_cast<float>(Module->GetNum("不透明")));
    SetModuleValue("DirectionSign", 3, static_cast<float>(Module->GetModeIndex("颜色")));
}

void BindDropped3D(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Dropped3D", Module->IsOn());
}

void BindEffectRender(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("EffectRender", Module->IsOn());
    SetModuleValue("EffectRender", 0, static_cast<float>(Module->GetNum("Gui大小")));
    SetModuleValue("EffectRender", 1, static_cast<float>(Module->GetNum("不透明")));
    SetModuleValue("EffectRender", 2, static_cast<float>(Module->GetNum("圆角")));
}

void BindEntityTrace(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("EntityTrace", Module->IsOn());
    SetModuleValue("EntityTrace", 0, static_cast<float>(Module->GetNum("线条")));
    SetModuleValue("EntityTrace", 1, static_cast<float>(Module->GetNum("数量")));
    SetModuleValue("EntityTrace", 2, static_cast<float>(Module->GetCheckMask("目标")));
    SetModuleValue("EntityTrace", 3, static_cast<float>(Module->GetModeIndex("颜色")));
}

void BindEsp(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("ESP", Module->IsOn());
    SetModuleValue("ESP", 0, static_cast<float>(Module->GetNum("线条")));
    SetModuleValue("ESP", 1, static_cast<float>(Module->GetNum("数量")));
    SetModuleValue("ESP", 2, static_cast<float>(Module->GetCheckMask("目标")));
    SetModuleValue("ESP", 3, static_cast<float>(Module->GetModeIndex("类型")));
    SetModuleValue("ESP", 4, static_cast<float>(Module->GetModeIndex("颜色")));
}

void BindFeatureList(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("FeatureList", Module->IsOn());
}

void BindFloatingText(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("FloatingText", Module->IsOn());
    SetModuleValue("FloatingText", 0, static_cast<float>(Module->GetNum("数量")));
    SetModuleValue("FloatingText", 1, static_cast<float>(Module->GetModeIndex("颜色")));
    SetModuleValue("FloatingText", 2, static_cast<float>(Module->GetNum("大小")));
}

void BindFog(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Fog", Module->IsOn());
    SetModuleValue("Fog", 0, static_cast<float>(Module->GetModeIndex("雾效类型")));
}

void BindFov(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Fov", Module->IsOn());
    SetModuleValue("Fov", 0, static_cast<float>(Module->GetNum("视野")));
}

void BindFuncNotif(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("FuncNotif", Module->IsOn());
}

void BindHandheldRotation(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("HandheldRotation", Module->IsOn());
    SetModuleValue("HandheldRotation", 0, static_cast<float>(Module->GetNum("旋转速度")));
}

void BindHeadHalo(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("HeadHalo", Module->IsOn());
    SetModuleValue("HeadHalo", 0, static_cast<float>(Module->GetCheckMask("目标")));
    SetModuleValue("HeadHalo", 1, static_cast<float>(Module->GetModeIndex("颜色")));
}

void BindHelditeminfo(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Helditeminfo", Module->IsOn());
    SetModuleValue("Helditeminfo", 0, static_cast<float>(Module->GetNum("大小")));
    SetModuleValue("Helditeminfo", 1, static_cast<float>(Module->GetNum("圆角")));
    SetModuleValue("Helditeminfo", 2, static_cast<float>(Module->GetNum("颜色")));
}

void BindHighlightInfo(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("HighlightInfo", Module->IsOn());
    SetModuleValue("HighlightInfo", 0, static_cast<float>(Module->GetNum("类型")));
    SetModuleValue("HighlightInfo", 1, static_cast<float>(Module->GetNum("大小")));
    SetModuleValue("HighlightInfo", 2, static_cast<float>(Module->GetNum("圆角")));
    SetModuleValue("HighlightInfo", 3, static_cast<float>(Module->GetNum("不透明")));
    SetModuleValue("HighlightInfo", 4, static_cast<float>(Module->GetNum("颜色")));
}

void BindLandingAura(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("LandingAura", Module->IsOn());
    SetModuleValue("LandingAura", 0, static_cast<float>(Module->GetNum("线条大小")));
    SetModuleValue("LandingAura", 1, static_cast<float>(Module->GetNum("显示时常")));
    SetModuleValue("LandingAura", 2, static_cast<float>(Module->GetModeIndex("颜色")));
    SetModuleValue("LandingAura", 3, static_cast<float>(Module->GetCheckMask("目标")));
}

void BindPlaceRender(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("PlaceRender", Module->IsOn());
    SetModuleValue("PlaceRender", 0, static_cast<float>(Module->GetNum("速度")));
    SetModuleValue("PlaceRender", 1, static_cast<float>(Module->GetNum("类型")));
    SetModuleValue("PlaceRender", 2, static_cast<float>(Module->GetNum("颜色")));
    SetModuleValue("PlaceRender", 3, Module->GetBool("反向") ? 1.0f : 0.0f);
}

void BindRadarWarning(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("RadarWarning", Module->IsOn());
    SetModuleValue("RadarWarning", 0, static_cast<float>(Module->GetNum("雷达大小")));
    SetModuleValue("RadarWarning", 1, static_cast<float>(Module->GetNum("距离")));
    SetModuleValue("RadarWarning", 2, static_cast<float>(Module->GetCheckMask("目标")));
    SetModuleValue("RadarWarning", 3, static_cast<float>(Module->GetNum("雷达圆角")));
}

void BindSelfAura(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("SelfAura", Module->IsOn());
    SetModuleValue("SelfAura", 0, static_cast<float>(Module->GetNum("线条大小")));
    SetModuleValue("SelfAura", 1, static_cast<float>(Module->GetModeIndex("颜色")));
}

void BindSetTime(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("SetTime", Module->IsOn());
    SetModuleValue("SetTime", 0, static_cast<float>(Module->GetNum("时间")));
}

void BindSetWeather(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("SetWeather", Module->IsOn());
    SetModuleValue("SetWeather", 0, static_cast<float>(Module->GetModeIndex("天气")));
}

void BindShowCoords(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("ShowCoords", Module->IsOn());
    SetModuleValue("ShowCoords", 0, static_cast<float>(Module->GetNum("Gui大小")));
    SetModuleValue("ShowCoords", 1, static_cast<float>(Module->GetNum("圆角")));
    SetModuleValue("ShowCoords", 2, static_cast<float>(Module->GetModeIndex("颜色")));
}

void BindTagRender(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("TagRender", Module->IsOn());
    SetModuleValue("TagRender", 0, static_cast<float>(Module->GetCheckMask("目标")));
    SetModuleValue("TagRender", 1, static_cast<float>(Module->GetNum("范围")));
    SetModuleValue("TagRender", 2, static_cast<float>(Module->GetNum("数量")));
    SetModuleValue("TagRender", 3, static_cast<float>(Module->GetModeIndex("颜色")));
    SetModuleValue("TagRender", 4, Module->GetBool("坐标") ? 1.0f : 0.0f);
    SetModuleValue("TagRender", 5, Module->GetBool("名称") ? 1.0f : 0.0f);
    SetModuleValue("TagRender", 6, Module->GetBool("血量") ? 1.0f : 0.0f);
    SetModuleValue("TagRender", 7, Module->GetBool("距离") ? 1.0f : 0.0f);
    SetModuleValue("TagRender", 8, static_cast<float>(Module->GetNum("文字大小")));
}

void BindTrail(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Trail", Module->IsOn());
    SetModuleValue("Trail", 0, static_cast<float>(Module->GetCheckMask("目标")));
    SetModuleValue("Trail", 1, static_cast<float>(Module->GetModeIndex("颜色")));
    SetModuleValue("Trail", 2, static_cast<float>(Module->GetNum("时间")));
}

void BindViewCommandBlock(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("ViewCommandBlock", Module->IsOn());
    SetModuleValue("ViewCommandBlock", 0, static_cast<float>(Module->GetNum("Gui大小")));
    SetModuleValue("ViewCommandBlock", 1, static_cast<float>(Module->GetNum("Gui圆角")));
    SetModuleValue("ViewCommandBlock", 2, Module->GetBool("查看指令") ? 1.0f : 0.0f);
    SetModuleValue("ViewCommandBlock", 3, Module->GetBool("查看悬停注释") ? 1.0f : 0.0f);
    SetModuleValue("ViewCommandBlock", 4, Module->GetBool("查看方块类型") ? 1.0f : 0.0f);
    SetModuleValue("ViewCommandBlock", 5, Module->GetBool("查看条件") ? 1.0f : 0.0f);
    SetModuleValue("ViewCommandBlock", 6, Module->GetBool("查看红石控制") ? 1.0f : 0.0f);
    SetModuleValue("ViewCommandBlock", 7, Module->GetBool("查看延迟刻数") ? 1.0f : 0.0f);
}

void BindWatermark(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("Watermark", Module->IsOn());
}

void BindWorldImprisonment(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("WorldImprisonment", Module->IsOn());
    SetModuleValue("WorldImprisonment", 0, static_cast<float>(Module->GetNum("扩散速度")));
    SetModuleValue("WorldImprisonment", 1, static_cast<float>(Module->GetNum("囚笼半径")));
    SetModuleValue("WorldImprisonment", 2, static_cast<float>(Module->GetNum("石柱高度")));
    SetModuleValue("WorldImprisonment", 3, static_cast<float>(Module->GetNum("天空闪电")));
    SetModuleValue("WorldImprisonment", 4, static_cast<float>(Module->GetNum("符文数量")));
    SetModuleValue("WorldImprisonment", 5, Module->GetBool("实体连接") ? 1.0f : 0.0f);
    SetModuleValue("WorldImprisonment", 6, static_cast<float>(Module->GetNum("线条大小")));
    SetModuleValue("WorldImprisonment", 7, static_cast<float>(Module->GetModeIndex("颜色")));
}

void BindWorldRender(Gui::Module* Module)
{
    if (!Module)
    {
        return;
    }
    SetModuleFlag("WorldRender", Module->IsOn());
    SetModuleValue("WorldRender", 0, static_cast<float>(Module->GetNum("数量")));
}

}
