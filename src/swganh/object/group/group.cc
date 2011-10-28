
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
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    member_list_.Add(Member(member, name));
    GroupMessageBuilder::BuildMemberListDelta(this);
}

void Group::RemoveGroupMember(uint64_t member)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    auto iter = std::find_if(member_list_.Begin(), member_list_.End(), [=](const Member& x)->bool {
        return member == x.object_id;
    });

    if(iter != member_list_.End())
    {
        member_list_.Remove(iter);
        GroupMessageBuilder::BuildMemberListDelta(this);
    }
}

void Group::SetLootMode(LootMode loot_mode)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    loot_mode_ = loot_mode;
    GroupMessageBuilder::BuildLootMasterDelta(this);
}

LootMode Group::GetLootMode(void)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return (LootMode)loot_mode_;
}

void Group::SetDifficulty(uint16_t difficulty)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    difficulty_ = difficulty;
    GroupMessageBuilder::BuildDifficultyDelta(this);
}

uint16_t Group::GetDifficulty(void)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return difficulty_;
}

void Group::SetLootMaster(uint64_t loot_master)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    loot_master_ = loot_master;
    GroupMessageBuilder::BuildLootMasterDelta(this);
}

uint64_t Group::GetLootMaster(void)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return loot_master_;
}

boost::optional<BaselinesMessage> Group::GetBaseline3()
{
    return GroupMessageBuilder::BuildBaseline3(this);
}

boost::optional<BaselinesMessage> Group::GetBaseline6()
{
    return GroupMessageBuilder::BuildBaseline6(this);
}