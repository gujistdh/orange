#include "orange/Modules/World/TPMod.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/RenderCore.h"
#include "orange/Modules/Script/ScriptModules.h"
#include "orange/Core/Main.h"

#include <cmath>

namespace Orange::Modules::World
{
    using namespace Orange::Game;

    namespace
    {
        std::mutex gTpMutex;
        std::vector<TPLocation> gLocations;
        bool gTpEnabled = false;
        int gTpMode = 0;
        bool gTpFlag = false;
        float gTpValue = 0.0f;
        int gTpSelected = -1;
        int gTpEditIndex = -1;
        int gTpEditField = 0;
        float gTpEditX = 0.0f;
        float gTpEditY = 0.0f;
        float gTpEditZ = 0.0f;
        std::string gTpEditName;
        float gTpMenuFloat = 0.0f;
        int gTpMenuInt = 0;
    }

    bool TPMod::IsEnabled()
    {
        return gTpEnabled;
    }

    void TPMod::SetConfig(bool Enabled, int Mode, bool Flag, float Value)
    {
        gTpEnabled = Enabled;
        if (!Enabled)
        {
            gTpFlag = false;
        }
        gTpMode = Mode;
        gTpFlag = Flag;
        gTpValue = Value;
        if (Enabled)
        {
            Script::Message::SendMessage("传送菜单已开启");
        }
        else
        {
            Script::Message::SendMessage("传送菜单已关闭");
        }
    }

    std::vector<TPLocation>& TPMod::GetLocations()
    {
        return gLocations;
    }

    void TPMod::ClearLocations()
    {
        std::lock_guard<std::mutex> Lock(gTpMutex);
        gLocations.clear();
        gTpSelected = -1;
    }

    void TPMod::AddLocationFromCurrentTick(void* Player)
    {
        if (!Player)
        {
            return;
        }
        Vec3 Position;
        if (!RenderCore::ActorPosition(Player, Position))
        {
            return;
        }
        TPLocation Entry;
        char Buffer[64];
        std::snprintf(Buffer, sizeof(Buffer), "位置 %d", static_cast<int>(gLocations.size()) + 1);
        Entry.Name = Buffer;
        Entry.X = Position.X;
        Entry.Y = Position.Y;
        Entry.Z = Position.Z;
        std::lock_guard<std::mutex> Lock(gTpMutex);
        gLocations.push_back(Entry);
    }

    bool TPMod::CaptureNowForCurrentPos(void* Player)
    {
        void* Local = Actor::GetPrimaryLocalPlayer();
        if (!Local)
        {
            return false;
        }
        AddLocationFromCurrentTick(Local);
        (void)Player;
        return true;
    }

    void TPMod::TeleportTo(const TPLocation& Location)
    {
        void* Local = Actor::GetPrimaryLocalPlayer();
        if (!Local)
        {
            return;
        }
        intptr_t Offset = Orange::Offsets::Load(Orange::Offsets::Slot::Actor_PosPtr);
        if (!Offset)
        {
            return;
        }
        float* Position = *reinterpret_cast<float**>(reinterpret_cast<Byte*>(Local) + Offset);
        if (!Position)
        {
            return;
        }
        Position[0] = Location.X;
        Position[1] = Location.Y;
        Position[2] = Location.Z;
        Actor::SetPos(Local, Position);
    }

    void TPMod::TeleportToIndex(int Index)
    {
        std::lock_guard<std::mutex> Lock(gTpMutex);
        if (Index < 0 || static_cast<std::size_t>(Index) >= gLocations.size())
        {
            return;
        }
        TPLocation Target = gLocations[static_cast<std::size_t>(Index)];
        TeleportTo(Target);
    }

    int TPMod::SelectedIndex()
    {
        return gTpSelected;
    }

    void TPMod::ProcessTick(void* LocalPlayer)
    {
        if (!gTpEnabled || !LocalPlayer)
        {
            return;
        }
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        if (!gTpFlag)
        {
            return;
        }
        TeleportToIndex(gTpSelected);
    }

    void TPMod::Draw()
    {
        if (!gTpEnabled)
        {
            return;
        }
    }

    void TPMod::OnTouch(float X, float Y)
    {
        if (!gTpEnabled)
        {
            return;
        }
        if (X < 40.0f || X > 380.0f || Y < 120.0f || Y > 540.0f)
        {
            return;
        }
        int Row = static_cast<int>((Y - 152.0f) / 22.0f);
        if (Row < 0)
        {
            return;
        }
        std::lock_guard<std::mutex> Lock(gTpMutex);
        if (static_cast<std::size_t>(Row) < gLocations.size())
        {
            gTpSelected = Row;
            gTpFlag = true;
        }
    }

    void TPMod::OpenEditField(int Index, int Field)
    {
        gTpEditIndex = Index;
        gTpEditField = Field;
        gTpEditX = 0.0f;
        gTpEditY = 0.0f;
        gTpEditZ = 0.0f;
    }

    void TPMod::ApplyEditInput(int Index, int Field, const std::string& Text)
    {
        if (Index != gTpEditIndex)
        {
            return;
        }
        if (Field == 0)
        {
            gTpEditName = Text;
            std::lock_guard<std::mutex> Lock(gTpMutex);
            if (Index >= 0 && static_cast<std::size_t>(Index) < gLocations.size())
            {
                gLocations[static_cast<std::size_t>(Index)].Name = Text;
            }
            return;
        }
        if (Field < 1 || Field > 3)
        {
            return;
        }
        float Value = 0.0f;
        try
        {
            Value = std::stof(Text);
        }
        catch (...)
        {
            return;
        }
        std::lock_guard<std::mutex> Lock(gTpMutex);
        if (Index < 0 || static_cast<std::size_t>(Index) >= gLocations.size())
        {
            return;
        }
        TPLocation& Entry = gLocations[static_cast<std::size_t>(Index)];
        if (Field == 1)
        {
            Entry.X = Value;
        }
        else if (Field == 2)
        {
            Entry.Y = Value;
        }
        else
        {
            Entry.Z = Value;
        }
    }

    float TPMod::GetMenuPropFloat(int Index)
    {
        (void)Index;
        return gTpMenuFloat;
    }

    void TPMod::SetMenuPropFloat(int Index, float Value)
    {
        (void)Index;
        gTpMenuFloat = Value;
    }

    int TPMod::GetMenuPropInt(int Index)
    {
        (void)Index;
        return gTpMenuInt;
    }

    void TPMod::SetMenuPropInt(int Index, int Value)
    {
        (void)Index;
        gTpMenuInt = Value;
    }
}
