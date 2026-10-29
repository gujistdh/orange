#include "orange/Modules/Render/RenderModules.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/RenderCore.h"
#include "orange/Core/Main.h"
#include "orange/Core/ArmMemHook.h"
#include "orange/Gui/Glow.h"
#include "orange/Gui/Gui.h"
#include "orange/Modules/Script/ScriptModules.h"

#include <cmath>
#include <chrono>

namespace Orange::Modules::Render
{
    using namespace Orange::Game;

    namespace
    {
        bool gEspEnabled = false;
        float gEspRange = 64.0f;
        float gEspLineWidth = 1.0f;
        int gEspMask = 1;
        int gEspType = 0;
        int gEspColorMode = 0;
        int gEspStyle = 0;
        unsigned int gEspColor = 0xFFFFFFFFu;
        int gEspCount = 32;
        float gHue = 0.0f;

        struct TrackedEntity
        {
            void* Actor = nullptr;
            Vec3 Position{ 0.0f, 0.0f, 0.0f };
            Vec3 Head{ 0.0f, 0.0f, 0.0f };
            Vec3 Feet{ 0.0f, 0.0f, 0.0f };
            float ScreenX = 0.0f;
            float ScreenY = 0.0f;
            float ScreenHeadY = 0.0f;
            float Distance = 0.0f;
            float Health = 0.0f;
            bool Visible = false;
        };

        float gScreenWidth = 1920.0f;
        float gScreenHeight = 1080.0f;

        bool gTraceEnabled = false;
        float gTraceRange = 64.0f;
        float gTraceWidth = 1.0f;
        int gTraceMask = 1;
        int gTraceMode = 0;
        int gTraceColorMode = 0;

        bool gTagEnabled = false;
        int gTagMask = 1;
        float gTagScale = 1.0f;
        int gTagMode = 0;
        float gTagRange = 64.0f;
        bool gTagHealth = true;
        bool gTagName = true;
        bool gTagDistance = true;
        bool gTagItem = false;
        int gTagColorMode = 0;

        bool gHaloEnabled = false;
        float gHaloRadius = 0.6f;
        int gHaloColorMode = 0;

        bool gTrailEnabled = false;
        float gTrailWidth = 1.5f;
        float gTrailDuration = 2.0f;
        int gTrailColorMode = 0;

        struct TrailPoint { Vec3 Position; float Age; };
        std::map<void*, std::vector<TrailPoint>> gTrailPoints;

        bool gFloatingEnabled = false;
        float gFloatingScale = 1.0f;
        float gFloatingDuration = 2.0f;
        int gFloatingMode = 0;
        int gFloatingColorMode = 0;

        struct FloatText { Vec3 Position; std::string Text; float Age; };
        std::vector<FloatText> gFloatTexts;

        struct BoxCorner
        {
            Vec3 World;
            float ScreenX = 0.0f;
            float ScreenY = 0.0f;
            bool Visible = false;
        };

        struct EspBox
        {
            void* Actor = nullptr;
            BoxCorner Corners[8];
            float Health = 0.0f;
            float Distance = 0.0f;
            int VisibleCount = 0;
        };

        std::vector<EspBox> gEspBoxes;
        std::vector<TrackedEntity> gTracked;

        void AdvanceHue()
        {
            gHue += 2.0f;
            if (gHue >= 360.0f)
            {
                gHue -= 360.0f;
            }
        }

