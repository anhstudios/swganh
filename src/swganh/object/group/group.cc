
#include "swganh/object/group/group.h"
#include "swganh/object/group/group_message_builder.h"

#include "swganh/object/tangible/tangible.h"
#include "swganh/messages/deltas_message.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object;
using namespace swganh::object::group;
using namespace swganh::object::tangible;

Group::Group()
    : member_list_counter_(0)
    , difficulty_(0)
    , loot_master_(0)
    , loot_mode_(FREE_LOOT)
{
}

Group::Group(uint32_t max_member_size)
    : member_list_counter_(0)
    , difficulty_(0)
    , loot_master_(0)
    , loot_mode_(FREE_LOOT)
{
    member_list_.resize(max_member_size);
}

Group::~Group()
{
}

void Group::AddGroupMember(std::shared_ptr<BaseTangible> member)
{
    uint16_t next_index = swganh::object::GetNextAvailableSlot(member_list_, member_index_free_list_);
    member_list_[next_index] = member;
    GroupMessageBuilder::BuildMemberListDelta(this, 1, next_index, member);
}

void Group::RemoveGroupMember(std::shared_ptr<BaseTangible> member)
{
    auto iter = FindMemberIterById_(member->GetObjectId());
    if(iter != member_list_.end())
    {
        uint16_t index_position = std::distance(member_list_.begin(), iter);
        member_list_.erase(iter);
        GroupMessageBuilder::BuildMemberListDelta(this, 0, index_position, member);
        member_index_free_list_.push_back(index_position);
    }
}

void Group::SetLootMode(LOOT_MODE loot_mode)
{
    loot_mode_ = loot_mode;
    GroupMessageBuilder::BuildLootMasterDelta(this);
}

void Group::SetDifficulty(uint16_t difficulty)
{
    difficulty_ = difficulty;
    GroupMessageBuilder::BuildDifficultyDelta(this);
}

void Group::SetLootMaster(uint64_t loot_master)
{
    loot_master_ = loot_master;
    GroupMessageBuilder::BuildLootMasterDelta(this);
}

boost::optional<BaselinesMessage> Group::GetBaseline6()
{
    return GroupMessageBuilder::BuildBaseline6(this);
}

Group::MemberList::iterator Group::FindMemberIterById_(uint64_t id)
{
    auto iter = std::find_if(member_list_.begin(), member_list_.end(), [=](std::shared_ptr<swganh::object::tangible::BaseTangible> member)->bool {
        if(member->GetObjectId() == id)
            return true;
        else
            return false;
    });

    if(iter == member_list_.end())
        throw std::out_of_range("Group member does not exist.");

    return iter;
}