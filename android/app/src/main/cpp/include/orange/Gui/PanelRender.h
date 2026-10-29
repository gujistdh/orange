#pragma once

#include "orange/Common.h"

namespace Orange::Gui
{
    class PanelRender
    {
    public:
        static void SetVisible(bool Value);
        static bool IsVisible();
        static void ToggleVisible();

        static void Draw(ImDrawList* Draw, float ScreenWidth, float ScreenHeight);
        static bool OnTouch(int Action, float X, float Y, float ScreenWidth, float ScreenHeight);

        static int ActivePanel();
        static void SetActivePanel(int Value);
        static float PanelScroll(int Index);
        static void SetPanelScroll(int Index, float Value);
        static int HoveredModule();
        static void Reset();
    };
}
