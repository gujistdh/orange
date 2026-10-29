#include "orange/Config/Configuration.h"
#include "orange/Gui/Gui.h"
#include "orange/Gui/Module.h"
#include "orange/Gui/Panel.h"
#include "orange/Gui/JsonValue.h"

#include <dirent.h>
#include <fstream>
#include <sys/stat.h>

namespace Orange::Config
{
    namespace
    {
        std::string gCurrentName = "default";
        const char* const kDir = "/storage/emulated/0/Android/data/com.netease.x19/files/Orange/config";
    }

    const char* ConfigDir()
    {
        return kDir;
    }

    std::vector<std::string> ListConfigs()
    {
        std::vector<std::string> Result;
        DIR* Handle = opendir(kDir);
        if (!Handle)
        {
            return Result;
        }
        struct dirent* Entry = nullptr;
        while ((Entry = readdir(Handle)) != nullptr)
        {
            if (Entry->d_type != DT_REG && Entry->d_type != DT_UNKNOWN)
            {
                continue;
            }
            std::string Name = Entry->d_name;
            if (Name.size() <= 5)
            {
                continue;
            }
            if (Name.compare(Name.size() - 5, 5, ".json") != 0)
            {
                continue;
            }
            Result.push_back(Name);
        }
        closedir(Handle);
        std::sort(Result.begin(), Result.end());
        return Result;
    }

    std::string Serialize()
    {
        Gui::JsonValue Root;
        Root.Set("orange", Gui::JsonValue(2.0));
        Gui::JsonValue Panels;
        for (Gui::Panel* Panel : Gui::GetPanels())
        {
            Gui::JsonValue PanelValue;
            PanelValue.Set("name", Gui::JsonValue(Panel->GetName()));
            Gui::JsonValue Modules;
            for (Gui::Module* Item : Panel->GetModules())
            {
                Gui::JsonValue ModuleValue;
                ModuleValue.Set("id", Gui::JsonValue(Item->GetId()));
                ModuleValue.Set("on", Gui::JsonValue(Item->IsOn()));
                Gui::JsonValue Settings;
                for (Gui::Property* Prop : Item->GetProperties())
                {
                    Gui::JsonValue PropValue;
                    PropValue.Set("name", Gui::JsonValue(Prop->GetName()));
                    switch (Prop->GetKind())
                    {
                    case Gui::Property::Kind::Number:
                        PropValue.Set("v", Gui::JsonValue(Prop->GetNum()));
                        break;
                    case Gui::Property::Kind::Flag:
                        PropValue.Set("v", Gui::JsonValue(Prop->GetBool()));
                        break;
                    case Gui::Property::Kind::Mode:
                    case Gui::Property::Kind::Check:
                        PropValue.Set("v", Gui::JsonValue(static_cast<double>(Prop->GetModeIndex())));
                        break;
                    case Gui::Property::Kind::Text:
                    case Gui::Property::Kind::Color:
                        PropValue.Set("v", Gui::JsonValue(Prop->GetText()));
                        break;
                    }
                    Settings.Push(PropValue);
                }
                ModuleValue.Set("settings", Settings);
                Modules.Push(ModuleValue);
            }
            PanelValue.Set("modules", Modules);
            Panels.Push(PanelValue);
        }
        Root.Set("panels", Panels);
        return Root.Serialize();
    }

    bool Deserialize(const std::string& Text)
    {
        Gui::JsonValue Root;
        if (!Gui::JsonParse(Text, Root))
        {
            return false;
        }
        const Gui::JsonValue* Panels = Root.Find("panels");
        if (!Panels || !Panels->IsArray())
        {
            return false;
        }
        for (std::size_t PanelIndex = 0; PanelIndex < Panels->Size(); ++PanelIndex)
        {
            const Gui::JsonValue* PanelValue = Panels->At(PanelIndex);
            if (!PanelValue)
            {
                continue;
            }
            const Gui::JsonValue* Modules = PanelValue->Find("modules");
            if (!Modules || !Modules->IsArray())
            {
                continue;
            }
            for (std::size_t ModuleIndex = 0; ModuleIndex < Modules->Size(); ++ModuleIndex)
            {
                const Gui::JsonValue* ModuleValue = Modules->At(ModuleIndex);
                if (!ModuleValue)
                {
                    continue;
                }
                const Gui::JsonValue* Id = ModuleValue->Find("id");
                if (!Id)
                {
                    continue;
                }
                Gui::Module* Target = Gui::FindModule(Id->AsString());
                if (!Target)
                {
                    continue;
                }
                if (const Gui::JsonValue* On = ModuleValue->Find("on"))
                {
                    Target->SetOn(On->BoolOr(false));
                }
                const Gui::JsonValue* Settings = ModuleValue->Find("settings");
                if (!Settings || !Settings->IsArray())
                {
                    continue;
                }
                for (std::size_t SettingIndex = 0; SettingIndex < Settings->Size(); ++SettingIndex)
                {
                    const Gui::JsonValue* Setting = Settings->At(SettingIndex);
                    if (!Setting)
                    {
                        continue;
                    }
                    const Gui::JsonValue* Name = Setting->Find("name");
                    const Gui::JsonValue* Value = Setting->Find("v");
                    if (!Name || !Value)
                    {
                        continue;
                    }
                    Gui::Property* Prop = Target->GetProp(Name->AsString());
                    if (!Prop)
                    {
                        continue;
                    }
                    switch (Prop->GetKind())
                    {
                    case Gui::Property::Kind::Number:
                        Prop->SetNum(Value->NumberOr(0.0));
                        break;
                    case Gui::Property::Kind::Flag:
                        Prop->SetBool(Value->BoolOr(false));
                        break;
                    case Gui::Property::Kind::Mode:
                        Prop->SetModeIndex(static_cast<int>(Value->NumberOr(0.0)));
                        break;
                    case Gui::Property::Kind::Check:
                        Prop->SetCheckMask(static_cast<int>(Value->NumberOr(0.0)));
                        break;
                    case Gui::Property::Kind::Text:
                    case Gui::Property::Kind::Color:
                        Prop->SetText(Value->StringOr(std::string()));
                        break;
                    }
                }
            }
        }
        return true;
    }

    bool LoadConfig(const std::string& Name)
    {
        std::string Path = std::string(kDir) + "/" + Name;
        std::ifstream Stream(Path, std::ios::binary);
        if (!Stream)
        {
            return false;
        }
        std::string Content((std::istreambuf_iterator<char>(Stream)), std::istreambuf_iterator<char>());
        gCurrentName = Name;
        return Deserialize(Content);
    }

    bool SaveConfig(const std::string& Name)
    {
        mkdir(kDir, 0777);
        std::string Path = std::string(kDir) + "/" + Name;
        std::ofstream Stream(Path, std::ios::binary | std::ios::trunc);
        if (!Stream)
        {
            return false;
        }
        Stream << Serialize();
        gCurrentName = Name;
        return true;
    }

    const std::string& CurrentName()
    {
        return gCurrentName;
    }

    void SetCurrentName(const std::string& Value)
    {
        gCurrentName = Value;
    }
}
