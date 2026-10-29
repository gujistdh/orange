#include "orange/Gui/Gui.h"
#include "orange/Gui/Glow.h"
#include "orange/Gui/PanelRender.h"
#include "orange/Game/RenderCore.h"
#include "orange/Gui/ModuleBuilder.h"
#include "orange/Modules/ModuleRegistry.h"
#include "orange/Modules/Editors/Editors.h"
#include "orange/Modules/World/TPMod.h"

namespace Orange::Gui
{
    int kBindersMainCount = 45;
    int kBindersExtraCount = 61;

    namespace
    {
        std::vector<Panel*> Panels;
        bool Built = false;
        struct PanelDefinition
        {
            const char* Name;
            const char* Category;
            const char* FileName;
        };

        const PanelDefinition kPanelDefinitions[] = {
            { "Combat", "战斗", "combat.json" },
            { "Movement", "移动", "movement.json" },
            { "Player", "玩家", "player.json" },
            { "Render", "渲染", "render.json" },
            { "World", "世界", "world.json" },
            { "Script", "脚本", "script.json" },
            { "Misc", "杂物", "Misc.json" },
        };

        const std::vector<std::string>& EmptyPresets()
        {
            static const std::vector<std::string> Presets = {
                "白色", "黑色", "红色", "橙色", "黄色", "绿色", "青色", "蓝色", "紫色", "粉色"
            };
            return Presets;
        }
    }

    int DefaultPanelCount()
    {
        return static_cast<int>(sizeof(kPanelDefinitions) / sizeof(kPanelDefinitions[0]));
    }

    const char* DefaultPanelFileName(int Index)
    {
        if (Index < 0 || Index >= DefaultPanelCount())
        {
            return nullptr;
        }
        return kPanelDefinitions[Index].FileName;
    }

    const char* DefaultPanelJson(int Index)
    {
        if (Index < 0 || Index >= DefaultPanelCount())
        {
            return nullptr;
        }
        return "{}";
    }

    Panel* CreatePanel(const std::string& Name, const std::string& Category)
    {
        Panel* Result = new Panel(Name, Category);
        Result->SetIndex(static_cast<int>(Panels.size()));
        Panels.push_back(Result);
        return Result;
    }

    void BuildPanels(std::vector<Panel*>& Out)
    {
        for (int Index = 0; Index < DefaultPanelCount(); ++Index)
        {
            const PanelDefinition& Definition = kPanelDefinitions[Index];
            Panel* Target = CreatePanel(Definition.Name, Definition.Category);
            Out.push_back(Target);
            Modules::RegisterCategory(Definition.Name, Target);
        }
    }

    void Build()
    {
        if (Built)
        {
            return;
        }
        std::vector<Panel*> Local;
        BuildPanels(Local);
        for (Panel* Target : Panels)
        {
            for (Module* Item : Target->GetModules())
            {
                if (Item && Item->IsOn())
                {
                    Item->Apply();
                }
            }
        }
        Built = true;
    }

    std::vector<Panel*>& GetPanels()
    {
        return Panels;
    }

    Module* BuildModule(const JsonValue& Definition)
    {
        const JsonValue* Id = Definition.Find("id");
        if (!Id || !Id->IsString())
        {
            return nullptr;
        }
        bool Enabled = false;
        if (const JsonValue* Keybind = Definition.Find("keybind"))
        {
            Enabled = Keybind->BoolOr(false);
        }
        ModuleBuilder Builder(Id->AsString(), Enabled);
        Module* Result = Builder.Build();
        if (const JsonValue* Name = Definition.Find("name"))
        {
            Result->SetName(Name->StringOr(Id->AsString()));
        }
        return Result;
    }

    Module* FindModule(const std::string& Id)
    {
        for (Panel* Target : Panels)
        {
            Module* Found = Target->FindModule(Id);
            if (Found)
            {
                return Found;
            }
        }
        return nullptr;
    }

    Module* FindBinder(const std::string& Id)
    {
        return FindModule(Id);
    }

    float BinderToFloat(const std::string& Text)
    {
        if (Text.empty())
        {
            return 0.0f;
        }
        return static_cast<float>(std::atof(Text.c_str()));
    }

    unsigned int BinderColorOf(Module* Owner, const char* Name)
    {
        if (!Owner || !Name)
        {
            return 0xFFFFFFFFu;
        }
        return ParseColor(Owner->GetText(Name));
    }

    unsigned int ParseColor(const std::string& Text)
    {
        if (Text.size() == 7 && Text[0] == '#')
        {
            return static_cast<unsigned int>(std::strtoul(Text.c_str() + 1, nullptr, 16)) << 8 | 0xFFu;
        }
        static const std::pair<const char*, unsigned int> kNames[] = {
            { "白色", 0xFFFFFFFFu }, { "黑色", 0xFF000000u }, { "红色", 0xFFFF0000u },
            { "橙色", 0xFFFF8000u }, { "黄色", 0xFFFFFF00u }, { "绿色", 0xFF00FF00u },
            { "青色", 0xFF00FFFFu }, { "蓝色", 0xFF0000FFu }, { "紫色", 0xFF8000FFu },
            { "粉色", 0xFFFF80C0u },
        };
        for (const auto& Entry : kNames)
        {
            if (Text == Entry.first)
            {
                return Entry.second;
            }
        }
        return 0xFFFFFFFFu;
    }

    const std::vector<std::string>& ColorPresets()
    {
        return EmptyPresets();
    }

    float UiUnit()
    {
        return 1.0f;
    }