        void Collect(float Range, int Mask, float Width, float Height)
        {
            gTracked.clear();
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
                TrackedEntity Entry;
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
                if (!TargetMask::Allow(Mask, false, Entity))
                {
                    continue;
                }
                RenderCore::ActorHead(Entity, Entry.Head);
                RenderCore::ActorFeet(Entity, Entry.Feet);
                Entry.Health = Actor::GetHealth(Entity);
                float HeadX = 0.0f;
                float HeadY = 0.0f;
                float FeetX = 0.0f;
                float FeetY = 0.0f;
                bool HeadOk = RenderCore::WorldToScreen(Entry.Head, HeadX, HeadY, Width, Height);
                bool FeetOk = RenderCore::WorldToScreen(Entry.Feet, FeetX, FeetY, Width, Height);
                if (HeadOk || FeetOk)
                {
                    Entry.Visible = true;
                    Entry.ScreenX = HeadOk ? HeadX : FeetX;
                    Entry.ScreenHeadY = HeadOk ? HeadY : FeetY;
                    Entry.ScreenY = FeetOk ? FeetY : HeadY;
                }
                gTracked.push_back(Entry);
            }
        }

        void BuildBox(void* Actor, EspBox& Out, float Width, float Height)
        {
            Vec3 Min;
            Vec3 Max;
            if (!RenderCore::GetActorBox(Actor, Min, Max))
            {
                Out.VisibleCount = 0;
                return;
            }
            Out.Corners[0].World = { Min.X, Min.Y, Min.Z };
            Out.Corners[1].World = { Max.X, Min.Y, Min.Z };
            Out.Corners[2].World = { Max.X, Min.Y, Max.Z };
            Out.Corners[3].World = { Min.X, Min.Y, Max.Z };
            Out.Corners[4].World = { Min.X, Max.Y, Min.Z };
            Out.Corners[5].World = { Max.X, Max.Y, Min.Z };
            Out.Corners[6].World = { Max.X, Max.Y, Max.Z };
            Out.Corners[7].World = { Min.X, Max.Y, Max.Z };
            Out.VisibleCount = 0;
            for (int Index = 0; Index < 8; ++Index)
            {
                Out.Corners[Index].Visible = RenderCore::WorldToScreen(Out.Corners[Index].World,
                                                                       Out.Corners[Index].ScreenX,
                                                                       Out.Corners[Index].ScreenY,
                                                                       Width, Height);
                if (Out.Corners[Index].Visible)
                {
                    ++Out.VisibleCount;
                }
            }
        }

