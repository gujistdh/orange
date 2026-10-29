#include "orange/Modules/Script/ScriptModules.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Game/RenderCore.h"
#include "orange/Core/Main.h"
#include "orange/Gui/Gui.h"
#include "orange/Gui/Glow.h"
#include "orange/Auth/ApiAuth.h"

#include <deque>
#include <mutex>
#include <fstream>
#include <dirent.h>
#include <fstream>
#include "orange/Game/Support.h"
#include "orange/Config/NbtNode.h"

namespace Orange::Modules::Script
{
    using namespace Orange::Game;

    namespace
    {
        std::mutex gMessageMutex;
        std::deque<std::string> gMessages;
        int gMessageMode = 0;

        bool gTextEnabled = false;

        bool gRunCommandEnabled = false;
        int gRunCommandMode = 0;
        int gRunCommandCount = 1;
        float gRunCommandDelay = 0.0f;

        std::mutex gCommandMutex;
        std::deque<std::string> gCommandQueue;
        std::map<std::string, std::string> gCommandRegistry;

        bool gChatEffectsEnabled = false;
        int gChatEffectsMode = 0;
        int gChatEffectsColor = 0;
        float gChatEffectsScale = 1.0f;
        int gChatEffectsStyle = 0;
        float gChatEffectsTimer = 0.0f;

        bool gBuildingToolsRunning = false;
        std::string gBuildingToolsFile;
        const char* const kBuildDir = "/storage/emulated/0/Android/data/com.netease.x19/files/Orange/build";

        bool gCardKeyTimeRunning = false;
        float gCardKeyTimeSeconds = 0.0f;
        std::mutex gChatMutex;
        std::string gChatUserName;
        std::string gChatTitle;
        std::string gChatLevel;
        int gChatLevelValue = 0;
        bool gChatEnabled = false;
        bool gChatPending = false;
        std::deque<std::string> gChatQueue;

        const char* const kUserNamePath = "/storage/emulated/0/Android/data/com.netease.x19/files/Orange/user.txt";

        bool IsLevelToken(const std::string& Token)
        {
            if (Token.size() < 2 || Token.size() > 0x10)
            {
                return false;
            }
            char First = Token[0];
            char Second = Token[1];
            if ((First == 'L' || First == 'l') && (Second == 'v' || Second == 'V'))
            {
                return true;
            }
            return First >= '0' && First <= '9';
        }

        bool IsSpace(char Ch)
        {
            return Ch == ' ' || Ch == '\t' || Ch == '\r' || Ch == '\n';
        }

        std::string TrimRecord(const std::string& Value)
        {
            std::size_t Start = 0;
            std::size_t End = Value.size();
            while (Start < End && IsSpace(Value[Start]))
            {
                ++Start;
            }
            while (End > Start && IsSpace(Value[End - 1]))
            {
                --End;
            }
            return Value.substr(Start, End - Start);
        }

        std::string FirstLine(const std::string& Value)
        {
            std::size_t Split = Value.find('\n');
            if (Split == std::string::npos)
            {
                return Value;
            }
            return Value.substr(0, Split);
        }
        std::vector<std::string> SplitRecord(const std::string& Record)
        {
            std::vector<std::string> Parts;
            std::string Current;
            for (char Ch : Record)
            {
                if (Ch == '[')
                {
                    if (!Current.empty())
                    {
                        Parts.push_back(Current);
                        Current.clear();
                    }
                    continue;
                }
                if (Ch == ']')
                {
                    if (!Current.empty())
                    {
                        Parts.push_back(Current);
                        Current.clear();
                    }
                    continue;
                }
                Current.push_back(Ch);
            }
            if (!Current.empty())
            {
                Parts.push_back(Current);
            }
            return Parts;
        }

        void ParseRecord(const std::string& Record)
        {
            std::string Cleaned = TrimRecord(Record);
            Cleaned = FirstLine(Cleaned);
            Cleaned = TrimRecord(Cleaned);
            std::vector<std::string> Parts = SplitRecord(Cleaned);
            gChatTitle.clear();
            gChatLevel.clear();
            gChatLevelValue = 0;
            for (std::size_t Index = 1; Index < Parts.size(); ++Index)
            {
                if (IsLevelToken(Parts[Index]))
                {
                    if (gChatLevel.empty())
                    {
                        gChatLevel = Parts[Index];
                        const char* Digits = Parts[Index].c_str();
                        if ((Parts[Index][0] == 'L' || Parts[Index][0] == 'l') && Parts[Index].size() > 2)
                        {
                            Digits = Parts[Index].c_str() + 2;
                        }
                        gChatLevelValue = std::atoi(Digits);
                    }
                    continue;
                }
                if (gChatTitle.empty())
                {
                    gChatTitle = Parts[Index];
                }
            }
        }

