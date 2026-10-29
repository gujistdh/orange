#pragma once

#include "orange/Common.h"

namespace Orange::Modules::Render
{
    class Esp
    {
    public:
        static void SetEnabled(float Range, float LineWidth, int Count, bool Value,
                               int TargetMask, int Type, int ColorMode, int Style, float Extra);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class EntityTrace
    {
    public:
        static void SetEnabled(float Range, float Width, int TargetMask, bool Value, int Mode, int ColorMode);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class TagRender
    {
    public:
        static void SetConfig(bool Enabled, int TargetMask, float Scale, int Mode, float Range,
                              bool Health, bool Name, bool Distance, bool Item, int ColorMode);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class HeadHalo
    {
    public:
        static void SetEnabled(bool Enabled, float Radius, int ColorMode);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class Trail
    {
    public:
        static void SetEnabled(bool Enabled, float Width, float Duration, int ColorMode);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class FloatingText
    {
    public:
        static void SetEnabled(bool Enabled, float Scale, float Duration, int Mode, int ColorMode);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class Fog
    {
    public:
        static void SetEnabled(bool Enabled, float Distance);
        static void ProcessTick(void* LocalPlayer);
    };

    class Fov
    {
    public:
        static void SetEnabled(bool Enabled, float Value);
        static void ProcessTick(void* LocalPlayer);
        static float GetCurrentFov();
    };

    class HandheldRotation
    {
    public:
        static bool IsEnabled();
        static void SetEnabled(bool Enabled);
        static void SetSpeed(float Value);
        static float GetAngleDegrees();
        static float GetSpeed();
        static void ProcessTick(void* LocalPlayer);
    };

    class Helditeminfo
    {
    public:
        static void SetConfig(bool Enabled, int Mode, float Scale, int ColorMode);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class BlockOutline
    {
    public:
        static void SetConfig(bool Enabled, int Mode, float Width);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class BlockRender
    {
    public:
        static void SetConfig(bool Enabled, int TargetMask, float Range, int Mode, int ColorMode);
        static void Init();
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class PlaceRender
    {
    public:
        static void SetConfig(bool Enabled, float Range, int Mode, int ColorMode, float Width);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class LandingAura
    {
    public:
        static void SetEnabled(bool Enabled, float Radius, int Mode, int ColorMode, float Width);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class SelfAura
    {
    public:
        static void SetEnabled(bool Enabled, float Radius, int Mode);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class WorldImprisonment
    {
    public:
        static void SetConfig(bool Enabled, float Range, int Mode, int ColorMode, float Width,
                              bool X, bool Y, bool Z, float Height);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class WorldRender
    {
    public:
        static void SetEnabled(bool Enabled, int Mode);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class EffectRender
    {
    public:
        static void SetConfig(bool Enabled, int Mode, int ColorMode);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
        static void OnTouch(float X, float Y);
    };

    class ShowCoordinates
    {
    public:
        static void SetConfig(bool Enabled, int Mode, int Precision, bool Chunk);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class RadarWarning
    {
    public:
        static void SetConfig(bool Enabled, float Range, int Mode, int ColorMode);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class HighlightInfo
    {
    public:
        static void SetConfig(bool Enabled, int Mode, float Scale, int ColorMode, bool Box, bool Info);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class DirectionSign
    {
    public:
        static void SetConfig(bool Enabled, int Mode, float Scale, int ColorMode, float Offset);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class Dropped3D
    {
    public:
        static void SetEnabled(bool Enabled, int Mode);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
    };

    class SetTime
    {
    public:
        static void SetEnabled(bool Enabled, int Time);
        static void ProcessTick(void* LocalPlayer);
    };

    class SetWeather
    {
    public:
        static void SetEnabled(bool Enabled, int Weather);
        static void ProcessTick(void* LocalPlayer);
    };
}
