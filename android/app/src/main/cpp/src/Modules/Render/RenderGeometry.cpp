#include "orange/Modules/Render/RenderModules.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/RenderCore.h"
#include "orange/Gui/Glow.h"
#include "orange/Gui/Gui.h"
#include "orange/Core/Main.h"

#include <cmath>

namespace Orange::Modules::Render
{
    using namespace Orange::Game;

    namespace
    {
        float gScreenWidth = 1920.0f;
        float gScreenHeight = 1080.0f;

        struct GeoState
        {
            bool Enabled = false;
            int Mode = 0;
            float Width = 1.0f;
            float Range = 8.0f;
            int ColorMode = 0;
            unsigned int Color = 0xFFFFFFFFu;
            float Radius = 2.0f;
            float Height = 3.0f;
            int Mask = 1;
            bool AxisX = true;
            bool AxisY = true;
            bool AxisZ = true;
            float Spin = 0.0f;
        };

        GeoState gBlockOutline;
        GeoState gPlaceRender;
        GeoState gLandingAura;
        GeoState gSelfAura;
        GeoState gWorldImprisonment;
        GeoState gBlockRender;
        GeoState gWorldRender;

        const int kBoxEdges[12][2] = {
            { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
            { 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
            { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 },
        };

        const int kBoxFaces[6][4] = {
            { 0, 1, 2, 3 }, { 4, 5, 6, 7 }, { 0, 1, 5, 4 },
            { 1, 2, 6, 5 }, { 2, 3, 7, 6 }, { 3, 0, 4, 7 },
        };

        void DrawBoxEdges(ImDrawList* Draw, const Vec3* Corners, unsigned int Color, float Width)
        {
            float ScreenX[8];
            float ScreenY[8];
            bool Visible[8];
            for (int Index = 0; Index < 8; ++Index)
            {
                Visible[Index] = RenderCore::WorldToScreen(Corners[Index], ScreenX[Index], ScreenY[Index],
                                                           gScreenWidth, gScreenHeight);
            }
            for (int Edge = 0; Edge < 12; ++Edge)
            {
                int From = kBoxEdges[Edge][0];
                int To = kBoxEdges[Edge][1];
                if (!Visible[From] || !Visible[To])
                {
                    continue;
                }
                Orange::Gui::Glow::Line(Draw, ScreenX[From], ScreenY[From], ScreenX[To], ScreenY[To],
                                        Color, Width, Orange::Gui::UiIntensity());
            }
        }

        void FillBoxFaces(ImDrawList* Draw, const Vec3* Corners, unsigned int Color, float Width)
        {
            float ScreenX[8];
            float ScreenY[8];
            bool Visible[8];
            for (int Index = 0; Index < 8; ++Index)
            {
                Visible[Index] = RenderCore::WorldToScreen(Corners[Index], ScreenX[Index], ScreenY[Index],
                                                           gScreenWidth, gScreenHeight);
            }
            for (int Face = 0; Face < 6; ++Face)
            {
                bool AllVisible = true;
                for (int Corner = 0; Corner < 4; ++Corner)
                {
                    if (!Visible[kBoxFaces[Face][Corner]])
                    {
                        AllVisible = false;
                        break;
                    }
                }
                if (!AllVisible)
                {
                    continue;
                }
                int A = kBoxFaces[Face][0];
                int B = kBoxFaces[Face][1];
                int C = kBoxFaces[Face][2];
                int D = kBoxFaces[Face][3];
                Orange::Gui::Glow::QuadFilled(Draw, ScreenX[A], ScreenY[A], ScreenX[B], ScreenY[B],
                                              ScreenX[C], ScreenY[C], ScreenX[D], ScreenY[D],
                                              Orange::Gui::Glow::Tint(Color, 0.3f), 0.0f);
            }
            for (int Edge = 0; Edge < 12; ++Edge)
            {
                int From = kBoxEdges[Edge][0];
                int To = kBoxEdges[Edge][1];
                if (!Visible[From] || !Visible[To])
                {
                    continue;
                }
                Orange::Gui::Glow::Line(Draw, ScreenX[From], ScreenY[From], ScreenX[To], ScreenY[To],
                                        Color, Width, Orange::Gui::UiIntensity());
            }
        }

        void BuildUnitBox(const Vec3& Origin, float Size, Vec3* Out)
        {
            float Half = Size * 0.5f;
            Out[0] = { Origin.X - Half, Origin.Y, Origin.Z - Half };
            Out[1] = { Origin.X + Half, Origin.Y, Origin.Z - Half };
            Out[2] = { Origin.X + Half, Origin.Y, Origin.Z + Half };
            Out[3] = { Origin.X - Half, Origin.Y, Origin.Z + Half };
            Out[4] = { Origin.X - Half, Origin.Y + Size, Origin.Z - Half };
            Out[5] = { Origin.X + Half, Origin.Y + Size, Origin.Z - Half };
            Out[6] = { Origin.X + Half, Origin.Y + Size, Origin.Z + Half };
            Out[7] = { Origin.X - Half, Origin.Y + Size, Origin.Z + Half };
        }

        void DrawRing(ImDrawList* Draw, const Vec3& Center, float Radius, int Segments,
                      unsigned int Color, float Width, float Rotation)
        {
            float PreviousX = 0.0f;
            float PreviousY = 0.0f;
            bool HasPrevious = false;
            for (int Index = 0; Index <= Segments; ++Index)
            {
                float Angle = Rotation + static_cast<float>(Index) * (6.2831853f / static_cast<float>(Segments));
                Vec3 Point;
                Point.X = Center.X + std::cos(Angle) * Radius;
                Point.Y = Center.Y;
                Point.Z = Center.Z + std::sin(Angle) * Radius;
                float ScreenX = 0.0f;
                float ScreenY = 0.0f;
                if (!RenderCore::WorldToScreen(Point, ScreenX, ScreenY, gScreenWidth, gScreenHeight))
                {
                    HasPrevious = false;
                    continue;
                }
                if (HasPrevious)
                {
                    Orange::Gui::Glow::Line(Draw, PreviousX, PreviousY, ScreenX, ScreenY, Color, Width, Orange::Gui::UiIntensity());
                }
                PreviousX = ScreenX;
                PreviousY = ScreenY;
                HasPrevious = true;
            }
        }
    }

    void BlockOutline::SetConfig(bool Enabled, int ModeValue, float Width)
    {
        gBlockOutline.Enabled = Enabled;
        gBlockOutline.Mode = ModeValue;
        gBlockOutline.Width = Width;
    }

    void BlockOutline::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (gBlockOutline.Enabled)
        {
            gBlockOutline.Spin += 1.0f;
        }
    }