    namespace
    {
        std::mutex gActionBarMutex;
        std::string gActionBarText;
        float gActionBarDuration = 0.0f;
        uint64_t gActionBarExpire = 0;
        int gActionBarStyle = 0;
        int gActionBarColor = 0;
        int gActionBarFlags = 0;

        bool gWatermarkEnabled = true;
        float gWatermarkIntensity = 1.0f;
        bool gNotifyEnabled = true;
        float gNotifyIntensity = 1.0f;

        uint64_t NowMillis()
        {
            using Clock = std::chrono::steady_clock;
            return static_cast<uint64_t>(
                std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now().time_since_epoch()).count());
        }

        int ClampInt(int Value, int Min, int Max)
        {
            if (Value < Min)
            {
                return Min;
            }
            if (Value > Max)
            {
                return Max;
            }
            return Value;
        }
    }

    bool IsWatermarkEnabled()
    {
        return gWatermarkEnabled;
    }

    void SetWatermarkEnabled(bool Value, float Delay)
    {
        gWatermarkEnabled = Value;
        gWatermarkIntensity = Glow::ClampIntensity(Delay);
    }

    float WatermarkIntensity()
    {
        return gWatermarkIntensity;
    }

    float UiIntensity()
    {
        return gWatermarkIntensity;
    }

    void SetUiIntensity(float Value)
    {
        gWatermarkIntensity = Glow::ClampIntensity(Value);
    }

    bool IsNotifyEnabled()
    {
        return gNotifyEnabled;
    }

    void SetNotifyEnabled(bool Value, float Delay)
    {
        gNotifyEnabled = Value;
        gNotifyIntensity = Glow::ClampIntensity(Delay);
    }

    float NotifyIntensity()
    {
        return gNotifyIntensity;
    }

    void SetActionBarText(const std::string& Text, float Duration)
    {
        std::lock_guard<std::mutex> Lock(gActionBarMutex);
        gActionBarText = Text;
        gActionBarDuration = Duration;
        gActionBarExpire = NowMillis() + 1200;
    }

    void SetActionBarStyle(int Style, int Color, int Flags)
    {
        std::lock_guard<std::mutex> Lock(gActionBarMutex);
        gActionBarStyle = ClampInt(Style, 0, 30);
        gActionBarColor = ClampInt(Color, 0, 30);
        gActionBarFlags = ClampInt(Flags, 0, 100);
    }

    bool GetActionBarText(std::string& OutText, float& OutAlpha)
    {
        std::lock_guard<std::mutex> Lock(gActionBarMutex);
        if (gActionBarText.empty())
        {
            return false;
        }
        uint64_t Now = NowMillis();
        if (Now >= gActionBarExpire)
        {
            gActionBarText.clear();
            return false;
        }
        uint64_t Remaining = gActionBarExpire - Now;
        float Fade = Remaining > 400 ? 1.0f : static_cast<float>(Remaining) / 400.0f;
        OutText = gActionBarText;
        OutAlpha = Fade;
        return true;
    }

    int ActionBarStyle()
    {
        std::lock_guard<std::mutex> Lock(gActionBarMutex);
        return gActionBarStyle;
    }

    int ActionBarColor()
    {
        std::lock_guard<std::mutex> Lock(gActionBarMutex);
        return gActionBarColor;
    }

    int ActionBarFlags()
    {
        std::lock_guard<std::mutex> Lock(gActionBarMutex);
        return gActionBarFlags;
    }

    void DrawActionBar(ImDrawList* Draw)
    {
        if (!Draw)
        {
            return;
        }
        std::string Text;
        float Alpha = 0.0f;
        if (!GetActionBarText(Text, Alpha))
        {
            return;
        }
        int Style = ActionBarStyle();
        int ColorMode = ActionBarColor();
        unsigned int Color = Orange::Game::RenderCore::ColorFromMode(ColorMode, 0xFFFFFFFFu, static_cast<float>(Style) * 10.0f);
        unsigned int FinalColor = Glow::Tint(Color, Alpha);
        float CenterX = 960.0f;
        float CenterY = 1080.0f - 160.0f;
        Glow::Text(Draw, CenterX - 60.0f, CenterY, FinalColor, Text.c_str(), 1.2f);
    }

    bool IsLimitedMode() { return false; }


    void OpenConfigGui(Module* Target) { (void)Target; }
    void OpenMusicGui() {}
    void OpenPyLoaderGui() {}
    void OpenNbtValueInput(const std::string& Path) { (void)Path; }

    bool OnTouch(int Action, float X, float Y)
    {
        if (Action != 0)
        {
            return false;
        }
        Modules::Editors::NbtEditor::OnTouch(X, Y);
        Modules::Editors::ItemSetEditor::OnTouch(X, Y);
        Modules::Editors::EnchantEditor::OnTouch(X, Y);
        Modules::Editors::ParticleEditor::OnTouch(X, Y);
        Modules::Editors::SoundEditor::OnTouch(X, Y);
        Modules::Editors::EffectEditor::OnTouch(X, Y);
        Modules::Editors::ViewUserEditor::OnTouch(X, Y);
        Modules::Editors::ViewCommandBlockEditor::OnTouch(X, Y);
        Modules::World::TPMod::OnTouch(X, Y);
        if (PanelRender::OnTouch(Action, X, Y, 1920.0f, 1080.0f))
        {
            return true;
        }
        return false;
    }
    void OnTextInputResult(const std::string& Text) { (void)Text; }

    void AuthClose() {}
    void AuthRegisterOk(const std::string& Text) { (void)Text; }
}
