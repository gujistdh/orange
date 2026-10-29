#include "orange/Modules/Editors/Editors.h"
#include "orange/Modules/Editors/EditorUi.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/RenderCore.h"
#include "orange/Gui/Glow.h"
#include "orange/Gui/LiquidGlass.h"
#include "orange/Config/NbtNode.h"
#include "orange/Core/Main.h"
#include "orange/Game/IconAtlas.h"

#include <fstream>

namespace Orange::Modules::Editors
{
    using namespace Orange::Game;

    namespace
    {
        struct EditorState
        {
            bool Enabled = false;
            int Selected = 0;
            int Scroll = 0;
            int Page = 0;
            bool InputOpen = false;
            std::string InputPath;
            std::string InputBuffer;
            EditorTheme Theme;
        };

        EditorState gNbt;
        EditorState gItemSet;
        EditorState gEnchant;
        EditorState gParticle;
        EditorState gSound;
        EditorState gEffect;
        EditorState gViewUser;
        EditorState gViewCommand;

        Orange::Config::NbtNode gNbtRoot;
        std::vector<std::string> gParticleList;
        std::vector<std::string> gSoundList;
        std::vector<std::string> gEffectList;
        int gCommandMode = 0;
        int gCommandStyle = 0;
        int gCommandCount = 0;
        bool gCommandFlag1 = false;
        bool gCommandFlag2 = false;
        bool gCommandFlag3 = false;
        bool gCommandFlag4 = false;

        void ResetEditor(EditorState& State)
        {
            State.Selected = 0;
            State.Scroll = 0;
            State.Page = 0;
            State.InputOpen = false;
            State.InputBuffer.clear();
            State.InputPath.clear();
        }

        void InitThemes()
        {
            gNbt.Theme = EditorUi::Default();
            gNbt.Theme.PanelWidth = 420.0f;
            gNbt.Theme.PanelHeight = 480.0f;
            gItemSet.Theme = EditorUi::Default();
            gItemSet.Theme.PanelWidth = 300.0f;
            gItemSet.Theme.PanelHeight = 260.0f;
            gEnchant.Theme = EditorUi::Default();
            gEnchant.Theme.PanelWidth = 320.0f;
            gEnchant.Theme.PanelHeight = 300.0f;
            gParticle.Theme = EditorUi::Default();
            gParticle.Theme.PanelWidth = 380.0f;
            gParticle.Theme.PanelHeight = 500.0f;
            gSound.Theme = EditorUi::Default();
            gSound.Theme.PanelWidth = 380.0f;
            gSound.Theme.PanelHeight = 500.0f;
            gEffect.Theme = EditorUi::Default();
            gEffect.Theme.PanelWidth = 340.0f;
            gEffect.Theme.PanelHeight = 440.0f;
            gViewUser.Theme = EditorUi::Default();
            gViewUser.Theme.PanelWidth = 300.0f;
            gViewUser.Theme.PanelHeight = 240.0f;
            gViewCommand.Theme = EditorUi::Default();
            gViewCommand.Theme.PanelWidth = 340.0f;
            gViewCommand.Theme.PanelHeight = 300.0f;
        }

        void DrawList(ImDrawList* Draw, const EditorState& State, const char* Title,
                      const std::vector<std::string>& Items, const char* Footer)
        {
            if (!Draw)
            {
                return;
            }
            EditorUi::DrawPanel(Draw, State.Theme, Title);
            int Visible = EditorUi::VisibleRows(State.Theme);
            int Start = State.Scroll;
            if (Start < 0)
            {
                Start = 0;
            }
            for (int Row = 0; Row < Visible; ++Row)
            {
                int Index = Start + Row;
                if (Index < 0 || static_cast<std::size_t>(Index) >= Items.size())
                {
                    break;
                }
                EditorUi::DrawChip(Draw, State.Theme, Row, Items[static_cast<std::size_t>(Index)].c_str(),
                                   Index == State.Selected);
            }
            if (Footer)
            {
                EditorUi::DrawFooter(Draw, State.Theme, Footer);
            }
        }
    }

