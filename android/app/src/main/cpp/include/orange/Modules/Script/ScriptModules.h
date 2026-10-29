#pragma once

#include "orange/Common.h"

namespace Orange::Modules::Script
{
    class Message
    {
    public:
        static void SendMessage(const std::string& Text);
        static void ProcessTick(void* LocalPlayer);
        static void DrawHud();
        static bool HasHudMessages();
        static int GetMode();
        static void SetMode(int Value);
    };

    class Text
    {
    public:
        static void Init();
        static bool IsNetworkReady();
        static void SetEnabled(bool Value);
        static void SendChat(const std::string& Text);
        static void SendCommand(const std::string& Command);
        static void ProcessTick(void* LocalPlayer);
    };

    class RunCommand
    {
    public:
        static void SetConfig(bool Enabled, int Mode, int Count, float Delay);
        static void ProcessTick(void* LocalPlayer);
    };

    class Command
    {
    public:
        static void Init();
        static void RegisterCommand(const std::string& Name, const std::string& Description);
        static void EnqueueCommand(const std::string& Command);
        static void ProcessTick(void* LocalPlayer);
    };

    class Chatroom
    {
    public:
        static void Init();
        static bool IsEnabled();
        static void SetEnabled(bool Value);
        static std::string GetUserNameCached(void* Player);
        static void SaveUserName(const std::string& Name);
        static void RefreshUser();
        static void RequestUserDialog();
        static bool TryIntercept(const std::string& Message);
        static void SetRecord(const std::string& Record);
        static std::string GetUserLevelText(void* Player);
        static void ProcessTick(void* LocalPlayer);
        static std::string GetUserName(void* Player);
        static std::string GetUserRecord(void* Player);
        static int GetUserLevel(void* Player);
        static std::string GetUserTitle(void* Player);
        static bool HasUserName(void* Player);
        static void CancelUserNameInput();
        static void SubmitUserName(const std::string& Name);
        static void OnAuthSuccess();
    };

    class ChatEffects
    {
    public:
        static void SetConfig(bool Enabled, int Mode, int ColorMode, float Scale, int Style);
        static void ProcessTick(void* LocalPlayer);
    };

    class BuildingTools
    {
    public:
        static void Init();
        static bool IsRunning();
        static void SetEnabled(bool Value);
        static void SelectFile(const std::string& Name);
        static std::string CurrentFile();
        static std::vector<std::string> ListFiles();
        static void ProcessTick(void* LocalPlayer);
        static void HandleCommand(const std::string& Command);
    };

    class CardKeyTime
    {
    public:
        static void Init();
        static void Run();
        static void ProcessTick(void* LocalPlayer);
        static void Draw();
        static void OnTouch(float X, float Y);
    };

    class PythonBridge
    {
    public:
        static void Init();
        static void ProcessTick(void* LocalPlayer);
    };
}
