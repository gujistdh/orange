#include "orange/Modules/Render/RenderModules.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/RenderCore.h"
#include "orange/Gui/Glow.h"
#include "orange/Gui/Gui.h"
#include "orange/Core/Main.h"
#include "orange/Core/ArmMemHook.h"

#include <cmath>

namespace Orange::Modules::Render
{
    using namespace Orange::Game;

    namespace
    {
        float gWidth = 1920.0f;
        float gHeight = 1080.0f;

        struct MiscState
        {
            bool Enabled = false;
            int Mode = 0;
            float Scale = 1.0f;
            int ColorMode = 0;
            float Range = 64.0f;
            float Offset = 0.0f;
            int Precision = 1;
            bool Extra = false;
            float Timer = 0.0f;
        };

        MiscState gCoords;
        MiscState gRadar;
        MiscState gHighlight;
        MiscState gDirection;
        MiscState gEffect;
        MiscState gDropped;
        MiscState gTime;
        MiscState gWeather;

        struct EffectRow
        {
            std::string Name;
            int Duration = 0;
            int Amplifier = 0;
        };

        std::vector<EffectRow> gEffectRows;

        struct TrackedActor
        {
            void* Actor = nullptr;
            Vec3 Position{ 0.0f, 0.0f, 0.0f };
            float ScreenX = 0.0f;
            float ScreenY = 0.0f;
            float Distance = 0.0f;
            float Health = 0.0f;
            bool Visible = false;
        };

        std::vector<TrackedActor> gActors;

        void CollectActors(float Range)
        {
            gActors.clear();
            void* Local = Actor::GetLocalPlayerInstance();
            if (!Local)
            {
                return;
            }
            Vec3 Origin;
            if (!RenderCore::ActorPosition(Local, Origin))
            {
                return;
            }
            std::vector<void*> Entities;
            EntityList::GetEntities(Entities);
            for (void* Entity : Entities)
            {
                if (!Entity || Entity == Local)
                {
                    continue;
                }
                TrackedActor Entry;
                Entry.Actor = Entity;
                if (!RenderCore::ActorPosition(Entity, Entry.Position))
                {
                    continue;
                }
                float DeltaX = Entry.Position.X - Origin.X;
                float DeltaY = Entry.Position.Y - Origin.Y;
                float DeltaZ = Entry.Position.Z - Origin.Z;
                Entry.Distance = std::sqrt(DeltaX * DeltaX + DeltaY * DeltaY + DeltaZ * DeltaZ);
                if (Entry.Distance > Range)
                {
                    continue;
                }
                Entry.Health = Actor::GetHealth(Entity);
                Vec3 Head;
                if (RenderCore::ActorHead(Entity, Head))
                {
                    Entry.Visible = RenderCore::WorldToScreen(Head, Entry.ScreenX, Entry.ScreenY, gWidth, gHeight);
                }
                gActors.push_back(Entry);
            }
        }
    }

    void ShowCoordinates::SetConfig(bool Enabled, int ModeValue, int Precision, bool Chunk)
    {
        gCoords.Enabled = Enabled;
        gCoords.Mode = ModeValue;
        gCoords.Precision = Precision;
        gCoords.Extra = Chunk;
    }

