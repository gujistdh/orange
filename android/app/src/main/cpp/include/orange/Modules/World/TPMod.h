#pragma once

#include "orange/Common.h"

namespace Orange::Modules::World
{
    struct TPLocation
    {
        std::string Name;
        float X = 0.0f;
        float Y = 0.0f;
        float Z = 0.0f;
    };

    class TPMod
    {
    public:
        static void SetConfig(bool Enabled, int Mode, bool Flag, float Value);
        static bool IsEnabled();
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
        static void OnTouch(float X, float Y);

        static std::vector<TPLocation>& GetLocations();
        static void ClearLocations();
        static void AddLocationFromCurrentTick(void* Player);
        static bool CaptureNowForCurrentPos(void* Player);
        static void TeleportTo(const TPLocation& Location);
        static void TeleportToIndex(int Index);

        static void OpenEditField(int Index, int Field);
        static void ApplyEditInput(int Index, int Field, const std::string& Text);

        static float GetMenuPropFloat(int Index);
        static void SetMenuPropFloat(int Index, float Value);
        static int GetMenuPropInt(int Index);
        static void SetMenuPropInt(int Index, int Value);

    private:
        static int SelectedIndex();
    };
}
