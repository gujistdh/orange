#pragma once

#include "orange/Common.h"

namespace Orange::Modules::Editors
{
    class NbtEditor
    {
    public:
        static void SetEnabled(bool Value);
        static bool IsEnabled();
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
        static void OnTouch(float X, float Y);
        static void OpenValueInput(const std::string& Path);
        static void OnTextInputResult(const std::string& Text);
        static void LoadFromPath(const std::string& Path);
        static void SaveToPath(const std::string& Path);
        static int SelectedIndex();
        static void SetSelectedIndex(int Value);
    };

    class ItemSetEditor
    {
    public:
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
        static void OnTouch(float X, float Y);
    };

    class EnchantEditor
    {
    public:
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
        static void OnTouch(float X, float Y);
    };

    class ParticleEditor
    {
    public:
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
        static void OnTouch(float X, float Y);
        static int SelectedCount();
    };

    class SoundEditor
    {
    public:
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
        static void OnTouch(float X, float Y);
    };

    class EffectEditor
    {
    public:
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
        static void OnTouch(float X, float Y);
    };

    class ViewUserEditor
    {
    public:
        static void SetEnabled(bool Value);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
        static void OnTouch(float X, float Y);
    };

    class ViewCommandBlockEditor
    {
    public:
        static void SetConfig(bool Enabled, int Mode, int Style, bool Flag1, bool Flag2,
                              bool Flag3, bool Flag4, int Count);
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
        static void OnTouch(float X, float Y);
    };
}
