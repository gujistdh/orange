#include "orange/Modules/Script/CommandSystem.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Core/Main.h"

#include <mutex>
#include <deque>

namespace Orange::Modules::Script
{
    using namespace Orange::Game;

    namespace
    {
        std::mutex gSuggestMutex;
        std::vector<SuggestEntry> gSuggestEntries;
        std::vector<ArgSpec> gArgSpecs;

        std::mutex gChatMutex;
        std::deque<std::string> gChatQueue;

        bool gInterceptNextSend = false;
        bool gCommandHooked = false;
    }

    void CommandSuggest::Init()
    {
        std::lock_guard<std::mutex> Lock(gSuggestMutex);
        gSuggestEntries.clear();
        gArgSpecs.clear();
    }

    void CommandSuggest::AddEntry(const std::string& Name, const std::string& Description, bool Ours)
    {
        std::lock_guard<std::mutex> Lock(gSuggestMutex);
        for (SuggestEntry& Entry : gSuggestEntries)
        {
            if (Entry.Name == Name)
            {
                Entry.Description = Description;
                Entry.Ours = Ours;
                return;
            }
        }
        SuggestEntry Entry;
        Entry.Name = Name;
        Entry.Description = Description;
        Entry.Ours = Ours;
        gSuggestEntries.push_back(Entry);
    }

    bool CommandSuggest::IsOurs(const std::string& Name)
    {
        std::lock_guard<std::mutex> Lock(gSuggestMutex);
        for (const SuggestEntry& Entry : gSuggestEntries)
        {
            if (Entry.Name == Name)
            {
                return true;
            }
        }
        return false;
    }

    void CommandSuggest::SetArgEnums(const std::string& Command, const std::vector<std::string>& Values,
                                     const std::vector<std::string>& Enums)
    {
        std::lock_guard<std::mutex> Lock(gSuggestMutex);
        for (ArgSpec& Spec : gArgSpecs)
        {
            if (Spec.Name == Command)
            {
                Spec.Values = Values;
                Spec.Enums = Enums;
                return;
            }
        }
        ArgSpec Spec;
        Spec.Name = Command;
        Spec.Values = Values;
        Spec.Enums = Enums;
        gArgSpecs.push_back(Spec);
    }

    bool CommandSuggest::FindEntry(const std::string& Name, SuggestEntry& Out)
    {
        std::lock_guard<std::mutex> Lock(gSuggestMutex);
        for (const SuggestEntry& Entry : gSuggestEntries)
        {
            if (Entry.Name == Name)
            {
                Out = Entry;
                return true;
            }
        }
        return false;
    }

    std::vector<SuggestEntry> CommandSuggest::List()
    {
        std::lock_guard<std::mutex> Lock(gSuggestMutex);
        return gSuggestEntries;
    }

    std::size_t CommandSuggest::Count()
    {
        std::lock_guard<std::mutex> Lock(gSuggestMutex);
        return gSuggestEntries.size();
    }

    void CommandSwitch::Init()
    {
        gCommandHooked = false;
        if (!Orange::Core::BaseAddress)
        {
            return;
        }
        gCommandHooked = true;
    }

    bool CommandSwitch::InterceptSend(const std::string& TextValue)
    {
        if (!gCommandHooked)
        {
            return false;
        }
        if (TextValue.empty())
        {
            return false;
        }
        if (TextValue[0] != '.')
        {
            return false;
        }
        std::string Command = TextValue.substr(1);
        if (Command.empty())
        {
            return false;
        }
        std::vector<SuggestEntry> Entries = CommandSuggest::List();
        for (const SuggestEntry& Entry : Entries)
        {
            if (Command.compare(0, Entry.Name.size(), Entry.Name) == 0)
            {
                ChatRequest::Push(TextValue);
                return true;
            }
        }
        return false;
    }

    void CommandSwitch::SetInterceptNext(bool Value)
    {
        gInterceptNextSend = Value;
    }

    bool CommandSwitch::InterceptNext()
    {
        bool Value = gInterceptNextSend;
        gInterceptNextSend = false;
        return Value;
    }

    void ChatRequest::Push(const std::string& TextValue)
    {
        std::lock_guard<std::mutex> Lock(gChatMutex);
        gChatQueue.push_back(TextValue);
        if (gChatQueue.size() > 32)
        {
            gChatQueue.pop_front();
        }
    }

    bool ChatRequest::Pop(std::string& Out)
    {
        std::lock_guard<std::mutex> Lock(gChatMutex);
        if (gChatQueue.empty())
        {
            return false;
        }
        Out = gChatQueue.front();
        gChatQueue.pop_front();
        return true;
    }

    void ChatRequest::Clear()
    {
        std::lock_guard<std::mutex> Lock(gChatMutex);
        gChatQueue.clear();
    }

    std::size_t ChatRequest::Size()
    {
        std::lock_guard<std::mutex> Lock(gChatMutex);
        return gChatQueue.size();
    }
}
