#include "orange/Game/Support.h"
#include "orange/Game/Infra.h"
#include "orange/Game/Sdk.h"

namespace Orange::Game
{
    Tag::Tag() = default;
    Tag::Tag(const std::string& InName, int InType) : Name(InName), Type(InType) {}
    Tag::~Tag() = default;
    Tag& Tag::operator=(const Tag& Other) = default;

    TrailRecord::TrailRecord() = default;
    TrailRecord::TrailRecord(float InX, float InY, float InZ, float InAge)
        : X(InX), Y(InY), Z(InZ), Age(InAge) {}
    TrailRecord::~TrailRecord() = default;
    TrailRecord& TrailRecord::operator=(const TrailRecord& Other) = default;

    TPLocationEntry::TPLocationEntry() = default;
    TPLocationEntry::~TPLocationEntry() = default;
    TPLocationEntry& TPLocationEntry::operator=(const TPLocationEntry& Other) = default;

    HudLine::HudLine() = default;
    HudLine::HudLine(const std::string& InText, unsigned int InColor, float InOffset)
        : Text(InText), Color(InColor), Offset(InOffset) {}
    HudLine::~HudLine() = default;
    HudLine& HudLine::operator=(const HudLine& Other) = default;

    EffectEntry::EffectEntry() = default;
    EffectEntry::~EffectEntry() = default;
    EffectEntry& EffectEntry::operator=(const EffectEntry& Other) = default;

    PlayerEntry::PlayerEntry() = default;
    PlayerEntry::~PlayerEntry() = default;
    PlayerEntry& PlayerEntry::operator=(const PlayerEntry& Other) = default;

    CsArgSpec::CsArgSpec() = default;
    CsArgSpec::~CsArgSpec() = default;
    CsArgSpec& CsArgSpec::operator=(const CsArgSpec& Other) = default;

    CsSuggestEntry::CsSuggestEntry() = default;
    CsSuggestEntry::~CsSuggestEntry() = default;
    CsSuggestEntry& CsSuggestEntry::operator=(const CsSuggestEntry& Other) = default;

    CamState::CamState() = default;
    CamState::~CamState() = default;
    CamState& CamState::operator=(const CamState& Other) = default;

    KaActionBarHolder::KaActionBarHolder() = default;
    KaActionBarHolder::~KaActionBarHolder() = default;
    KaActionBarHolder& KaActionBarHolder::operator=(const KaActionBarHolder& Other) = default;

    KaTickGuard::KaTickGuard() = default;

    KaTickGuard::KaTickGuard(bool* Target) : Flag(Target)
    {
        if (Flag)
        {
            Previous = *Flag;
            *Flag = true;
        }
    }

    KaTickGuard::~KaTickGuard()
    {
        if (Flag)
        {
            *Flag = Previous;
        }
    }

    KaTickGuard& KaTickGuard::operator=(const KaTickGuard& Other) = default;

    HookFunctionHandle::HookFunctionHandle() = default;
    HookFunctionHandle::~HookFunctionHandle() = default;
    HookFunctionHandle& HookFunctionHandle::operator=(const HookFunctionHandle& Other) = default;

    bool RangeLess::operator()(float Left, float Right) const
    {
        return Left < Right;
    }

    ChatReq::ChatReq() = default;
    ChatReq::ChatReq(const std::string& InText, int InPriority) : Text(InText), Priority(InPriority) {}
    ChatReq::~ChatReq() = default;
    ChatReq& ChatReq::operator=(const ChatReq& Other) = default;

    void ItemIcon::Init()
    {
        ItemIconStore::Init();
    }

    void* ItemIcon::Texture(int ItemId, int Variant)
    {
        return ItemIconStore::Texture(ItemId, Variant);
    }

    void* ItemIcon::Find(int ItemId)
    {
        return ItemIconStore::Find(ItemId);
    }

    int ItemIcon::VariantCount(int ItemId)
    {
        return ItemIconStore::VariantCount(ItemId);
    }

    void ItemIcon::ReleaseAll()
    {
        ItemIconStore::ReleaseAll();
    }

    int ItemIcon::ResidentCount()
    {
        return ItemIconStore::ResidentCount();
    }

    void BlockClick::Install()
    {
        BlockClickTracker::Install();
    }

    bool BlockClick::Installed()
    {
        return BlockClickTracker::Installed();
    }

    void BlockClick::ProcessTick(void* LocalPlayer)
    {
        BlockClickTracker::ProcessTick(LocalPlayer);
    }

    void BlockClick::Clear()
    {
        BlockClickTracker::Clear();
    }

    int BlockClick::GetClick(int Index)
    {
        return BlockClickTracker::GetClick(Index);
    }

    int BlockClick::Serial()
    {
        return BlockClickTracker::Serial();
    }

    BtGroup::BtGroup() = default;
    BtGroup::~BtGroup() = default;
    BtGroup& BtGroup::operator=(const BtGroup& Other) = default;

    BtLoadJob::BtLoadJob() = default;
    BtLoadJob::~BtLoadJob() = default;
    BtLoadJob& BtLoadJob::operator=(const BtLoadJob& Other) = default;

    BtExportJob::BtExportJob() = default;
    BtExportJob::~BtExportJob() = default;
    BtExportJob& BtExportJob::operator=(const BtExportJob& Other) = default;
}
