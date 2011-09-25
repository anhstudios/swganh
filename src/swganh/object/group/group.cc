
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
    , last_member_index_(0)
{
}

Group::~Group()
{
}

void Group::AddGroupMember(std::shared_ptr<BaseTangible> member)
{
    uint16_t next_index = GetNextMemberIndex_();
    member_list_.insert(std::make_pair(next_index, member));
    last_member_index_ = next_index;
    GroupMessageBuilder::BuildMemberListDelta(this, 1, next_index, member);
}

void Group::RemoveGroupMember(std::shared_ptr<BaseTangible> member)
{
    uint16_t index_position = 0;
    auto iter = std::find_if(member_list_.begin(), member_list_.end(), [=, &index_position](std::pair<uint16_t, std::shared_ptr<BaseTangible>> list_member)->bool {
        if(member->GetObjectId() == list_member.second->GetObjectId())
        {
            index_position = list_member.first;
            return true;
        }

        return false;
    });

    if(iter != member_list_.end())
    {
        member_list_.erase(iter);
        GroupMessageBuilder::BuildMemberListDelta(this, 0, index_position, member);
        member_index_free_list_.push(index_position);
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

uint16_t Group::GetNextMemberIndex_()
{
    uint16_t next_member_index = last_member_index_++;

    if(member_index_free_list_.size() > 0) {
        next_member_index = member_index_free_list_.top();
        member_index_free_list_.pop();
    }

    return next_member_index;
}