    void NbtEditor::SetEnabled(bool Value)
    {
        bool Was = gNbt.Enabled;
        gNbt.Enabled = Value;
        if (Was != Value && Value)
        {
            ResetEditor(gNbt);
        }
    }

    bool NbtEditor::IsEnabled()
    {
        return gNbt.Enabled;
    }

    void NbtEditor::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gNbt.Enabled)
        {
            return;
        }
        gNbt.Scroll = gNbt.Scroll < 0 ? 0 : gNbt.Scroll;
    }

    void NbtEditor::Draw()
    {
        if (!gNbt.Enabled)
        {
            return;
        }
        std::vector<std::string> Items;
        for (const Orange::Config::NbtNode& Child : gNbtRoot.GetChildren())
        {
            Items.push_back(Child.GetKey());
        }
        if (Items.empty())
        {
            Items.push_back("(空)");
        }
        char Footer[64];
        std::snprintf(Footer, sizeof(Footer), "%d 项", static_cast<int>(Items.size()));
        DrawList(ImGui::GetBackgroundDrawList(), gNbt, "NBT 编辑器", Items, Footer);
    }

    void NbtEditor::OnTouch(float X, float Y)
    {
        if (!gNbt.Enabled)
        {
            return;
        }
        int Row = EditorUi::HitTestRow(gNbt.Theme, X, Y, 64);
        if (Row >= 0)
        {
            gNbt.Selected = gNbt.Scroll + Row;
        }
    }

    void NbtEditor::OpenValueInput(const std::string& Path)
    {
        gNbt.InputOpen = true;
        gNbt.InputPath = Path;
        gNbt.InputBuffer.clear();
    }

    void NbtEditor::OnTextInputResult(const std::string& Text)
    {
        if (!gNbt.InputOpen)
        {
            return;
        }
        gNbt.InputBuffer = Text;
        gNbt.InputOpen = false;
    }

    void NbtEditor::LoadFromPath(const std::string& Path)
    {
        std::ifstream Stream(Path, std::ios::binary);
        if (!Stream)
        {
            return;
        }
        std::string Content((std::istreambuf_iterator<char>(Stream)), std::istreambuf_iterator<char>());
        Orange::Config::NbtParser Parser(Content);
        Orange::Config::NbtNode Root;
        if (Parser.Parse(Root))
        {
            gNbtRoot = Root;
        }
    }

    void NbtEditor::SaveToPath(const std::string& Path)
    {
        std::ofstream Stream(Path, std::ios::binary | std::ios::trunc);
        if (!Stream)
        {
            return;
        }
        Stream << gNbtRoot.Serialize();
    }

    int NbtEditor::SelectedIndex()
    {
        return gNbt.Selected;
    }

    void NbtEditor::SetSelectedIndex(int Value)
    {
        gNbt.Selected = Value;
    }

    void ItemSetEditor::SetEnabled(bool Value)
    {
        bool Was = gItemSet.Enabled;
        gItemSet.Enabled = Value;
        if (Was != Value && Value)
        {
            ResetEditor(gItemSet);
        }
    }

    void ItemSetEditor::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gItemSet.Enabled)
        {
            return;
        }
    }

    void ItemSetEditor::Draw()
    {
        if (!gItemSet.Enabled)
        {
            return;
        }
        std::vector<std::string> Items = { "保存当前物品", "应用已保存物品", "清空" };
        DrawList(ImGui::GetBackgroundDrawList(), gItemSet, "物品编辑器", Items, nullptr);
    }

    void ItemSetEditor::OnTouch(float X, float Y)
    {
        if (!gItemSet.Enabled)
        {
            return;
        }
        int Row = EditorUi::HitTestRow(gItemSet.Theme, X, Y, 3);
        if (Row >= 0)
        {
            gItemSet.Selected = Row;
        }
    }

    void EnchantEditor::SetEnabled(bool Value)
    {
        bool Was = gEnchant.Enabled;
        gEnchant.Enabled = Value;
        if (Was != Value && Value)
        {
            ResetEditor(gEnchant);
        }
    }

    void EnchantEditor::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gEnchant.Enabled)
        {
            return;
        }
    }

    void EnchantEditor::Draw()
    {
        if (!gEnchant.Enabled)
        {
            return;
        }
        std::vector<std::string> Items = { "锋利", "耐久", "保护", "经验修补", "无限", "击退" };
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        DrawList(Draw, gEnchant, "附魔编辑器", Items, nullptr);
        if (!Draw)
        {
            return;
        }
        for (int Index = 0; Index < static_cast<int>(Items.size()); ++Index)
        {
            unsigned int Texture = Game::IconAtlas::EnchantIcon(Index + 1);
            if (!Texture)
            {
                continue;
            }
            EditorLayout Layout = EditorUi::RowBounds(gEnchant.Theme, Index);
            Draw->AddImage(static_cast<ImTextureID>(Texture),
                           ImVec2(Layout.X + Layout.Width - 24.0f, Layout.Y + 4.0f),
                           ImVec2(Layout.X + Layout.Width - 6.0f, Layout.Y + Layout.Height - 4.0f));
        }
    }

    void EnchantEditor::OnTouch(float X, float Y)
    {
        if (!gEnchant.Enabled)
        {
            return;
        }
        int Row = EditorUi::HitTestRow(gEnchant.Theme, X, Y, 6);
        if (Row >= 0)
        {
            gEnchant.Selected = Row;
        }
    }

    void ParticleEditor::SetEnabled(bool Value)
    {
        bool Was = gParticle.Enabled;
        gParticle.Enabled = Value;
        ResetEditor(gParticle);
        (void)Was;
    }

    void ParticleEditor::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gParticle.Enabled)
        {
            return;
        }
    }

    void ParticleEditor::Draw()
    {
        if (!gParticle.Enabled)
        {
            return;
        }
        std::vector<std::string> Items = gParticleList;
        if (Items.empty())
        {
            Items.push_back("(无粒子)");
        }
        char Footer[64];
        std::snprintf(Footer, sizeof(Footer), "已选 %d", gParticle.Selected);
        DrawList(ImGui::GetBackgroundDrawList(), gParticle, "粒子编辑器", Items, Footer);
    }

    void ParticleEditor::OnTouch(float X, float Y)
    {
        if (!gParticle.Enabled)
        {
            return;
        }
        int Row = EditorUi::HitTestRow(gParticle.Theme, X, Y, static_cast<int>(gParticleList.size()));
        if (Row >= 0)
        {
            gParticle.Selected = gParticle.Scroll + Row;
        }
    }

    int ParticleEditor::SelectedCount()
    {
        return gParticle.Selected;
    }

    void SoundEditor::SetEnabled(bool Value)
    {
        bool Was = gSound.Enabled;
        gSound.Enabled = Value;
        ResetEditor(gSound);
        (void)Was;
    }

    void SoundEditor::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gSound.Enabled)
        {
            return;
        }
    }

    void SoundEditor::Draw()
    {
        if (!gSound.Enabled)
        {
            return;
        }
        std::vector<std::string> Items = gSoundList;
        if (Items.empty())
        {
            Items.push_back("(无音效)");
        }
        char Footer[64];
        std::snprintf(Footer, sizeof(Footer), "%d 条音效", static_cast<int>(Items.size()));
        DrawList(ImGui::GetBackgroundDrawList(), gSound, "音效编辑器", Items, Footer);
    }

    void SoundEditor::OnTouch(float X, float Y)
    {
        if (!gSound.Enabled)
        {
            return;
        }
        int Row = EditorUi::HitTestRow(gSound.Theme, X, Y, static_cast<int>(gSoundList.size()));
        if (Row >= 0)
        {
            gSound.Selected = gSound.Scroll + Row;
        }
    }

    void EffectEditor::SetEnabled(bool Value)
    {
        bool Was = gEffect.Enabled;
        gEffect.Enabled = Value;
        ResetEditor(gEffect);
        (void)Was;
    }

    void EffectEditor::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gEffect.Enabled)
        {
            return;
        }
    }

    void EffectEditor::Draw()
    {
        if (!gEffect.Enabled)
        {
            return;
        }
        std::vector<std::string> Items = gEffectList;
        if (Items.empty())
        {
            Items.push_back("(无药水)");
        }
        DrawList(ImGui::GetBackgroundDrawList(), gEffect, "效果编辑器", Items, nullptr);
    }

    void EffectEditor::OnTouch(float X, float Y)
    {
        if (!gEffect.Enabled)
        {
            return;
        }
        int Row = EditorUi::HitTestRow(gEffect.Theme, X, Y, static_cast<int>(gEffectList.size()));
        if (Row >= 0)
        {
            gEffect.Selected = gEffect.Scroll + Row;
        }
    }

    void ViewUserEditor::SetEnabled(bool Value)
    {
        bool Was = gViewUser.Enabled;
        gViewUser.Enabled = Value;
        if (Was != Value && Value)
        {
            ResetEditor(gViewUser);
        }
    }

    void ViewUserEditor::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gViewUser.Enabled)
        {
            return;
        }
    }

    void ViewUserEditor::Draw()
    {
        if (!gViewUser.Enabled)
        {
            return;
        }
        std::vector<std::string> Items = { "查看信息", "复制名称", "关闭" };
        DrawList(ImGui::GetBackgroundDrawList(), gViewUser, "查看用户", Items, nullptr);
    }

    void ViewUserEditor::OnTouch(float X, float Y)
    {
        if (!gViewUser.Enabled)
        {
            return;
        }
        int Row = EditorUi::HitTestRow(gViewUser.Theme, X, Y, 3);
        if (Row >= 0)
        {
            gViewUser.Selected = Row;
        }
    }

    void ViewCommandBlockEditor::SetConfig(bool Enabled, int Mode, int Style, bool Flag1, bool Flag2,
                                           bool Flag3, bool Flag4, int Count)
    {
        bool Was = gViewCommand.Enabled;
        gViewCommand.Enabled = Enabled;
        gCommandMode = Mode;
        gCommandStyle = Style;
        gCommandFlag1 = Flag1;
        gCommandFlag2 = Flag2;
        gCommandFlag3 = Flag3;
        gCommandFlag4 = Flag4;
        gCommandCount = Count;
        if (Was != Enabled && Enabled)
        {
            ResetEditor(gViewCommand);
        }
    }

    void ViewCommandBlockEditor::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gViewCommand.Enabled)
        {
            return;
        }
        if (LocalPlayer)
        {
            Game::Render::InstallHooks();
        }
    }

    void ViewCommandBlockEditor::Draw()
    {
        if (!gViewCommand.Enabled)
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        if (!Draw)
        {
            return;
        }
        Orange::Gui::LiquidGlass::AddRectClipped(gViewCommand.Theme.PanelX, gViewCommand.Theme.PanelY,
                                                 gViewCommand.Theme.PanelWidth, gViewCommand.Theme.PanelHeight,
                                                 gViewCommand.Theme.PanelRadius, gViewCommand.Theme.PanelAlpha,
                                                 0.0f, 0.0f, 1920.0f, 1080.0f);
        std::vector<std::string> Items = { "命令方块列表", "显示坐标", "显示命令", "复制" };
        DrawList(Draw, gViewCommand, "查看命令方块", Items, nullptr);
    }

    void ViewCommandBlockEditor::OnTouch(float X, float Y)
    {
        if (!gViewCommand.Enabled)
        {
            return;
        }
        int Row = EditorUi::HitTestRow(gViewCommand.Theme, X, Y, 4);
        if (Row >= 0)
        {
            gViewCommand.Selected = Row;
        }
    }

    namespace
    {
        struct ThemeInitializer
        {
            ThemeInitializer()
            {
                InitThemes();
            }
        };

        ThemeInitializer gThemeInitializer;
    }
}