    void BlockOutline::Draw()
    {
        if (!gBlockOutline.Enabled)
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
        Vec3 Corners[8];
        BuildUnitBox(Position, 1.0f, Corners);
        unsigned int Color = RenderCore::ColorFromMode(gBlockOutline.ColorMode, gBlockOutline.Color, gBlockOutline.Spin);
        DrawBoxEdges(Draw, Corners, Color, gBlockOutline.Width);
    }

    void PlaceRender::SetConfig(bool Enabled, float Range, int ModeValue, int ColorMode, float Width)
    {
        gPlaceRender.Enabled = Enabled;
        gPlaceRender.Range = Range;
        gPlaceRender.Mode = ModeValue;
        gPlaceRender.ColorMode = ColorMode;
        gPlaceRender.Width = Width;
    }

    void PlaceRender::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gPlaceRender.Enabled)
        {
            return;
        }
        gPlaceRender.Spin += 2.0f;
        if (gPlaceRender.Spin >= 360.0f)
        {
            gPlaceRender.Spin -= 360.0f;
        }
    }

    void PlaceRender::Draw()
    {
        if (!gPlaceRender.Enabled)
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
        Vec3 Target;
        Target.X = Position.X;
        Target.Y = Position.Y - 1.0f;
        Target.Z = Position.Z - gPlaceRender.Range;
        Vec3 Corners[8];
        BuildUnitBox(Target, 1.0f, Corners);
        unsigned int Color = RenderCore::ColorFromMode(gPlaceRender.ColorMode, gPlaceRender.Color, gPlaceRender.Spin);
        FillBoxFaces(Draw, Corners, Color, gPlaceRender.Width);
    }

    void LandingAura::SetEnabled(bool Enabled, float Radius, int ModeValue, int ColorMode, float Width)
    {
        gLandingAura.Enabled = Enabled;
        gLandingAura.Radius = Radius;
        gLandingAura.Mode = ModeValue;
        gLandingAura.ColorMode = ColorMode;
        gLandingAura.Width = Width;
    }

    void LandingAura::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gLandingAura.Enabled)
        {
            return;
        }
        gLandingAura.Spin += 3.0f;
        if (gLandingAura.Spin >= 360.0f)
        {
            gLandingAura.Spin -= 360.0f;
        }
    }

    void LandingAura::Draw()
    {
        if (!gLandingAura.Enabled)
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        void* Local = Actor::GetLocalPlayerInstance();
        if (!Draw || !Local)
        {
            return;
        }
        Vec3 Feet;
        if (!RenderCore::ActorFeet(Local, Feet))
        {
            return;
        }
        unsigned int Color = RenderCore::ColorFromMode(gLandingAura.ColorMode, gLandingAura.Color, gLandingAura.Spin);
        DrawRing(Draw, Feet, gLandingAura.Radius, 48, Color, gLandingAura.Width, gLandingAura.Spin * 0.017453f);
        DrawRing(Draw, Feet, gLandingAura.Radius * 0.8f, 36, Orange::Gui::Glow::Tint(Color, 0.7f),
                 gLandingAura.Width * 0.7f, -gLandingAura.Spin * 0.017453f);
    }

    void SelfAura::SetEnabled(bool Enabled, float Radius, int ModeValue)
    {
        gSelfAura.Enabled = Enabled;
        gSelfAura.Radius = Radius;
        gSelfAura.Mode = ModeValue;
    }

    void SelfAura::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gSelfAura.Enabled)
        {
            return;
        }
        gSelfAura.Spin += 4.0f;
        if (gSelfAura.Spin >= 360.0f)
        {
            gSelfAura.Spin -= 360.0f;
        }
    }

    void SelfAura::Draw()
    {
        if (!gSelfAura.Enabled)
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        void* Local = Actor::GetLocalPlayerInstance();
        if (!Draw || !Local)
        {
            return;
        }
        Vec3 Feet;
        if (!RenderCore::ActorFeet(Local, Feet))
        {
            return;
        }
        for (int Ring = 0; Ring < 3; ++Ring)
        {
            float Radius = gSelfAura.Radius * (0.6f + static_cast<float>(Ring) * 0.3f);
            float Rotation = gSelfAura.Spin * 0.017453f * (Ring % 2 == 0 ? 1.0f : -1.0f);
            unsigned int Color = RenderCore::Rainbow(gSelfAura.Spin + static_cast<float>(Ring) * 120.0f);
            DrawRing(Draw, Feet, Radius, 32, Color, 1.5f, Rotation);
        }
    }

    void WorldImprisonment::SetConfig(bool Enabled, float Range, int ModeValue, int ColorMode, float Width,
                                      bool X, bool Y, bool Z, float Height)
    {
        gWorldImprisonment.Enabled = Enabled;
        gWorldImprisonment.Range = Range;
        gWorldImprisonment.Mode = ModeValue;
        gWorldImprisonment.ColorMode = ColorMode;
        gWorldImprisonment.Width = Width;
        gWorldImprisonment.AxisX = X;
        gWorldImprisonment.AxisY = Y;
        gWorldImprisonment.AxisZ = Z;
        gWorldImprisonment.Height = Height;
    }

    void WorldImprisonment::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gWorldImprisonment.Enabled)
        {
            return;
        }
        gWorldImprisonment.Spin += 1.5f;
        if (gWorldImprisonment.Spin >= 360.0f)
        {
            gWorldImprisonment.Spin -= 360.0f;
        }
    }

    void WorldImprisonment::Draw()
    {
        if (!gWorldImprisonment.Enabled)
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        void* Local = Actor::GetLocalPlayerInstance();
        if (!Draw || !Local)
        {
            return;
        }
        Vec3 Feet;
        if (!RenderCore::ActorFeet(Local, Feet))
        {
            return;
        }
        unsigned int Color = RenderCore::ColorFromMode(gWorldImprisonment.ColorMode, gWorldImprisonment.Color,
                                                       gWorldImprisonment.Spin);
        float Size = gWorldImprisonment.Range;
        Vec3 Corners[8];
        float BaseX = Feet.X - Size * 0.5f;
        float BaseY = Feet.Y;
        float BaseZ = Feet.Z - Size * 0.5f;
        Corners[0] = { BaseX, BaseY, BaseZ };
        Corners[1] = { BaseX + Size, BaseY, BaseZ };
        Corners[2] = { BaseX + Size, BaseY, BaseZ + Size };
        Corners[3] = { BaseX, BaseY, BaseZ + Size };
        Corners[4] = { BaseX, BaseY + gWorldImprisonment.Height, BaseZ };
        Corners[5] = { BaseX + Size, BaseY + gWorldImprisonment.Height, BaseZ };
        Corners[6] = { BaseX + Size, BaseY + gWorldImprisonment.Height, BaseZ + Size };
        Corners[7] = { BaseX, BaseY + gWorldImprisonment.Height, BaseZ + Size };
        DrawBoxEdges(Draw, Corners, Color, gWorldImprisonment.Width);
        float CenterX = Feet.X;
        float CenterY = Feet.Y;
        float CenterZ = Feet.Z;
        float ScreenX = 0.0f;
        float ScreenY = 0.0f;
        if (gWorldImprisonment.AxisX)
        {
            Vec3 From = { CenterX - Size, CenterY + gWorldImprisonment.Height * 0.5f, CenterZ };
            Vec3 To = { CenterX + Size, CenterY + gWorldImprisonment.Height * 0.5f, CenterZ };
            float FromX = 0.0f;
            float FromY = 0.0f;
            float ToX = 0.0f;
            float ToY = 0.0f;
            if (RenderCore::WorldToScreen(From, FromX, FromY, gScreenWidth, gScreenHeight) &&
                RenderCore::WorldToScreen(To, ToX, ToY, gScreenWidth, gScreenHeight))
            {
                Orange::Gui::Glow::Line(Draw, FromX, FromY, ToX, ToY,
                                        Orange::Gui::Glow::Tint(Color, Orange::Gui::UiIntensity()), 1.0f, 0.5f);
            }
        }
        if (gWorldImprisonment.AxisY)
        {
            Vec3 From = { CenterX, CenterY - 1.0f, CenterZ };
            Vec3 To = { CenterX, CenterY + gWorldImprisonment.Height + 1.0f, CenterZ };
            float FromX = 0.0f;
            float FromY = 0.0f;
            float ToX = 0.0f;
            float ToY = 0.0f;
            if (RenderCore::WorldToScreen(From, FromX, FromY, gScreenWidth, gScreenHeight) &&
                RenderCore::WorldToScreen(To, ToX, ToY, gScreenWidth, gScreenHeight))
            {
                Orange::Gui::Glow::Line(Draw, FromX, FromY, ToX, ToY,
                                        Orange::Gui::Glow::Tint(Color, Orange::Gui::UiIntensity()), 1.0f, 0.5f);
            }
        }
        if (gWorldImprisonment.AxisZ)
        {
            Vec3 From = { CenterX, CenterY + gWorldImprisonment.Height * 0.5f, CenterZ - Size };
            Vec3 To = { CenterX, CenterY + gWorldImprisonment.Height * 0.5f, CenterZ + Size };
            float FromX = 0.0f;
            float FromY = 0.0f;
            float ToX = 0.0f;
            float ToY = 0.0f;
            if (RenderCore::WorldToScreen(From, FromX, FromY, gScreenWidth, gScreenHeight) &&
                RenderCore::WorldToScreen(To, ToX, ToY, gScreenWidth, gScreenHeight))
            {
                Orange::Gui::Glow::Line(Draw, FromX, FromY, ToX, ToY,
                                        Orange::Gui::Glow::Tint(Color, Orange::Gui::UiIntensity()), 1.0f, 0.5f);
            }
        }
        (void)ScreenX;
        (void)ScreenY;
    }

    void BlockRender::SetConfig(bool Enabled, int TargetMaskValue, float Range, int ModeValue, int ColorMode)
    {
        gBlockRender.Enabled = Enabled;
        gBlockRender.Mask = TargetMask::Normalize(TargetMaskValue);
        gBlockRender.Range = Range;
        gBlockRender.Mode = ModeValue;
        gBlockRender.ColorMode = ColorMode;
    }

    void BlockRender::Init()
    {
        gBlockRender.Spin = 0.0f;
    }

    void BlockRender::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gBlockRender.Enabled)
        {
            return;
        }
        gBlockRender.Spin += 2.0f;
        if (gBlockRender.Spin >= 360.0f)
        {
            gBlockRender.Spin -= 360.0f;
        }
    }

    void BlockRender::Draw()
    {
        if (!gBlockRender.Enabled)
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
        float BaseX = std::floor(Position.X);
        float BaseY = std::floor(Position.Y) - 1.0f;
        float BaseZ = std::floor(Position.Z);
        int Radius = static_cast<int>(gBlockRender.Range);
        if (Radius < 1)
        {
            Radius = 1;
        }
        if (Radius > 8)
        {
            Radius = 8;
        }
        for (int OffsetX = -Radius; OffsetX <= Radius; ++OffsetX)
        {
            for (int OffsetZ = -Radius; OffsetZ <= Radius; ++OffsetZ)
            {
                Vec3 Origin;
                Origin.X = BaseX + static_cast<float>(OffsetX);
                Origin.Y = BaseY;
                Origin.Z = BaseZ + static_cast<float>(OffsetZ);
                Vec3 Corners[8];
                BuildUnitBox(Origin, 1.0f, Corners);
                unsigned int Color = RenderCore::ColorFromMode(gBlockRender.ColorMode, 0xFFFFFFFFu,
                                                               gBlockRender.Spin + static_cast<float>(OffsetX * 12 + OffsetZ * 7));
                DrawBoxEdges(Draw, Corners, Color, 1.0f);
            }
        }
    }

    void WorldRender::SetEnabled(bool Enabled, int ModeValue)
    {
        gWorldRender.Enabled = Enabled;
        gWorldRender.Mode = ModeValue;
    }

    void WorldRender::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gWorldRender.Enabled)
        {
            return;
        }
        gWorldRender.Spin += 1.0f;
        if (gWorldRender.Spin >= 360.0f)
        {
            gWorldRender.Spin -= 360.0f;
        }
    }

    void WorldRender::Draw()
    {
        if (!gWorldRender.Enabled)
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
        const int kGridRadius = 16;
        const float kGridStep = 4.0f;
        for (int Offset = -kGridRadius; Offset <= kGridRadius; ++Offset)
        {
            Vec3 From;
            Vec3 To;
            From.X = Origin.X + static_cast<float>(Offset) * kGridStep;
            From.Y = Origin.Y;
            From.Z = Origin.Z - static_cast<float>(kGridRadius) * kGridStep;
            To.X = From.X;
            To.Y = From.Y;
            To.Z = Origin.Z + static_cast<float>(kGridRadius) * kGridStep;
            float FromX = 0.0f;
            float FromY = 0.0f;
            float ToX = 0.0f;
            float ToY = 0.0f;
            if (RenderCore::WorldToScreen(From, FromX, FromY, gScreenWidth, gScreenHeight) &&
                RenderCore::WorldToScreen(To, ToX, ToY, gScreenWidth, gScreenHeight))
            {
                Orange::Gui::Glow::Line(Draw, FromX, FromY, ToX, ToY, 0x40FFFFFFu, 1.0f, 0.0f);
            }
            From.X = Origin.X - static_cast<float>(kGridRadius) * kGridStep;
            From.Z = Origin.Z + static_cast<float>(Offset) * kGridStep;
            To.X = Origin.X + static_cast<float>(kGridRadius) * kGridStep;
            To.Z = From.Z;
            if (RenderCore::WorldToScreen(From, FromX, FromY, gScreenWidth, gScreenHeight) &&
                RenderCore::WorldToScreen(To, ToX, ToY, gScreenWidth, gScreenHeight))
            {
                Orange::Gui::Glow::Line(Draw, FromX, FromY, ToX, ToY, 0x40FFFFFFu, 1.0f, 0.0f);
            }
        }
        for (int NodeX = -kGridRadius; NodeX <= kGridRadius; NodeX += 4)
        {
            for (int NodeZ = -kGridRadius; NodeZ <= kGridRadius; NodeZ += 4)
            {
                Vec3 Point;
                Point.X = Origin.X + static_cast<float>(NodeX) * kGridStep;
                Point.Y = Origin.Y;
                Point.Z = Origin.Z + static_cast<float>(NodeZ) * kGridStep;
                float ScreenX = 0.0f;
                float ScreenY = 0.0f;
                if (!RenderCore::WorldToScreen(Point, ScreenX, ScreenY, gScreenWidth, gScreenHeight))
                {
                    continue;
                }
                Orange::Gui::Glow::CircleFilled(Draw, ScreenX, ScreenY, 2.5f, 0x60FFFFFFu, 0.8f);
            }
        }
    }
}