        std::string ReadUserNameFile()
        {
            std::ifstream Stream(kUserNamePath, std::ios::binary);
            if (!Stream)
            {
                return std::string();
            }
            char Buffer[512];
            Stream.read(Buffer, sizeof(Buffer) - 1);
            std::streamsize Read = Stream.gcount();
            if (Read <= 0)
            {
                return std::string();
            }
            Buffer[Read] = 0;
            return std::string(Buffer);
        }
    }

    void Message::SendMessage(const std::string& TextValue)
    {
        std::lock_guard<std::mutex> Lock(gMessageMutex);
        gMessages.push_back(TextValue);
        if (gMessages.size() > 64)
        {
            gMessages.pop_front();
        }
    }

    void Message::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        std::lock_guard<std::mutex> Lock(gMessageMutex);
        while (!gMessages.empty())
        {
            const std::string& Front = gMessages.front();
            Game::Text::SendChat(Front);
            gMessages.pop_front();
        }
    }

    void Message::DrawHud()
    {
    }

    bool Message::HasHudMessages()
    {
        std::lock_guard<std::mutex> Lock(gMessageMutex);
        return !gMessages.empty();
    }

    int Message::GetMode()
    {
        return gMessageMode;
    }

    void Message::SetMode(int Value)
    {
        gMessageMode = Value;
    }

    void Text::Init()
    {
        gTextEnabled = true;
    }

    bool Text::IsNetworkReady()
    {
        return Orange::Core::BaseAddress != 0;
    }

    void Text::SetEnabled(bool Value)
    {
        gTextEnabled = Value;
    }

    void Text::SendChat(const std::string& Message)
    {
        if (!gTextEnabled)
        {
            return;
        }
        Game::Text::SendChat(Message);
    }

    void Text::SendCommand(const std::string& CommandValue)
    {
        if (!gTextEnabled)
        {
            return;
        }
        Game::Text::SendCommand(CommandValue);
    }

    void Text::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gTextEnabled)
        {
            return;
        }
        std::lock_guard<std::mutex> Lock(gChatMutex);
        while (!gChatQueue.empty())
        {
            Game::Text::SendChat(gChatQueue.front());
            gChatQueue.pop_front();
        }
    }

    void RunCommand::SetConfig(bool Enabled, int Mode, int Count, float Delay)
    {
        gRunCommandEnabled = Enabled;
        gRunCommandMode = Mode;
        gRunCommandCount = Count;
        gRunCommandDelay = Delay;
    }

    void RunCommand::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gRunCommandEnabled)
        {
            return;
        }
    }

    void Command::Init()
    {
        gCommandRegistry.clear();
    }

    void Command::RegisterCommand(const std::string& Name, const std::string& Description)
    {
        std::lock_guard<std::mutex> Lock(gCommandMutex);
        gCommandRegistry[Name] = Description;
    }

    void Command::EnqueueCommand(const std::string& CommandValue)
    {
        std::lock_guard<std::mutex> Lock(gCommandMutex);
        gCommandQueue.push_back(CommandValue);
    }

    void Command::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        std::lock_guard<std::mutex> Lock(gCommandMutex);
        while (!gCommandQueue.empty())
        {
            Game::Text::SendCommand(gCommandQueue.front());
            gCommandQueue.pop_front();
        }
    }


    void Chatroom::Init()
    {
        std::lock_guard<std::mutex> Lock(gChatMutex);
        gChatUserName = ReadUserNameFile();
        gChatTitle.clear();
        gChatLevel.clear();
        gChatLevelValue = 0;
        gChatEnabled = false;
        gChatPending = false;
    }

    bool Chatroom::IsEnabled()
    {
        return gChatEnabled;
    }

    void Chatroom::SetEnabled(bool Value)
    {
        gChatEnabled = Value;
    }

    void Chatroom::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gChatEnabled)
        {
            return;
        }
    }

    std::string Chatroom::GetUserName(void* Player)
    {
        (void)Player;
        std::lock_guard<std::mutex> Lock(gChatMutex);
        if (!gChatUserName.empty())
        {
            return gChatUserName;
        }
        return ReadUserNameFile();
    }

    std::string Chatroom::GetUserNameCached(void* Player)
    {
        return GetUserName(Player);
    }

    std::string Chatroom::GetUserRecord(void* Player)
    {
        return GetUserName(Player);
    }

    int Chatroom::GetUserLevel(void* Player)
    {
        (void)Player;
        std::lock_guard<std::mutex> Lock(gChatMutex);
        return gChatLevelValue;
    }

    std::string Chatroom::GetUserTitle(void* Player)
    {
        (void)Player;
        std::lock_guard<std::mutex> Lock(gChatMutex);
        return gChatTitle;
    }

    std::string Chatroom::GetUserLevelText(void* Player)
    {
        (void)Player;
        std::lock_guard<std::mutex> Lock(gChatMutex);
        return gChatLevel;
    }

    bool Chatroom::HasUserName(void* Player)
    {
        (void)Player;
        std::lock_guard<std::mutex> Lock(gChatMutex);
        return !gChatUserName.empty();
    }

    void Chatroom::SaveUserName(const std::string& Name)
    {
        std::lock_guard<std::mutex> Lock(gChatMutex);
        gChatUserName = Name;
        std::ofstream Stream(kUserNamePath, std::ios::binary | std::ios::trunc);
        if (Stream)
        {
            Stream << Name;
        }
    }

    void Chatroom::SetRecord(const std::string& Record)
    {
        std::lock_guard<std::mutex> Lock(gChatMutex);
        ParseRecord(Record);
    }

    void Chatroom::CancelUserNameInput()
    {
        std::lock_guard<std::mutex> Lock(gChatMutex);
        gChatPending = false;
    }

    void Chatroom::SubmitUserName(const std::string& Name)
    {
        SaveUserName(Name);
        std::lock_guard<std::mutex> Lock(gChatMutex);
        gChatPending = false;
    }

    void Chatroom::OnAuthSuccess()
    {
        std::lock_guard<std::mutex> Lock(gChatMutex);
        gChatEnabled = true;
    }

    void Chatroom::RefreshUser()
    {
        std::lock_guard<std::mutex> Lock(gChatMutex);
        gChatLevelValue = 0;
    }

    void Chatroom::RequestUserDialog()
    {
        std::lock_guard<std::mutex> Lock(gChatMutex);
        gChatPending = true;
    }

    bool Chatroom::TryIntercept(const std::string& Message)
    {
        if (Message.empty())
        {
            return false;
        }
        if (!Orange::Core::BaseAddress)
        {
            return false;
        }
        intptr_t VtableOffset = Orange::Offsets::Load(Orange::Offsets::Slot::TextPacket_Vtable);
        if (!VtableOffset)
        {
            return false;
        }
        if (Message[0] == '.')
        {
            Command::EnqueueCommand(Message.substr(1));
            return true;
        }
        return false;
    }

    void ChatEffects::SetConfig(bool Enabled, int Mode, int ColorMode, float Scale, int Style)
    {
        gChatEffectsEnabled = Enabled;
        gChatEffectsMode = Mode;
        gChatEffectsColor = ColorMode;
        gChatEffectsScale = Scale;
        gChatEffectsStyle = Style;
    }

    void ChatEffects::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gChatEffectsEnabled)
        {
            return;
        }
        gChatEffectsTimer += 0.05f;
        if (gChatEffectsTimer > 10.0f)
        {
            gChatEffectsTimer = 0.0f;
        }
    }

    namespace
    {
        int gBtState = 0;
        int gBtTimer = 0;
        bool gBtReady = false;
        std::string gBtCurrentFile;
        std::mutex gBtMutex;
        Game::BtGroup gBtGroup;
        Game::BtLoadJob gBtLoadJob;
        Game::BtExportJob gBtExportJob;

        constexpr int kBtStateIdle = 0;
        constexpr int kBtStateLoading = 1;
        constexpr int kBtStatePlacing = 2;
        constexpr int kBtStateExporting = 3;
    }

    void BuildingTools::Init()
    {
        std::lock_guard<std::mutex> Lock(gBtMutex);
        gBtState = kBtStateIdle;
        gBtTimer = 0;
        gBtReady = false;
        gBtCurrentFile.clear();
        gBtLoadJob = Game::BtLoadJob();
        gBtExportJob = Game::BtExportJob();
    }

    bool BuildingTools::IsRunning()
    {
        if (gBtState == kBtStateIdle)
        {
            return gBtReady || gBuildingToolsRunning;
        }
        return true;
    }

    void BuildingTools::SetEnabled(bool Value)
    {
        std::lock_guard<std::mutex> Lock(gBtMutex);
        gBtReady = Value;
        if (!Value)
        {
            gBtState = kBtStateIdle;
            gBtTimer = 0;
        }
    }

    void BuildingTools::SelectFile(const std::string& Name)
    {
        std::lock_guard<std::mutex> Lock(gBtMutex);
        std::string Resolved = Name;
        if (Resolved.size() < 5 || Resolved.compare(Resolved.size() - 5, 5, ".json") != 0)
        {
            Resolved += ".json";
        }
        gBtCurrentFile = Resolved;
        gBtLoadJob = Game::BtLoadJob();
        gBtLoadJob.Path = std::string(kBuildDir) + "/" + Resolved;
        gBtLoadJob.Progress = 0;
        gBtLoadJob.Done = false;
        gBtState = kBtStateLoading;
        gBtTimer = 20;
    }

    std::string BuildingTools::CurrentFile()
    {
        std::lock_guard<std::mutex> Lock(gBtMutex);
        return gBtCurrentFile;
    }

    std::vector<std::string> BuildingTools::ListFiles()
    {
        std::vector<std::string> Result;
        DIR* Handle = opendir(kBuildDir);
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

    void BuildingTools::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        std::lock_guard<std::mutex> Lock(gBtMutex);
        if (gBtTimer > 0)
        {
            --gBtTimer;
        }
        if (gBtState == kBtStateLoading)
        {
            if (gBtTimer > 0)
            {
                return;
            }
            std::ifstream Stream(gBtLoadJob.Path, std::ios::binary);
            if (Stream)
            {
                std::string Content((std::istreambuf_iterator<char>(Stream)), std::istreambuf_iterator<char>());
                Orange::Config::NbtParser Parser(Content);
                Orange::Config::NbtNode Root;
                if (Parser.Parse(Root))
                {
                    gBtLoadJob.Progress = 100;
                    gBtLoadJob.Done = true;
                    gBtState = kBtStatePlacing;
                    gBtTimer = 0;
                    return;
                }
            }
            gBtLoadJob.Done = true;
            gBtState = kBtStateIdle;
            gBtTimer = 0;
            return;
        }
        if (gBtState == kBtStatePlacing)
        {
            if (gBtLoadJob.Done && gBtTimer == 0)
            {
                gBtState = kBtStateIdle;
            }
        }
    }

    void BuildingTools::HandleCommand(const std::string& CommandValue)
    {
        std::lock_guard<std::mutex> Lock(gBtMutex);
        if (CommandValue.empty())
        {
            return;
        }
        std::vector<std::string> Tokens;
        std::string Current;
        for (char Ch : CommandValue)
        {
            if (Ch == ' ')
            {
                if (!Current.empty())
                {
                    Tokens.push_back(Current);
                    Current.clear();
                }
                continue;
            }
            Current.push_back(Ch);
        }
        if (!Current.empty())
        {
            Tokens.push_back(Current);
        }
        if (Tokens.empty())
        {
            return;
        }
        const std::string& Verb = Tokens[0];
        if (Verb == "load" && Tokens.size() >= 2)
        {
            std::string Name = Tokens[1];
            if (Name.size() < 5 || Name.compare(Name.size() - 5, 5, ".json") != 0)
            {
                Name += ".json";
            }
            gBtCurrentFile = Name;
            gBtLoadJob = Game::BtLoadJob();
            gBtLoadJob.Path = std::string(kBuildDir) + "/" + Name;
            gBtState = kBtStateLoading;
            gBtTimer = 20;
            return;
        }
        if (Verb == "stop")
        {
            gBtState = kBtStateIdle;
            gBtTimer = 0;
            return;
        }
        if (Verb == "export" && Tokens.size() >= 2)
        {
            gBtExportJob = Game::BtExportJob();
            gBtExportJob.Path = Tokens[1];
            gBtExportJob.Progress = 0;
            gBtExportJob.Done = false;
            gBtState = kBtStateExporting;
            return;
        }
    }

    void CardKeyTime::Init()
    {
    }

    void CardKeyTime::Run()
    {
        gCardKeyTimeRunning = true;
    }

    void CardKeyTime::ProcessTick(void* LocalPlayer)
    {
        (void)LocalPlayer;
        if (!gCardKeyTimeRunning)
        {
            return;
        }
        gCardKeyTimeSeconds += 0.05f;
    }

    void CardKeyTime::Draw()
    {
        if (!gCardKeyTimeRunning)
        {
            return;
        }
        ImDrawList* Draw = ImGui::GetBackgroundDrawList();
        if (!Draw)
        {
            return;
        }
        long long Remaining = static_cast<long long>(gCardKeyTimeSeconds);
        char Buffer[96];
        std::snprintf(Buffer, sizeof(Buffer), "卡密剩余 %lld 分 %lld 秒",
                      Remaining / 60, Remaining % 60);
        Orange::Gui::Glow::Text(Draw, 24.0f, 70.0f, 0xFFFFFFFFu, Buffer, Orange::Gui::UiIntensity());
    }

}
