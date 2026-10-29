#pragma once

#include "orange/Common.h"

namespace Orange::Game
{
    struct Tag
    {
        std::string Name;
        int Type = 0;
        Tag();
        Tag(const std::string& InName, int InType);
        ~Tag();
        Tag& operator=(const Tag& Other);
    };

    struct TrailRecord
    {
        float X = 0.0f;
        float Y = 0.0f;
        float Z = 0.0f;
        float Age = 0.0f;
        TrailRecord();
        TrailRecord(float InX, float InY, float InZ, float InAge);
        ~TrailRecord();
        TrailRecord& operator=(const TrailRecord& Other);
    };

    struct TPLocationEntry
    {
        std::string Name;
        float X = 0.0f;
        float Y = 0.0f;
        float Z = 0.0f;
        TPLocationEntry();
        ~TPLocationEntry();
        TPLocationEntry& operator=(const TPLocationEntry& Other);
    };

    struct HudLine
    {
        std::string Text;
        unsigned int Color = 0xFFFFFFFFu;
        float Offset = 0.0f;
        HudLine();
        HudLine(const std::string& InText, unsigned int InColor, float InOffset);
        ~HudLine();
        HudLine& operator=(const HudLine& Other);
    };

    struct EffectEntry
    {
        int Id = 0;
        int Duration = 0;
        int Amplifier = 0;
        bool Ambient = false;
        EffectEntry();
        ~EffectEntry();
        EffectEntry& operator=(const EffectEntry& Other);
    };

    struct PlayerEntry
    {
        void* Actor = nullptr;
        std::string Name;
        int Level = 0;
        PlayerEntry();
        ~PlayerEntry();
        PlayerEntry& operator=(const PlayerEntry& Other);
    };

    struct CsArgSpec
    {
        std::string Name;
        std::vector<std::string> Values;
        std::vector<std::string> Enums;
        CsArgSpec();
        ~CsArgSpec();
        CsArgSpec& operator=(const CsArgSpec& Other);
    };

    struct CsSuggestEntry
    {
        std::string Name;
        std::string Description;
        bool Ours = false;
        CsSuggestEntry();
        ~CsSuggestEntry();
        CsSuggestEntry& operator=(const CsSuggestEntry& Other);
    };

    struct CamState
    {
        float Position[3] = { 0.0f, 0.0f, 0.0f };
        float Rotation[2] = { 0.0f, 0.0f };
        bool Valid = false;
        CamState();
        ~CamState();
        CamState& operator=(const CamState& Other);
    };

    struct KaActionBarHolder
    {
        std::string Text;
        float Time = 0.0f;
        int Style = 0;
        KaActionBarHolder();
        ~KaActionBarHolder();
        KaActionBarHolder& operator=(const KaActionBarHolder& Other);
    };

    struct KaTickGuard
    {
        bool* Flag = nullptr;
        bool Previous = false;
        KaTickGuard();
        explicit KaTickGuard(bool* Target);
        ~KaTickGuard();
        KaTickGuard& operator=(const KaTickGuard& Other);
    };

    struct HookFunctionHandle
    {
        void* Target = nullptr;
        void* Original = nullptr;
        bool Installed = false;
        HookFunctionHandle();
        ~HookFunctionHandle();
        HookFunctionHandle& operator=(const HookFunctionHandle& Other);
    };

    struct RangeLess
    {
        bool operator()(float Left, float Right) const;
    };

    class ChatReq
    {
    public:
        std::string Text;
        int Priority = 0;
        ChatReq();
        ChatReq(const std::string& InText, int InPriority);
        ~ChatReq();
        ChatReq& operator=(const ChatReq& Other);
    };

    class ItemIcon
    {
    public:
        static void Init();
        static void* Texture(int ItemId, int Variant);
        static void* Find(int ItemId);
        static int VariantCount(int ItemId);
        static void ReleaseAll();
        static int ResidentCount();
    };

    class BlockClick
    {
    public:
        static void Install();
        static bool Installed();
        static void ProcessTick(void* LocalPlayer);
        static void Clear();
        static int GetClick(int Index);
        static int Serial();
    };

    class BtGroup
    {
    public:
        std::string Name;
        std::vector<std::string> Entries;
        bool Enabled = false;
        BtGroup();
        ~BtGroup();
        BtGroup& operator=(const BtGroup& Other);
    };

    class BtLoadJob
    {
    public:
        std::string Path;
        int Progress = 0;
        bool Done = false;
        BtLoadJob();
        ~BtLoadJob();
        BtLoadJob& operator=(const BtLoadJob& Other);
    };

    class BtExportJob
    {
    public:
        std::string Path;
        int Progress = 0;
        bool Done = false;
        BtExportJob();
        ~BtExportJob();
        BtExportJob& operator=(const BtExportJob& Other);
    };
}
