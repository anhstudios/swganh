
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
    : member_list_(5)
    , difficulty_(0)
    , loot_master_(0)
    , loot_mode_(FREE_LOOT)
{
}

Group::Group(uint32_t max_member_size)
    : member_list_(max_member_size)
    , difficulty_(0)
    , loot_master_(0)
    , loot_mode_(FREE_LOOT)
{
}

Group::~Group()
{
}

void Group::AddGroupMember(uint64_t member, std::string name)
{
    {
        std::lock_guard<std::mutex> lock(group_mutex_);
        member_list_.Add(Member(member, name));
    }

    GroupMessageBuilder::BuildMemberListDelta(this);
}

void Group::RemoveGroupMember(uint64_t member)
{
    {
        std::lock_guard<std::mutex> lock(group_mutex_);
        auto iter = std::find_if(begin(member_list_), end(member_list_), [=](const Member& x)->bool {
            return member == x.object_id;
        });

        if(iter == end(member_list_))
        {
            return;
        }
        
        member_list_.Remove(iter);
    }
            
    GroupMessageBuilder::BuildMemberListDelta(this);
}
    
swganh::messages::containers::NetworkSortedVector<Member> Group::GetGroupMembers()
{
    std::lock_guard<std::mutex> lock(group_mutex_);
    return member_list_;
}

void Group::SetLootMode(LootMode loot_mode)
{
    loot_mode_ = loot_mode;
    GroupMessageBuilder::BuildLootMasterDelta(this);
}

LootMode Group::GetLootMode(void)
{
    uint32_t loot_mode = loot_mode_;
    return (LootMode)loot_mode;
}

void Group::SetDifficulty(uint16_t difficulty)
{
    difficulty_ = difficulty;
    GroupMessageBuilder::BuildDifficultyDelta(this);
}

uint16_t Group::GetDifficulty(void)
{
    return difficulty_;
}

void Group::SetLootMaster(uint64_t loot_master)
{
    loot_master_ = loot_master;
    GroupMessageBuilder::BuildLootMasterDelta(this);
}

uint64_t Group::GetLootMaster(void)
{
    return loot_master_;
}

uint16_t Group::GetCapacity(void)
{
    std::lock_guard<std::mutex> lock(group_mutex_);
    return member_list_.Capacity();
}

uint16_t Group::GetSize(void)
{
    std::lock_guard<std::mutex> lock(group_mutex_);
    return member_list_.Size();
}

boost::optional<BaselinesMessage> Group::GetBaseline3()
{
    return GroupMessageBuilder::BuildBaseline3(this);
}

boost::optional<BaselinesMessage> Group::GetBaseline6()
{
    return GroupMessageBuilder::BuildBaseline6(this);
}