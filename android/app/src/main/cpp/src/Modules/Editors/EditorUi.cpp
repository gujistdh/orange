#include "orange/Modules/Editors/EditorUi.h"
#include "orange/Gui/Glow.h"
#include "orange/Gui/LiquidGlass.h"

namespace Orange::Modules::Editors
{
    EditorTheme EditorUi::Default()
    {
        EditorTheme Theme;
        return Theme;
    }

    EditorLayout EditorUi::RowBounds(const EditorTheme& Theme, int Row)
    {
        EditorLayout Layout;
        Layout.X = Theme.PanelX + Theme.Padding;
        Layout.Y = Theme.PanelY + 34.0f + static_cast<float>(Row) * (Theme.RowHeight + Theme.RowGap);
        Layout.Width = Theme.PanelWidth - Theme.Padding * 2.0f;
        Layout.Height = Theme.RowHeight;
        return Layout;
    }

    int EditorUi::VisibleRows(const EditorTheme& Theme)
    {
        float Available = Theme.PanelHeight - 34.0f - Theme.Padding * 2.0f - 24.0f;
        float Step = Theme.RowHeight + Theme.RowGap;
        if (Step <= 0.0f)
        {
            return 0;
        }
        return static_cast<int>(Available / Step);
    }

    void EditorUi::DrawPanel(ImDrawList* Draw, const EditorTheme& Theme, const char* Title)
    {
        if (!Draw)
        {
            return;
        }
        Orange::Gui::Glass::Panel(Draw, ImVec2(Theme.PanelX, Theme.PanelY),
                                  ImVec2(Theme.PanelX + Theme.PanelWidth, Theme.PanelY + Theme.PanelHeight),
                                  Theme.PanelRadius, Theme.PanelAlpha);
        Orange::Gui::LiquidGlass::AddRect(Theme.PanelX, Theme.PanelY, Theme.PanelWidth, Theme.PanelHeight,
                                          Theme.PanelRadius, Theme.PanelAlpha);
        if (Title)
        {
            Orange::Gui::Glow::Text(Draw, Theme.PanelX + Theme.Padding, Theme.PanelY + 10.0f,
                                    Theme.TextColor, Title, 0.8f);
        }
        Draw->AddLine(ImVec2(Theme.PanelX + Theme.Padding, Theme.PanelY + 30.0f),
                      ImVec2(Theme.PanelX + Theme.PanelWidth - Theme.Padding, Theme.PanelY + 30.0f),
                      0x30FFFFFFu, 1.0f);
    }

    void EditorUi::DrawChipAt(ImDrawList* Draw, const EditorTheme& Theme, const EditorLayout& Layout,
                              const char* Text, bool Selected)
    {
        if (!Draw)
        {
            return;
        }
        unsigned int Color = Selected ? Theme.AccentColor : Theme.ChipColor;
        Orange::Gui::Glass::Chip(Draw, ImVec2(Layout.X, Layout.Y),
                                 ImVec2(Layout.X + Layout.Width, Layout.Y + Layout.Height),
                                 Theme.ChipRadius, Color, Theme.ChipAlpha);
        if (Text)
        {
            Orange::Gui::Glow::Text(Draw, Layout.X + 8.0f, Layout.Y + 5.0f,
                                    Theme.TextColor, Text, Selected ? 0.8f : 0.3f);
        }
    }

    void EditorUi::DrawChip(ImDrawList* Draw, const EditorTheme& Theme, int Row, const char* Text, bool Selected)
    {
        DrawChipAt(Draw, Theme, RowBounds(Theme, Row), Text, Selected);
    }

    void EditorUi::DrawLabel(ImDrawList* Draw, const EditorTheme& Theme, int Row, const char* Text)
    {
        if (!Draw || !Text)
        {
            return;
        }
        EditorLayout Layout = RowBounds(Theme, Row);
        Orange::Gui::Glow::Text(Draw, Layout.X + 8.0f, Layout.Y + 5.0f, Theme.TextColor, Text, 0.3f);
    }

    void EditorUi::DrawFooter(ImDrawList* Draw, const EditorTheme& Theme, const char* Text)
    {
        if (!Draw || !Text)
        {
            return;
        }
        Orange::Gui::Glow::Text(Draw, Theme.PanelX + Theme.Padding,
                                Theme.PanelY + Theme.PanelHeight - 22.0f,
                                0x80FFFFFFu, Text, 0.0f);
    }

    bool EditorUi::HitTestPanel(const EditorTheme& Theme, float X, float Y)
    {
        return X >= Theme.PanelX && X <= Theme.PanelX + Theme.PanelWidth &&
               Y >= Theme.PanelY && Y <= Theme.PanelY + Theme.PanelHeight;
    }

    int EditorUi::HitTestRow(const EditorTheme& Theme, float X, float Y, int RowCount)
    {
        if (!HitTestPanel(Theme, X, Y))
        {
            return -1;
        }
        float Top = Theme.PanelY + 34.0f;
        if (Y < Top)
        {
            return -1;
        }
        float Step = Theme.RowHeight + Theme.RowGap;
        if (Step <= 0.0f)
        {
            return -1;
        }
        int Row = static_cast<int>((Y - Top) / Step);
        if (Row < 0 || Row >= RowCount)
        {
            return -1;
        }
        float RowTop = Top + static_cast<float>(Row) * Step;
        if (Y > RowTop + Theme.RowHeight)
        {
            return -1;
        }
        float Left = Theme.PanelX + Theme.Padding;
        float Right = Theme.PanelX + Theme.PanelWidth - Theme.Padding;
        if (X < Left || X > Right)
        {
            return -1;
        }
        return Row;
    }
}
