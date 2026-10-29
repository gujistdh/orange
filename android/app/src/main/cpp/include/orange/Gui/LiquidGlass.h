#pragma once

#include "orange/Common.h"

namespace Orange::Gui
{
    class LiquidGlass
    {
    public:
        static void SetEnabled(bool Value);
        static bool IsEnabled();

        static void SetParams(float Blur, float Refraction, float Height, float Aberration);
        static float BlurRadius();
        static float RefractionAmount();
        static float RefractionHeight();
        static float ChromaticAberration();

        static void BeginFrame();
        static void AddRect(float X, float Y, float W, float H, float Radius, float Alpha);
        static void AddRectClipped(float X, float Y, float W, float H, float Radius, float Alpha,
                                   float ClipX, float ClipY, float ClipW, float ClipH);
        static void Render(int Width, int Height);
        static void Release();
        static void Invalidate();
        static bool CaptureBackdrop(int Width, int Height);

    private:
        static bool EnsureProgram();
        static bool EnsureTexture(int Width, int Height);
    };

    class Glass
    {
    public:
        static unsigned int Col(unsigned int Color, float Alpha);
        static void Panel(ImDrawList* Draw, const ImVec2& Min, const ImVec2& Max, float Radius, float Alpha);
        static void Chip(ImDrawList* Draw, const ImVec2& Min, const ImVec2& Max, float Radius, unsigned int Color, float Alpha);
    };
}