        const int kBoxEdges[12][2] = {
            { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
            { 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
            { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 },
        };

        const int kBoxFaces[6][4] = {
            { 0, 1, 2, 3 }, { 4, 5, 6, 7 }, { 0, 1, 5, 4 },
            { 1, 2, 6, 5 }, { 2, 3, 7, 6 }, { 3, 0, 4, 7 },
        };
    }

    void Esp::SetEnabled(float Range, float LineWidth, int Count, bool Value,
                         int TargetMaskValue, int Type, int ColorMode, int Style, float Extra)
    {
        gEspEnabled = Value;
        gEspRange = Range;
        gEspLineWidth = LineWidth;
        gEspCount = Count;
        gEspMask = TargetMask::Normalize(TargetMaskValue);
        gEspType = Type;
        gEspColorMode = ColorMode;
        gEspStyle = Style;
        (void)Extra;
    }

    void Esp::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gEspEnabled)
        {
            return;
        }
        Collect(gEspRange, gEspMask, gScreenWidth, gScreenHeight);
        AdvanceHue();
    }

    void Esp::Draw()
    {
        if (!gEspEnabled)
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        if (!Draw)
        {
            return;
        }
        int Limit = gEspCount > 0 ? gEspCount : 32;
        int Drawn = 0;
        for (const TrackedEntity& Entry : gTracked)
        {
            if (!Entry.Visible || Drawn >= Limit)
            {
                continue;
            }
            unsigned int Color = 0;
            if (gEspColorMode == 1)
            {
                Color = RenderCore::Rainbow(static_cast<float>(Drawn) * 90.0f + gHue * 24.0f);
            }
            else if (gEspColorMode == 2)
            {
                Color = RenderCore::ColorFromMode(2, 0xFFFFFFFFu, Entry.Health);
            }
            else
            {
                Color = RenderCore::ColorFromMode(0, gEspColor, 0.0f);
            }

            EspBox Box;
            Box.Actor = Entry.Actor;
            Box.Health = Entry.Health;
            Box.Distance = Entry.Distance;
            BuildBox(Entry.Actor, Box, gScreenWidth, gScreenHeight);
            if (Box.VisibleCount < 2)
            {
                continue;
            }

            if (gEspType == 0)
            {
                for (int Edge = 0; Edge < 12; ++Edge)
                {
                    const BoxCorner& From = Box.Corners[kBoxEdges[Edge][0]];
                    const BoxCorner& To = Box.Corners[kBoxEdges[Edge][1]];
                    if (!From.Visible || !To.Visible)
                    {
                        continue;
                    }
                    Orange::Gui::Glow::Line(Draw, From.ScreenX, From.ScreenY, To.ScreenX, To.ScreenY,
                                            Color, gEspLineWidth, Orange::Gui::UiIntensity());
                }
            }
            else if (gEspType == 1)
            {
                for (int Face = 0; Face < 6; ++Face)
                {
                    const BoxCorner* Points[4];
                    bool AllVisible = true;
                    for (int Corner = 0; Corner < 4; ++Corner)
                    {
                        Points[Corner] = &Box.Corners[kBoxFaces[Face][Corner]];
                        if (!Points[Corner]->Visible)
                        {
                            AllVisible = false;
                            break;
                        }
                    }
                    if (!AllVisible)
                    {
                        continue;
                    }
                    Orange::Gui::Glow::QuadFilled(Draw,
                        Points[0]->ScreenX, Points[0]->ScreenY,
                        Points[1]->ScreenX, Points[1]->ScreenY,
                        Points[2]->ScreenX, Points[2]->ScreenY,
                        Points[3]->ScreenX, Points[3]->ScreenY,
                        Orange::Gui::Glow::Tint(Color, 0.35f), 0.0f);
                }
                for (int Edge = 0; Edge < 12; ++Edge)
                {
                    const BoxCorner& From = Box.Corners[kBoxEdges[Edge][0]];
                    const BoxCorner& To = Box.Corners[kBoxEdges[Edge][1]];
                    if (!From.Visible || !To.Visible)
                    {
                        continue;
                    }
                    Orange::Gui::Glow::Line(Draw, From.ScreenX, From.ScreenY, To.ScreenX, To.ScreenY,
                                            Color, gEspLineWidth, Orange::Gui::UiIntensity());
                }
            }
            else
            {
                float Top = Entry.ScreenHeadY;
                float Bottom = Entry.ScreenY;
                float HeightValue = Bottom - Top;
                if (HeightValue > 1.0f)
                {
                    float BoxWidth = HeightValue * 0.5f;
                    float Left = Entry.ScreenX - BoxWidth * 0.5f;
                    Orange::Gui::Glow::Rect(Draw, Left, Top, BoxWidth, HeightValue, Color, 1.0f);
                }
            }

            if (Entry.Health >= 0.0f && Entry.Health <= 20.0f)
            {
                float Top = Entry.ScreenHeadY;
                float Bottom = Entry.ScreenY;
                float BarHeight = Bottom - Top;
                if (BarHeight > 4.0f)
                {
                    float Ratio = Entry.Health / 20.0f;
                    unsigned int HealthColor = RenderCore::ColorFromMode(2, 0xFFFFFFFFu, Entry.Health);
                    Orange::Gui::Glow::RectFilled(Draw, Entry.ScreenX - 30.0f, Top - 6.0f, 60.0f, 3.0f, 0x60000000u, 0.0f);
                    Orange::Gui::Glow::RectFilled(Draw, Entry.ScreenX - 30.0f, Top - 6.0f, 60.0f * Ratio, 3.0f, HealthColor, 0.0f);
                }
            }
            ++Drawn;
        }
    }

    void EntityTrace::SetEnabled(float Range, float Width, int TargetMaskValue, bool Value, int Mode, int ColorMode)
    {
        gTraceEnabled = Value;
        gTraceRange = Range;
        gTraceWidth = Width;
        gTraceMask = TargetMask::Normalize(TargetMaskValue);
        gTraceMode = Mode;
        gTraceColorMode = ColorMode;
    }

    void EntityTrace::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gTraceEnabled)
        {
            return;
        }
        Collect(gTraceRange, gTraceMask, gScreenWidth, gScreenHeight);
        AdvanceHue();
    }

    void EntityTrace::Draw()
    {
        if (!gTraceEnabled)
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        if (!Draw)
        {
            return;
        }
        float OriginX = 0.0f;
        float OriginY = 0.0f;
        RenderCore::TracerAnchor(gScreenWidth, gScreenHeight, OriginX, OriginY);
        int Index = 0;
        for (const TrackedEntity& Entry : gTracked)
        {
            if (!Entry.Visible)
            {
                continue;
            }
            unsigned int Color = 0;
            if (gTraceColorMode == 1)
            {
                Color = RenderCore::Rainbow(static_cast<float>(Index) * 90.0f + gHue * 24.0f);
            }
            else if (gTraceColorMode == 2)
            {
                Color = RenderCore::ColorFromMode(2, 0xFFFFFFFFu, Entry.Health);
            }
            else
            {
                Color = RenderCore::ColorFromMode(0, 0xFFFFFFFFu, 0.0f);
            }
            Vec3 Anchor;
            if (gTraceMode == 1)
            {
                Anchor = Entry.Feet;
            }
            else
            {
                Anchor = Entry.Head;
            }
            float TargetX = 0.0f;
            float TargetY = 0.0f;
            if (!RenderCore::WorldToScreen(Anchor, TargetX, TargetY, gScreenWidth, gScreenHeight))
            {
                continue;
            }
            Orange::Gui::Glow::Line(Draw, OriginX, OriginY, TargetX, TargetY, Color, gTraceWidth, Orange::Gui::UiIntensity());
            ++Index;
        }
    }

    void TagRender::SetConfig(bool Enabled, int TargetMaskValue, float Scale, int Mode, float Range,
                              bool Health, bool Name, bool Distance, bool Item, int ColorMode)
    {
        gTagEnabled = Enabled;
        gTagMask = TargetMask::Normalize(TargetMaskValue);
        gTagScale = Scale;
        gTagMode = Mode;
        gTagRange = Range;
        gTagHealth = Health;
        gTagName = Name;
        gTagDistance = Distance;
        gTagItem = Item;
        gTagColorMode = ColorMode;
    }

    void TagRender::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gTagEnabled)
        {
            return;
        }
        Collect(gTagRange, gTagMask, gScreenWidth, gScreenHeight);
        AdvanceHue();
    }

    void TagRender::Draw()
    {
        if (!gTagEnabled)
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        if (!Draw)
        {
            return;
        }
        ImFont* Font = ImGui::GetFont();
        int Index = 0;
        for (const TrackedEntity& Entry : gTracked)
        {
            if (!Entry.Visible)
            {
                continue;
            }
            unsigned int Color = 0;
            if (gTagColorMode == 1)
            {
                Color = RenderCore::Rainbow(static_cast<float>(Index) * 90.0f + gHue * 24.0f);
            }
            else if (gTagColorMode == 2)
            {
                Color = RenderCore::ColorFromMode(2, 0xFFFFFFFFu, Entry.Health);
            }
            else
            {
                Color = RenderCore::ColorFromMode(0, 0xFFFFFFFFu, 0.0f);
            }
            float BaseY = Entry.ScreenHeadY - 12.0f * gTagScale;
            if (gTagName)
            {
                std::string NameValue = Script::Chatroom::GetUserName(Entry.Actor);
                if (!NameValue.empty())
                {
                    ImVec2 Size = Font ? Font->CalcTextSizeA(14.0f * gTagScale, 1000.0f, 0.0f, NameValue.c_str()) : ImVec2(0.0f, 0.0f);
                    float TextX = Entry.ScreenX - Size.x * 0.5f;
                    Orange::Gui::Glow::Text(Draw, TextX, BaseY - 16.0f * gTagScale, Color, NameValue.c_str(), 0.8f);
                    BaseY -= 16.0f * gTagScale;
                }
            }
            if (gTagHealth)
            {
                char Buffer[32];
                std::snprintf(Buffer, sizeof(Buffer), "%.1f", Entry.Health);
                ImVec2 Size = Font ? Font->CalcTextSizeA(13.0f * gTagScale, 1000.0f, 0.0f, Buffer) : ImVec2(0.0f, 0.0f);
                unsigned int HealthColor = RenderCore::ColorFromMode(2, 0xFFFFFFFFu, Entry.Health);
                Orange::Gui::Glow::Text(Draw, Entry.ScreenX - Size.x * 0.5f, BaseY - 14.0f * gTagScale, HealthColor, Buffer, 0.6f);
                BaseY -= 14.0f * gTagScale;
            }
            if (gTagDistance)
            {
                char Buffer[32];
                std::snprintf(Buffer, sizeof(Buffer), "%.1fm", Entry.Distance);
                ImVec2 Size = Font ? Font->CalcTextSizeA(12.0f * gTagScale, 1000.0f, 0.0f, Buffer) : ImVec2(0.0f, 0.0f);
                Orange::Gui::Glow::Text(Draw, Entry.ScreenX - Size.x * 0.5f, BaseY - 13.0f * gTagScale, Color, Buffer, 0.6f);
            }
            ++Index;
        }
    }

    void HeadHalo::SetEnabled(bool Enabled, float Radius, int ColorMode)
    {
        gHaloEnabled = Enabled;
        gHaloRadius = Radius;
        gHaloColorMode = ColorMode;
    }

    void HeadHalo::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gHaloEnabled)
        {
            return;
        }
        Collect(64.0f, 1, gScreenWidth, gScreenHeight);
        AdvanceHue();
    }

    void HeadHalo::Draw()
    {
        if (!gHaloEnabled)
        {
            return;
        }
        unsigned int Color = RenderCore::ColorFromMode(gHaloColorMode, 0xFFFFFFFFu, gHue);
        for (const TrackedEntity& Entry : gTracked)
        {
            if (!Entry.Visible)
            {
                continue;
            }
            Glow::Circle(Entry.ScreenX, Entry.ScreenHeadY, gHaloRadius * 40.0f, Color, 2.0f);
        }
    }

    void Trail::SetEnabled(bool Enabled, float Width, float Duration, int ColorMode)
    {
        gTrailEnabled = Enabled;
        gTrailWidth = Width;
        gTrailDuration = Duration;
        gTrailColorMode = ColorMode;
        if (!Enabled)
        {
            gTrailPoints.clear();
        }
    }

    void Trail::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gTrailEnabled)
        {
            return;
        }
        Collect(128.0f, 1, gScreenWidth, gScreenHeight);
        for (auto& Pair : gTrailPoints)
        {
            for (TrailPoint& Point : Pair.second)
            {
                Point.Age += 0.05f;
            }
        }
        for (const TrackedEntity& Entry : gTracked)
        {
            std::vector<TrailPoint>& Points = gTrailPoints[Entry.Actor];
            Points.push_back({ Entry.Position, 0.0f });
            while (!Points.empty() && Points.front().Age > gTrailDuration)
            {
                Points.erase(Points.begin());
            }
        }
    }

    void Trail::Draw()
    {
        if (!gTrailEnabled)
        {
            return;
        }
        unsigned int Color = RenderCore::ColorFromMode(gTrailColorMode, 0xFFFFFFFFu, gHue);
        for (const auto& Pair : gTrailPoints)
        {
            float PreviousX = 0.0f;
            float PreviousY = 0.0f;
            bool HasPrevious = false;
            for (const TrailPoint& Point : Pair.second)
            {
                float ScreenX = 0.0f;
                float ScreenY = 0.0f;
                if (!RenderCore::WorldToScreen(Point.Position, ScreenX, ScreenY, gScreenWidth, gScreenHeight))
                {
                    HasPrevious = false;
                    continue;
                }
                if (HasPrevious)
                {
                    Glow::Line(PreviousX, PreviousY, ScreenX, ScreenY, Color, gTrailWidth);
                }
                PreviousX = ScreenX;
                PreviousY = ScreenY;
                HasPrevious = true;
            }
        }
    }

    void FloatingText::SetEnabled(bool Enabled, float Scale, float Duration, int Mode, int ColorMode)
    {
        gFloatingEnabled = Enabled;
        gFloatingScale = Scale;
        gFloatingDuration = Duration;
        gFloatingMode = Mode;
        gFloatingColorMode = ColorMode;
        if (!Enabled)
        {
            gFloatTexts.clear();
        }
    }

    void FloatingText::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gFloatingEnabled)
        {
            return;
        }
        for (FloatText& Entry : gFloatTexts)
        {
            Entry.Age += 0.05f;
            Entry.Position.Y += 0.02f * gFloatingScale;
        }
        gFloatTexts.erase(std::remove_if(gFloatTexts.begin(), gFloatTexts.end(),
            [](const FloatText& Entry) { return Entry.Age > gFloatingDuration; }), gFloatTexts.end());
    }

    void FloatingText::Draw()
    {
        if (!gFloatingEnabled)
        {
            return;
        }
        unsigned int Color = RenderCore::ColorFromMode(gFloatingColorMode, 0xFFFFFFFFu, gHue);
        for (const FloatText& Entry : gFloatTexts)
        {
            float ScreenX = 0.0f;
            float ScreenY = 0.0f;
            if (!RenderCore::WorldToScreen(Entry.Position, ScreenX, ScreenY, gScreenWidth, gScreenHeight))
            {
                continue;
            }
            ImDrawList* Draw = ImGui::GetBackgroundDrawList();
            if (Draw && !Entry.Text.empty())
            {
                Orange::Gui::Glow::Text(Draw, ScreenX, ScreenY, Color, Entry.Text.c_str(), 0.8f);
            }
            else
            {
                Glow::Rect(ScreenX - 2.0f, ScreenY - 2.0f, 4.0f, 4.0f, Color);
            }
        }
    }

    namespace
    {
        bool gFogEnabled = false;
        int gFogLevel = 0;
        void* gFogHook = nullptr;
        void* gFogOriginal = nullptr;

        bool gFovEnabled = false;
        float gFovValue = 70.0f;
        void* gFovHook = nullptr;
        void* gFovOriginal = nullptr;

        bool gHeldEnabled = false;
        int gHeldMode = 0;
        float gHeldScale = 1.0f;
        int gHeldColorMode = 0;

        bool gHandheldEnabled = false;
        float gHandheldAngle = 0.0f;
        float gHandheldSpeed = 0.0f;

        using FogSetupFn = void (*)(void*, int);
        using FovGetFn = float (*)(void*);

        void FogHook(void* Self, int Level)
        {
            if (gFogEnabled)
            {
                Level = gFogLevel;
            }
            if (gFogOriginal)
            {
                reinterpret_cast<FogSetupFn>(gFogOriginal)(Self, Level);
            }
        }

        float FovHook(void* Self)
        {
            if (gFovEnabled)
            {
                return gFovValue;
            }
            if (gFovOriginal)
            {
                return reinterpret_cast<FovGetFn>(gFovOriginal)(Self);
            }
            return 70.0f;
        }
    }

    void Fog::SetEnabled(bool Enabled, float Distance)
    {
        gFogEnabled = Enabled;
        int Level = static_cast<int>(Distance);
        if (Level < 0 || Level > 6)
        {
            Level = 0;
        }
        gFogLevel = Level;
    }

    void Fog::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gFogEnabled)
        {
            return;
        }
        if (gFogHook || !Orange::Core::BaseAddress)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::LevelRendererPlayer_setupFog);
        if (!Offset)
        {
            return;
        }
        void* Target = reinterpret_cast<void*>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        if (Orange::Core::ArmMemHook::Hook(Target, reinterpret_cast<void*>(FogHook), &gFogOriginal))
        {
            gFogHook = Target;
        }
    }

    void Fov::SetEnabled(bool Enabled, float Value)
    {
        gFovEnabled = Enabled;
        if (Value >= 5.0f && Value <= 130.0f)
        {
            gFovValue = Value;
        }
    }

    void Fov::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gFovEnabled)
        {
            return;
        }
        if (gFovHook || !Orange::Core::BaseAddress)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::CameraAPI_getFov);
        if (!Offset)
        {
            return;
        }
        void* Target = reinterpret_cast<void*>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        if (Orange::Core::ArmMemHook::Hook(Target, reinterpret_cast<void*>(FovHook), &gFovOriginal))
        {
            gFovHook = Target;
        }
    }

    float Fov::GetCurrentFov()
    {
        return gFovValue;
    }

    bool HandheldRotation::IsEnabled()
    {
        return gHandheldEnabled;
    }

    void HandheldRotation::SetEnabled(bool Enabled)
    {
        gHandheldEnabled = Enabled;
    }

    void HandheldRotation::SetSpeed(float Value)
    {
        gHandheldSpeed = Value;
    }

    float HandheldRotation::GetAngleDegrees()
    {
        return gHandheldAngle;
    }

    float HandheldRotation::GetSpeed()
    {
        return gHandheldSpeed;
    }

    void HandheldRotation::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gHandheldEnabled)
        {
            return;
        }
        if (gHandheldSpeed <= 0.0f)
        {
            return;
        }
        gHandheldAngle += gHandheldSpeed * 1.8f;
        if (gHandheldAngle >= 360.0f)
        {
            gHandheldAngle -= 360.0f * std::floor(gHandheldAngle / 360.0f);
        }
    }

    void Helditeminfo::SetConfig(bool Enabled, int ModeValue, float Scale, int ColorMode)
    {
        gHeldEnabled = Enabled;
        gHeldMode = ModeValue;
        gHeldScale = Scale;
        gHeldColorMode = ColorMode;
    }

    void Helditeminfo::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gHeldEnabled)
        {
            return;
        }
    }

    void Helditeminfo::Draw()
    {
        if (!gHeldEnabled)
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        void* Local = Actor::GetLocalPlayerInstance();
        if (!Draw || !Local)
        {
            return;
        }
        void* Carried = Player::GetCarriedItem(Local);
        if (!Carried || ItemStack::IsNull(Carried))
        {
            return;
        }
        int Count = ItemStack::GetCount(Carried);
        int Damage = ItemStack::GetDamageValue(Carried);
        int MaxStack = ItemStack::GetMaxStackSize(Carried);
        unsigned int Color = RenderCore::ColorFromMode(gHeldColorMode, 0xFFFFFFFFu, static_cast<float>(Damage));
        float PanelX = gScreenWidth * 0.5f - 80.0f;
        float PanelY = gScreenHeight - 90.0f;
        Orange::Gui::Glow::RectFilled(Draw, PanelX, PanelY, 160.0f * gHeldScale, 24.0f * gHeldScale, 0x80000000u, 0.0f);
        char Buffer[96];
        std::snprintf(Buffer, sizeof(Buffer), "x%d  %d/%d", Count, Damage, MaxStack);
        Orange::Gui::Glow::Text(Draw, PanelX + 8.0f, PanelY + 4.0f, Color, Buffer, 0.5f);
    }
}
