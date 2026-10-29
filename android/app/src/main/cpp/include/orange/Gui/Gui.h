#pragma once

#include "orange/Common.h"
#include "orange/Gui/Module.h"
#include "orange/Gui/Panel.h"
#include "orange/Gui/JsonValue.h"

namespace Orange::Gui
{
    void Build();
    void BuildPanels(std::vector<Panel*>& Out);
    std::vector<Panel*>& GetPanels();

    Panel* CreatePanel(const std::string& Name, const std::string& Category);
    Module* BuildModule(const JsonValue& Definition);

    Module* FindModule(const std::string& Id);
    Module* FindBinder(const std::string& Id);

    int DefaultPanelCount();
    const char* DefaultPanelFileName(int Index);
    const char* DefaultPanelJson(int Index);

    float BinderToFloat(const std::string& Text);
    unsigned int BinderColorOf(Module* Owner, const char* Name);

    unsigned int ParseColor(const std::string& Text);
    const std::vector<std::string>& ColorPresets();

    float UiUnit();

    bool IsWatermarkEnabled();
    void SetWatermarkEnabled(bool Value, float Delay);

    bool IsNotifyEnabled();
    void SetNotifyEnabled(bool Value, float Delay);

    bool IsLimitedMode();

    void SetActionBarText(const std::string& Text, float Delay);
    bool GetActionBarText(std::string& OutText, float& OutAlpha);
    int ActionBarStyle();
    int ActionBarColor();
    int ActionBarFlags();
    float WatermarkIntensity();
    float UiIntensity();
    void SetUiIntensity(float Value);
    float NotifyIntensity();
    void DrawActionBar(ImDrawList* Draw);
    void SetActionBarStyle(int Style, int Color, int Flags);

    void OpenConfigGui(Module* Target);
    void OpenMusicGui();
    void OpenPyLoaderGui();
    void OpenNbtValueInput(const std::string& Path);

    bool OnTouch(int Action, float X, float Y);
    void OnTextInputResult(const std::string& Text);

    void AuthClose();
    void AuthRegisterOk(const std::string& Text);

    extern int kBindersMainCount;
    extern int kBindersExtraCount;
}
