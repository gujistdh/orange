#include "orange/Gui/PanelRender.h"
#include "orange/Gui/Gui.h"
#include "orange/Gui/Glow.h"
#include "orange/Gui/LiquidGlass.h"
#include "orange/Gui/Module.h"
#include "orange/Gui/Panel.h"

namespace Orange::Gui
{
    namespace
    {
        constexpr float kPanelWidth = 320.0f;
        constexpr float kPanelHeight = 520.0f;
        constexpr float kHeaderHeight = 36.0f;
        constexpr float kRowHeight = 28.0f;
        constexpr float kRowGap = 4.0f;
        constexpr float kPadding = 10.0f;
        constexpr float kRadius = 10.0f;

        bool gVisible = false;
        int gActivePanel = 0;
        int gHoveredModule = -1;
        float gScroll[16] = { 0.0f };
    }

    void PanelRender::SetVisible(bool Value)
    {
        gVisible = Value;
    }

    bool PanelRender::IsVisible()
    {
        return gVisible;
    }

    void PanelRender::ToggleVisible()
    {
        gVisible = !gVisible;
    }

    int PanelRender::ActivePanel()
    {
        return gActivePanel;
    }

    void PanelRender::SetActivePanel(int Value)
    {
        gActivePanel = Value;
    }

    float PanelRender::PanelScroll(int Index)
    {
        if (Index < 0 || Index >= 16)
        {
            return 0.0f;
        }
        return gScroll[Index];
    }

    void PanelRender::SetPanelScroll(int Index, float Value)
    {
        if (Index < 0 || Index >= 16)
        {
            return;
        }
        gScroll[Index] = Value < 0.0f ? 0.0f : Value;
    }

    int PanelRender::HoveredModule()
    {
        return gHoveredModule;
    }

    void PanelRender::Reset()
    {
        gVisible = false;
        gActivePanel = 0;
        gHoveredModule = -1;
        for (int Index = 0; Index < 16; ++Index)
        {
            gScroll[Index] = 0.0f;
        }
    }

    void PanelRender::Draw(ImDrawList* Draw, float ScreenWidth, float ScreenHeight)
    {
        if (!gVisible || !Draw)
        {
            return;
        }
        std::vector<Panel*>& Panels = GetPanels();
        if (Panels.empty())
        {
            return;
        }
        if (gActivePanel < 0 || static_cast<std::size_t>(gActivePanel) >= Panels.size())
        {
            gActivePanel = 0;
        }

        float TabWidth = 76.0f;
        float TabY = 40.0f;
        for (std::size_t Index = 0; Index < Panels.size(); ++Index)
        {
            float TabX = 40.0f + static_cast<float>(Index) * (TabWidth + 6.0f);
            bool Active = static_cast<int>(Index) == gActivePanel;
            unsigned int TabColor = Active ? 0xFF3A7BD5u : 0x40FFFFFFu;
            Glass::Chip(Draw, ImVec2(TabX, TabY), ImVec2(TabX + TabWidth, TabY + 30.0f), 6.0f, TabColor, 0.85f);
            Glow::Text(Draw, TabX + 10.0f, TabY + 7.0f, 0xFFFFFFFFu,
                       Panels[Index]->GetCategory().c_str(), Active ? UiIntensity() : 0.0f);
        }

        Panel* Current = Panels[static_cast<std::size_t>(gActivePanel)];
        float PanelX = 40.0f;
        float PanelY = TabY + 42.0f;
        Glass::Panel(Draw, ImVec2(PanelX, PanelY), ImVec2(PanelX + kPanelWidth, PanelY + kPanelHeight), kRadius, 0.92f);
        LiquidGlass::AddRect(PanelX, PanelY, kPanelWidth, kPanelHeight, kRadius, 0.92f);

        Glow::Text(Draw, PanelX + kPadding, PanelY + 10.0f, 0xFFFFFFFFu, Current->GetName().c_str(), 0.8f);
        Draw->AddLine(ImVec2(PanelX + kPadding, PanelY + kHeaderHeight - 6.0f),
                      ImVec2(PanelX + kPanelWidth - kPadding, PanelY + kHeaderHeight - 6.0f), 0x30FFFFFFu, 1.0f);

        const std::vector<Module*>& Modules = Current->GetModules();
        float RowY = PanelY + kHeaderHeight + 4.0f - gScroll[gActivePanel];
        float ClipTop = PanelY + kHeaderHeight;
        float ClipBottom = PanelY + kPanelHeight - kPadding;
        for (std::size_t Index = 0; Index < Modules.size(); ++Index)
        {
            Module* Item = Modules[Index];
            if (!Item)
            {
                continue;
            }
            float Top = RowY;
            float Bottom = RowY + kRowHeight;
            RowY += kRowHeight + kRowGap;
            if (Bottom < ClipTop || Top > ClipBottom)
            {
                continue;
            }
            unsigned int ChipColor = Item->IsOn() ? 0xFF2E7D32u : 0x30FFFFFFu;
            Glass::Chip(Draw, ImVec2(PanelX + kPadding, Top),
                        ImVec2(PanelX + kPanelWidth - kPadding, Bottom), 6.0f, ChipColor, 0.8f);
            Glow::Text(Draw, PanelX + kPadding + 10.0f, Top + 6.0f, 0xFFFFFFFFu,
                       Item->GetName().c_str(), Item->IsOn() ? UiIntensity() : 0.0f);
            if (Item->HasProps())
            {
                Glow::Text(Draw, PanelX + kPanelWidth - kPadding - 18.0f, Top + 6.0f, 0x80FFFFFFu, ">", 0.0f);
            }
        }
    }

    bool PanelRender::OnTouch(int Action, float X, float Y, float ScreenWidth, float ScreenHeight)
    {
        (void)ScreenWidth;
        (void)ScreenHeight;
        if (!gVisible)
        {
            return false;
        }
        if (Action != 0)
        {
            return false;
        }
        std::vector<Panel*>& Panels = GetPanels();
        float TabWidth = 76.0f;
        float TabY = 40.0f;
        for (std::size_t Index = 0; Index < Panels.size(); ++Index)
        {
            float TabX = 40.0f + static_cast<float>(Index) * (TabWidth + 6.0f);
            if (X >= TabX && X <= TabX + TabWidth && Y >= TabY && Y <= TabY + 30.0f)
            {
                gActivePanel = static_cast<int>(Index);
                return true;
            }
        }
        if (Panels.empty())
        {
            return false;
        }
        Panel* Current = Panels[static_cast<std::size_t>(gActivePanel)];
        float PanelX = 40.0f;
        float PanelY = TabY + 42.0f;
        if (X < PanelX || X > PanelX + kPanelWidth || Y < PanelY + kHeaderHeight || Y > PanelY + kPanelHeight)
        {
            return false;
        }
        const std::vector<Module*>& Modules = Current->GetModules();
        float RowY = PanelY + kHeaderHeight + 4.0f - gScroll[gActivePanel];
        for (std::size_t Index = 0; Index < Modules.size(); ++Index)
        {
            float Top = RowY;
            float Bottom = RowY + kRowHeight;
            RowY += kRowHeight + kRowGap;
            if (Y >= Top && Y <= Bottom)
            {
                Module* Item = Modules[Index];
                if (Item)
                {
                    bool Next = !Item->IsOn();
                    Item->SetOn(Next);
                    if (Next)
                    {
                        Item->Apply();
                    }
                    gHoveredModule = static_cast<int>(Index);
                }
                return true;
            }
        }
        return false;
    }
}
