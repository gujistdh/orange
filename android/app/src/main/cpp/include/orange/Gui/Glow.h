#pragma once

#include "orange/Common.h"

namespace Orange::Gui
{
    class Glow
    {
    public:
        static void Rect(ImDrawList* Draw, float X, float Y, float W, float H, unsigned int Color, float Intensity);
        static void RectFilled(ImDrawList* Draw, float X, float Y, float W, float H, unsigned int Color, float Intensity);
        static void Line(ImDrawList* Draw, float X1, float Y1, float X2, float Y2, unsigned int Color, float Thickness, float Intensity);
        static void Polyline(ImDrawList* Draw, const float* Points, int Count, unsigned int Color, float Thickness, float Intensity);
        static void Circle(ImDrawList* Draw, float X, float Y, float Radius, unsigned int Color, float Thickness, float Intensity);
        static void CircleFilled(ImDrawList* Draw, float X, float Y, float Radius, unsigned int Color, float Intensity);
        static void QuadFilled(ImDrawList* Draw, float X1, float Y1, float X2, float Y2,
                               float X3, float Y3, float X4, float Y4, unsigned int Color, float Intensity);
        static void Text(ImDrawList* Draw, float X, float Y, unsigned int Color, const char* Text, float Intensity);

        static float ClampIntensity(float Value);
        static unsigned int Tint(unsigned int Color, float Factor);
        static float LayerRadius(int Layer, float Intensity);
        static float LayerAlpha(int Layer);
    };
}