    void ShowCoordinates::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gCoords.Enabled)
        {
            return;
        }
        gCoords.Timer += 0.05f;
    }

    void ShowCoordinates::Draw()
    {
        if (!gCoords.Enabled)
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        void* Local = Actor::GetLocalPlayerInstance();
        if (!Draw || !Local)
        {
            return;
        }
        Vec3 Position;
        if (!RenderCore::ActorPosition(Local, Position))
        {
            return;
        }
        int Digits = gCoords.Precision < 0 ? 0 : (gCoords.Precision > 4 ? 4 : gCoords.Precision);
        char Buffer[128];
        std::snprintf(Buffer, sizeof(Buffer), "X %.*f  Y %.*f  Z %.*f", Digits, Position.X, Digits, Position.Y, Digits, Position.Z);
        Orange::Gui::Glow::Text(Draw, 24.0f, 24.0f, 0xFFFFFFFFu, Buffer, 0.6f);
        if (gCoords.Extra)
        {
            char ChunkBuffer[96];
            std::snprintf(ChunkBuffer, sizeof(ChunkBuffer), "Chunk %d %d",
                          static_cast<int>(std::floor(Position.X)) >> 4,
                          static_cast<int>(std::floor(Position.Z)) >> 4);
            Orange::Gui::Glow::Text(Draw, 24.0f, 44.0f, 0xFFFFFFFFu, ChunkBuffer, 0.6f);
        }
    }

    void RadarWarning::SetConfig(bool Enabled, float Range, int ModeValue, int ColorMode)
    {
        gRadar.Enabled = Enabled;
        gRadar.Range = Range;
        gRadar.Mode = ModeValue;
        gRadar.ColorMode = ColorMode;
    }

    void RadarWarning::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (gRadar.Enabled)
        {
            CollectActors(gRadar.Range);
        }
    }

    void RadarWarning::Draw()
    {
        if (!gRadar.Enabled)
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        void* Local = Actor::GetLocalPlayerInstance();
        if (!Draw || !Local)
        {
            return;
        }
        Vec3 Origin;
        if (!RenderCore::ActorPosition(Local, Origin))
        {
            return;
        }
        float CenterX = gWidth * 0.5f;
        float CenterY = gHeight - 120.0f;
        float Radius = 80.0f;
        Orange::Gui::Glow::Circle(Draw, CenterX, CenterY, Radius, 0x60FFFFFFu, 1.0f, 0.0f);
        for (const TrackedActor& Entry : gActors)
        {
            float Ratio = Entry.Distance / (gRadar.Range > 0.0f ? gRadar.Range : 1.0f);
            if (Ratio > 1.0f)
            {
                continue;
            }
            float DeltaX = Entry.Position.X - Origin.X;
            float DeltaZ = Entry.Position.Z - Origin.Z;
            float Angle = std::atan2(DeltaZ, DeltaX);
            float PointX = CenterX + std::cos(Angle) * Radius * Ratio;
            float PointY = CenterY + std::sin(Angle) * Radius * Ratio;
            unsigned int Color = RenderCore::ColorFromMode(gRadar.ColorMode, 0xFFFFFFFFu, Entry.Health);
            Orange::Gui::Glow::CircleFilled(Draw, PointX, PointY, 3.0f, Color, 1.0f);
        }
    }

    void HighlightInfo::SetConfig(bool Enabled, int ModeValue, float Scale, int ColorMode, bool Box, bool Info)
    {
        gHighlight.Enabled = Enabled;
        gHighlight.Mode = ModeValue;
        gHighlight.Scale = Scale;
        gHighlight.ColorMode = ColorMode;
        gHighlight.Extra = Box;
        gHighlight.Precision = Info ? 1 : 0;
    }

    void HighlightInfo::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (gHighlight.Enabled)
        {
            CollectActors(32.0f);
        }
    }

    void HighlightInfo::Draw()
    {
        if (!gHighlight.Enabled)
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        if (!Draw)
        {
            return;
        }
        for (const TrackedActor& Entry : gActors)
        {
            if (!Entry.Visible)
            {
                continue;
            }
            unsigned int Color = RenderCore::ColorFromMode(gHighlight.ColorMode, 0xFFFFFFFFu, Entry.Health);
            if (gHighlight.Extra)
            {
                Vec3 Min;
                Vec3 Max;
                if (RenderCore::GetActorBox(Entry.Actor, Min, Max))
                {
                    Vec3 Corners[8];
                    Corners[0] = { Min.X, Min.Y, Min.Z };
                    Corners[1] = { Max.X, Min.Y, Min.Z };
                    Corners[2] = { Max.X, Min.Y, Max.Z };
                    Corners[3] = { Min.X, Min.Y, Max.Z };
                    Corners[4] = { Min.X, Max.Y, Min.Z };
                    Corners[5] = { Max.X, Max.Y, Min.Z };
                    Corners[6] = { Max.X, Max.Y, Max.Z };
                    Corners[7] = { Min.X, Max.Y, Max.Z };
                    for (int Edge = 0; Edge < 12; ++Edge)
                    {
                        static const int kEdges[12][2] = {
                            { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
                            { 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
                            { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 },
                        };
                        float FromX = 0.0f;
                        float FromY = 0.0f;
                        float ToX = 0.0f;
                        float ToY = 0.0f;
                        if (!RenderCore::WorldToScreen(Corners[kEdges[Edge][0]], FromX, FromY, gWidth, gHeight))
                        {
                            continue;
                        }
                        if (!RenderCore::WorldToScreen(Corners[kEdges[Edge][1]], ToX, ToY, gWidth, gHeight))
                        {
                            continue;
                        }
                        Orange::Gui::Glow::Line(Draw, FromX, FromY, ToX, ToY, Color, 1.5f * gHighlight.Scale, 0.8f);
                    }
                }
            }
            if (gHighlight.Precision != 0)
            {
                char Buffer[64];
                std::snprintf(Buffer, sizeof(Buffer), "%.1f HP", Entry.Health);
                Orange::Gui::Glow::Text(Draw, Entry.ScreenX - 20.0f, Entry.ScreenY - 30.0f * gHighlight.Scale,
                                        Color, Buffer, 0.6f);
            }
        }
    }

    void DirectionSign::SetConfig(bool Enabled, int ModeValue, float Scale, int ColorMode, float Offset)
    {
        gDirection.Enabled = Enabled;
        gDirection.Mode = ModeValue;
        gDirection.Scale = Scale;
        gDirection.ColorMode = ColorMode;
        gDirection.Offset = Offset;
    }

    void DirectionSign::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (gDirection.Enabled)
        {
            CollectActors(96.0f);
        }
    }

    void DirectionSign::Draw()
    {
        if (!gDirection.Enabled)
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        void* Local = Actor::GetLocalPlayerInstance();
        if (!Draw || !Local)
        {
            return;
        }
        Vec3 Origin;
        if (!RenderCore::ActorPosition(Local, Origin))
        {
            return;
        }
        for (const TrackedActor& Entry : gActors)
        {
            unsigned int Color = RenderCore::ColorFromMode(gDirection.ColorMode, 0xFFFFFFFFu, Entry.Distance);
            float DeltaX = Entry.Position.X - Origin.X;
            float DeltaZ = Entry.Position.Z - Origin.Z;
            float Angle = std::atan2(DeltaZ, DeltaX);
            Vec3 Arrow;
            Arrow.X = Origin.X + std::cos(Angle) * (2.0f + gDirection.Offset);
            Arrow.Y = Origin.Y + 1.5f;
            Arrow.Z = Origin.Z + std::sin(Angle) * (2.0f + gDirection.Offset);
            float ScreenX = 0.0f;
            float ScreenY = 0.0f;
            if (!RenderCore::WorldToScreen(Arrow, ScreenX, ScreenY, gWidth, gHeight))
            {
                continue;
            }
            Orange::Gui::Glow::CircleFilled(Draw, ScreenX, ScreenY, 4.0f * gDirection.Scale, Color, 1.0f);
        }
    }

    void EffectRender::SetConfig(bool Enabled, int ModeValue, int ColorMode)
    {
        gEffect.Enabled = Enabled;
        gEffect.Mode = ModeValue;
        gEffect.ColorMode = ColorMode;
    }

    void EffectRender::ProcessTick(void* LocalPlayer)
    {
        gEffectRows.clear();
        (void)LocalPlayer;
        if (!gEffect.Enabled)
        {
            return;
        }
    }

    void EffectRender::Draw()
    {
        if (!gEffect.Enabled || gEffectRows.empty())
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        if (!Draw)
        {
            return;
        }
        float PanelX = 20.0f;
        float PanelY = 200.0f;
        float PanelWidth = 220.0f;
        float RowHeight = 22.0f;
        float PanelHeight = RowHeight * static_cast<float>(gEffectRows.size()) + 16.0f;
        Orange::Gui::Glow::RectFilled(Draw, PanelX, PanelY, PanelWidth, PanelHeight, 0x80000000u, 0.0f);
        Orange::Gui::Glow::Rect(Draw, PanelX, PanelY, PanelWidth, PanelHeight, 0x40FFFFFFu, 1.0f);
        float RowY = PanelY + 8.0f;
        for (const EffectRow& Row : gEffectRows)
        {
            unsigned int Color = RenderCore::ColorFromMode(gEffect.ColorMode, 0xFFFFFFFFu,
                                                           static_cast<float>(Row.Duration));
            char Buffer[96];
            std::snprintf(Buffer, sizeof(Buffer), "%s %d (%ds)", Row.Name.c_str(), Row.Amplifier + 1, Row.Duration / 20);
            Orange::Gui::Glow::Text(Draw, PanelX + 10.0f, RowY, Color, Buffer, 0.5f);
            RowY += RowHeight;
        }
    }

    void EffectRender::OnTouch(float X, float Y)
    {
        (void)X;
        (void)Y;
        if (!gEffect.Enabled)
        {
            return;
        }
    }

    namespace
    {
        void* gDropHookRender = nullptr;
        void* gDropHookTranslate = nullptr;
        void* gDropHookRotate = nullptr;
        void* gDropOrigRender = nullptr;
        void* gDropOrigTranslate = nullptr;
        void* gDropOrigRotate = nullptr;
        int gDropCounter = 0;
        bool gDropInstalled = false;

        using TranslateFn = void (*)(void*, float, float, float);
        using RotateFn = void (*)(void*, float);

        void DropTranslateHook(void* Self, float X, float Y, float Z)
        {
            if (gDropped.Enabled)
            {
                Y += 0.25f;
            }
            if (gDropOrigTranslate)
            {
                reinterpret_cast<TranslateFn>(gDropOrigTranslate)(Self, X, Y, Z);
            }
        }

        void DropRotateHook(void* Self, float Angle)
        {
            if (gDropped.Enabled)
            {
                Angle += static_cast<float>(gDropCounter) * 0.5f;
            }
            if (gDropOrigRotate)
            {
                reinterpret_cast<RotateFn>(gDropOrigRotate)(Self, Angle);
            }
        }

        void InstallDropHooks()
        {
            if (gDropInstalled || !Orange::Core::BaseAddress)
            {
                return;
            }
            intptr_t RenderOffset = Orange::Offsets::Load(Orange::Offsets::Slot::ItemRenderer_render);
            intptr_t TranslateOffset = Orange::Offsets::Load(Orange::Offsets::Slot::DroppedItem_applyTranslate);
            intptr_t RotateOffset = Orange::Offsets::Load(Orange::Offsets::Slot::DroppedItem_applyRotateY);
            if (!TranslateOffset || !RotateOffset)
            {
                return;
            }
            void* TranslateTarget = reinterpret_cast<void*>(Orange::Core::BaseAddress + static_cast<Address>(TranslateOffset));
            void* RotateTarget = reinterpret_cast<void*>(Orange::Core::BaseAddress + static_cast<Address>(RotateOffset));
            if (Orange::Core::ArmMemHook::Hook(TranslateTarget, reinterpret_cast<void*>(DropTranslateHook), &gDropOrigTranslate))
            {
                gDropHookTranslate = TranslateTarget;
            }
            if (Orange::Core::ArmMemHook::Hook(RotateTarget, reinterpret_cast<void*>(DropRotateHook), &gDropOrigRotate))
            {
                gDropHookRotate = RotateTarget;
            }
            if (RenderOffset)
            {
                gDropHookRender = reinterpret_cast<void*>(Orange::Core::BaseAddress + static_cast<Address>(RenderOffset));
                gDropOrigRender = gDropHookRender;
            }
            gDropInstalled = true;
        }
    }

    void Dropped3D::SetEnabled(bool Enabled, int ModeValue)
    {
        gDropped.Enabled = Enabled;
        gDropped.Mode = ModeValue;
        if (!Enabled)
        {
            gDropCounter = 0;
        }
    }

    void Dropped3D::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gDropped.Enabled)
        {
            return;
        }
        InstallDropHooks();
        ++gDropCounter;
        if (gDropCounter > 360)
        {
            gDropCounter = 0;
        }
    }

    void Dropped3D::Draw()
    {
        if (!gDropped.Enabled)
        {
            return;
        }
        gDropped.Timer += 0.05f;
    }

    void SetTime::SetEnabled(bool Enabled, int TimeValue)
    {
        gTime.Enabled = Enabled;
        gTime.Mode = TimeValue < 0 ? 0 : (TimeValue > 24000 ? 24000 : TimeValue);
    }

    void SetTime::ProcessTick(void* LocalPlayer)
    {
        if (!gTime.Enabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        intptr_t LevelOffset = Orange::Offsets::Load(Orange::Offsets::Slot::Actor_getLevel);
        intptr_t WeatherVtable = Orange::Offsets::Load(Orange::Offsets::Slot::Level_WeatherVtableOffset);
        intptr_t SetTimeOffset = Orange::Offsets::Load(Orange::Offsets::Slot::LevelData_setTime);
        if (!LevelOffset || !WeatherVtable || !SetTimeOffset)
        {
            return;
        }
        using GetLevelFn = void* (*)(void*);
        GetLevelFn GetLevel = reinterpret_cast<GetLevelFn>(Orange::Core::BaseAddress + static_cast<Address>(LevelOffset));
        void* Level = GetLevel(LocalPlayer);
        if (!Level)
        {
            return;
        }
        void* LevelData = *reinterpret_cast<void**>(reinterpret_cast<Byte*>(Level) + WeatherVtable);
        if (!LevelData)
        {
            return;
        }
        using SetTimeFn = void (*)(void*, int);
        SetTimeFn Apply = reinterpret_cast<SetTimeFn>(Orange::Core::BaseAddress + static_cast<Address>(SetTimeOffset));
        Apply(LevelData, gTime.Mode);
    }

    void SetWeather::SetEnabled(bool Enabled, int WeatherValue)
    {
        gWeather.Enabled = Enabled;
        gWeather.Mode = WeatherValue < 0 ? 0 : (WeatherValue > 2 ? 2 : WeatherValue);
    }

    void SetWeather::ProcessTick(void* LocalPlayer)
    {
        if (!gWeather.Enabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        intptr_t LevelOffset = Orange::Offsets::Load(Orange::Offsets::Slot::Actor_getLevel);
        intptr_t WeatherVtable = Orange::Offsets::Load(Orange::Offsets::Slot::Level_WeatherVtableOffset);
        intptr_t RainOffset = Orange::Offsets::Load(Orange::Offsets::Slot::Weather_setRainLevel);
        intptr_t LightningOffset = Orange::Offsets::Load(Orange::Offsets::Slot::Weather_setLightningLevel);
        if (!LevelOffset || !WeatherVtable || !RainOffset || !LightningOffset)
        {
            return;
        }
        using GetLevelFn = void* (*)(void*);
        GetLevelFn GetLevel = reinterpret_cast<GetLevelFn>(Orange::Core::BaseAddress + static_cast<Address>(LevelOffset));
        void* Level = GetLevel(LocalPlayer);
        if (!Level)
        {
            return;
        }
        void* Weather = *reinterpret_cast<void**>(reinterpret_cast<Byte*>(Level) + WeatherVtable);
        if (!Weather)
        {
            return;
        }
        float Rain = 0.0f;
        float Lightning = 0.0f;
        if (gWeather.Mode == 1)
        {
            Rain = 1.0f;
        }
        else if (gWeather.Mode == 2)
        {
            Rain = 1.0f;
            Lightning = 1.0f;
        }
        using SetLevelFn = void (*)(void*, float);
        SetLevelFn SetRain = reinterpret_cast<SetLevelFn>(Orange::Core::BaseAddress + static_cast<Address>(RainOffset));
        SetLevelFn SetLightning = reinterpret_cast<SetLevelFn>(Orange::Core::BaseAddress + static_cast<Address>(LightningOffset));
        SetRain(Weather, Rain);
        SetLightning(Weather, Lightning);
    }
}
