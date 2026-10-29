#pragma once

#include "orange/Common.h"

namespace Orange::Modules::Script
{
    struct SuggestEntry
    {
        std::string Name;
        std::string Description;
        bool Ours = false;
    };

    struct ArgSpec
    {
        std::string Name;
        std::vector<std::string> Values;
        std::vector<std::string> Enums;
    };

    class CommandSuggest
    {
    public:
        static void Init();
        static void AddEntry(const std::string& Name, const std::string& Description, bool Ours);
        static bool IsOurs(const std::string& Name);
        static void SetArgEnums(const std::string& Command, const std::vector<std::string>& Values,
                                const std::vector<std::string>& Enums);
        static bool FindEntry(const std::string& Name, SuggestEntry& Out);
        static std::vector<SuggestEntry> List();
        static std::size_t Count();
    };

    class CommandSwitch
    {
    public:
        static void Init();
        static bool InterceptSend(const std::string& Text);
        static void SetInterceptNext(bool Value);
        static bool InterceptNext();
    };

    class ChatRequest
    {
    public:
        static void Push(const std::string& Text);
        static bool Pop(std::string& Out);
        static void Clear();
        static std::size_t Size();
    };
}
