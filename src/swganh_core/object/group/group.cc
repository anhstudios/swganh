// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "group.h"
#include "swganh_core/object/tangible/tangible.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::object;

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
		auto lock = AcquireLock();
		member_list_.Add(Member(member, name));
	}
    DISPATCH(Group, Member);
}

void Group::RemoveGroupMember(uint64_t member)
{
	{
		auto lock = AcquireLock();
		auto iter = std::find_if(begin(member_list_), end(member_list_), [=](const Member& x)->bool {
			return member == x.object_id;
		});

		if(iter == end(member_list_))
		{
			return;
		}
        
		member_list_.Remove(iter);
	}
    DISPATCH(Group, Member);
}
    
swganh::messages::containers::NetworkSortedVector<Member>& Group::GetGroupMembers()
{
    auto lock = AcquireLock();
    return member_list_;
}

void Group::SetLootMode(LootMode loot_mode)
{
    loot_mode_ = loot_mode;
	DISPATCH(Group, LootMode);
}

LootMode Group::GetLootMode(void)
{
    uint32_t loot_mode = loot_mode_;
    return (LootMode)loot_mode;
}

void Group::SetDifficulty(uint16_t difficulty)
{
    difficulty_ = difficulty;
	DISPATCH(Group, Difficulty);
}

uint16_t Group::GetDifficulty(void)
{
    return difficulty_;
}

void Group::SetLootMaster(uint64_t loot_master)
{
    loot_master_ = loot_master;
	DISPATCH(Group, LootMaster);
}

uint64_t Group::GetLootMaster(void)
{
    return loot_master_;
}

uint16_t Group::GetCapacity(void)
{
    auto lock = AcquireLock();
    return member_list_.Capacity();
}

uint16_t Group::GetSize(void)
{
    auto lock = AcquireLock();
    return member_list_.Size();
}