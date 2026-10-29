#include "orange/Gui/Glow.h"

namespace Orange::Gui
{
    namespace
    {
        constexpr int kGlowLayers = 6;
        constexpr float kGlowSpread = 8.0f;
        constexpr float kGlowMaxIntensity = 3.0f;
        constexpr float kGlowMinThickness = 0.1f;

        const float kRadiusTable[8] = { 0.0f, 0.0974f, 0.2397f, 0.4061f, 0.5903f, 0.789f, 1.0f, 0.0f };
        const float kAlphaTable[8] = { 0.0f, 0.2911f, 0.2344f, 0.1806f, 0.1303f, 0.085f, 0.05f, 0.0f };
    }

    float Glow::ClampIntensity(float Value)
    {
        if (Value < 0.0f)
        {
            return 0.0f;
        }
        if (Value <= kGlowMaxIntensity)
        {
            return Value;
        }
        return kGlowMaxIntensity;
    }

    float Glow::LayerRadius(int Layer, float Intensity)
    {
        if (Layer < 0 || Layer >= 8)
        {
            return 0.0f;
        }
        return Intensity * kGlowSpread * kRadiusTable[Layer] * 2.0f;
    }

    float Glow::LayerAlpha(int Layer)
    {
        if (Layer < 0 || Layer >= 8)
        {
            return 0.0f;
        }
        return kAlphaTable[Layer];
    }

    unsigned int Glow::Tint(unsigned int Color, float Factor)
    {
        unsigned int Alpha = (Color >> 24) & 0xFFu;
        int Scaled = static_cast<int>(static_cast<float>(Alpha) * Factor + 0.5f);
        if (Scaled < 0)
        {
            Scaled = 0;
        }
        if (Scaled > 255)
        {
            Scaled = 255;
        }
        return (Color & 0x00FFFFFFu) | (static_cast<unsigned int>(Scaled) << 24);
    }

    void Glow::Line(ImDrawList* Draw, float X1, float Y1, float X2, float Y2, unsigned int Color, float Thickness, float Intensity)
    {
        if (!Draw)
        {
            return;
        }
        float Strength = ClampIntensity(Intensity);
        float Base = Thickness <= kGlowMinThickness ? kGlowMinThickness : Thickness;
        if (Strength > 0.001f)
        {
            for (int Layer = kGlowLayers; Layer >= 1; --Layer)
            {
                float Width = Base + LayerRadius(Layer, Strength);
                unsigned int LayerColor = Tint(Color, LayerAlpha(Layer) * Strength);
                Draw->AddLine(ImVec2(X1, Y1), ImVec2(X2, Y2), LayerColor, Width);
            }
        }
        Draw->AddLine(ImVec2(X1, Y1), ImVec2(X2, Y2), Color, Base);
    }

