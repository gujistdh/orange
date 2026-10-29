#pragma once

#include "orange/Common.h"

namespace Orange::Modules::Editors
{
    struct EditorTheme
    {
        float PanelX = 40.0f;
        float PanelY = 120.0f;
        float PanelWidth = 360.0f;
        float PanelHeight = 460.0f;
        float Padding = 10.0f;
        float RowHeight = 26.0f;
        float RowGap = 4.0f;
        float ChipRadius = 6.0f;
        float PanelRadius = 10.0f;
        float PanelAlpha = 0.92f;
        float ChipAlpha = 0.75f;
        float TextScale = 1.0f;
        unsigned int TextColor = 0xFFFFFFFFu;
        unsigned int AccentColor = 0xFF3A7BD5u;
        unsigned int ChipColor = 0x40FFFFFFu;
    };

    struct EditorLayout
    {
        float X = 0.0f;
        float Y = 0.0f;
        float Width = 0.0f;
        float Height = 0.0f;
    };

    class EditorUi
    {
    public:
        static EditorTheme Default();

        static void DrawPanel(ImDrawList* Draw, const EditorTheme& Theme, const char* Title);
        static void DrawChip(ImDrawList* Draw, const EditorTheme& Theme, int Row, const char* Text, bool Selected);
        static void DrawChipAt(ImDrawList* Draw, const EditorTheme& Theme, const EditorLayout& Layout,
                               const char* Text, bool Selected);
        static void DrawLabel(ImDrawList* Draw, const EditorTheme& Theme, int Row, const char* Text);
        static void DrawFooter(ImDrawList* Draw, const EditorTheme& Theme, const char* Text);

        static EditorLayout RowBounds(const EditorTheme& Theme, int Row);
        static int HitTestRow(const EditorTheme& Theme, float X, float Y, int RowCount);
        static bool HitTestPanel(const EditorTheme& Theme, float X, float Y);
        static int VisibleRows(const EditorTheme& Theme);
    };
}