    void Glow::Rect(ImDrawList* Draw, float X, float Y, float W, float H, unsigned int Color, float Intensity)
    {
        if (!Draw)
        {
            return;
        }
        float Strength = ClampIntensity(Intensity);
        if (Strength > 0.001f)
        {
            for (int Layer = kGlowLayers; Layer >= 1; --Layer)
            {
                float Expand = LayerRadius(Layer, Strength);
                unsigned int LayerColor = Tint(Color, LayerAlpha(Layer) * Strength);
                Draw->AddRect(ImVec2(X - Expand, Y - Expand), ImVec2(X + W + Expand, Y + H + Expand), LayerColor);
            }
        }
        Draw->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), Color);
    }

    void Glow::RectFilled(ImDrawList* Draw, float X, float Y, float W, float H, unsigned int Color, float Intensity)
    {
        if (!Draw)
        {
            return;
        }
        float Strength = ClampIntensity(Intensity);
        if (Strength > 0.001f)
        {
            for (int Layer = kGlowLayers; Layer >= 1; --Layer)
            {
                float Expand = LayerRadius(Layer, Strength);
                unsigned int LayerColor = Tint(Color, LayerAlpha(Layer) * Strength);
                Draw->AddRectFilled(ImVec2(X - Expand, Y - Expand), ImVec2(X + W + Expand, Y + H + Expand), LayerColor);
            }
        }
        Draw->AddRectFilled(ImVec2(X, Y), ImVec2(X + W, Y + H), Color);
    }

    void Glow::Circle(ImDrawList* Draw, float X, float Y, float Radius, unsigned int Color, float Thickness, float Intensity)
    {
        if (!Draw)
        {
            return;
        }
        float Strength = ClampIntensity(Intensity);
        float Base = Thickness <= kGlowMinThickness ? kGlowMinThickness : Thickness;
        if (Strength > 0.001f)
        {
            for (int Layer = kGlowLayers; Layer >= 1; --Layer)
            {
                float Expand = LayerRadius(Layer, Strength);
                unsigned int LayerColor = Tint(Color, LayerAlpha(Layer) * Strength);
                Draw->AddCircle(ImVec2(X, Y), Radius + Expand, LayerColor, 0, Base + Expand * 2.0f);
            }
        }
        Draw->AddCircle(ImVec2(X, Y), Radius, Color, 0, Base);
    }

    void Glow::CircleFilled(ImDrawList* Draw, float X, float Y, float Radius, unsigned int Color, float Intensity)
    {
        if (!Draw)
        {
            return;
        }
        float Strength = ClampIntensity(Intensity);
        if (Strength > 0.001f)
        {
            for (int Layer = kGlowLayers; Layer >= 3; --Layer)
            {
                float Expand = LayerRadius(Layer, Strength);
                unsigned int LayerColor = Tint(Color, LayerAlpha(Layer) * Strength);
                Draw->AddCircleFilled(ImVec2(X, Y), Radius + Expand, LayerColor, 0);
            }
        }
        Draw->AddCircleFilled(ImVec2(X, Y), Radius, Color, 0);
    }

    void Glow::Polyline(ImDrawList* Draw, const float* Points, int Count, unsigned int Color, float Thickness, float Intensity)
    {
        if (!Draw || !Points || Count < 2)
        {
            return;
        }
        float Strength = ClampIntensity(Intensity);
        float Base = Thickness <= kGlowMinThickness ? kGlowMinThickness : Thickness;
        if (Strength > 0.001f)
        {
            for (int Layer = kGlowLayers; Layer >= 1; --Layer)
            {
                float Width = Base + LayerRadius(Layer, Strength);
                unsigned int LayerColor = Tint(Color, LayerAlpha(Layer) * Strength);
                for (int Index = 0; Index < Count - 1; ++Index)
                {
                    Draw->AddLine(ImVec2(Points[Index * 2], Points[Index * 2 + 1]),
                                  ImVec2(Points[(Index + 1) * 2], Points[(Index + 1) * 2 + 1]), LayerColor, Width);
                }
            }
        }
        for (int Index = 0; Index < Count - 1; ++Index)
        {
            Draw->AddLine(ImVec2(Points[Index * 2], Points[Index * 2 + 1]),
                          ImVec2(Points[(Index + 1) * 2], Points[(Index + 1) * 2 + 1]), Color, Base);
        }
    }

    void Glow::QuadFilled(ImDrawList* Draw, float X1, float Y1, float X2, float Y2,
                          float X3, float Y3, float X4, float Y4, unsigned int Color, float Intensity)
    {
        if (!Draw)
        {
            return;
        }
        float Strength = ClampIntensity(Intensity);
        if (Strength > 0.001f)
        {
            for (int Layer = kGlowLayers; Layer >= 1; --Layer)
            {
                float Expand = LayerRadius(Layer, Strength);
                unsigned int LayerColor = Tint(Color, LayerAlpha(Layer) * Strength);
                Draw->AddQuadFilled(ImVec2(X1 - Expand, Y1 - Expand), ImVec2(X2 + Expand, Y2 - Expand),
                                    ImVec2(X3 + Expand, Y3 + Expand), ImVec2(X4 - Expand, Y4 + Expand), LayerColor);
            }
        }
        Draw->AddQuadFilled(ImVec2(X1, Y1), ImVec2(X2, Y2), ImVec2(X3, Y3), ImVec2(X4, Y4), Color);
    }

    void Glow::Text(ImDrawList* Draw, float X, float Y, unsigned int Color, const char* Text, float Intensity)
    {
        if (!Draw || !Text)
        {
            return;
        }
        float Strength = ClampIntensity(Intensity);
        if (Strength > 0.001f)
        {
            for (int Layer = kGlowLayers; Layer >= 1; --Layer)
            {
                unsigned int LayerColor = Tint(Color, LayerAlpha(Layer) * Strength);
                Draw->AddText(ImVec2(X, Y), LayerColor, Text);
            }
        }
        Draw->AddText(ImVec2(X, Y), Color, Text);
    }
}
