// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "group.h"
#include "swganh_core/object/tangible/tangible.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object;

Group::Group()
    : difficulty_(0)
    , loot_master_(0)
    , loot_mode_(FREE_LOOT)
{
}

Group::Group(uint32_t max_member_size)
    : difficulty_(0)
    , loot_master_(0)
    , loot_mode_(FREE_LOOT)
{
}

Group::~Group()
{
}

void Group::AddGroupMember(uint64_t member, std::string name) { AddGroupMember(member, name, AcquireLock());}
void Group::AddGroupMember(uint64_t member, std::string name, boost::unique_lock<boost::mutex>& lock)
{
	member_list_.add(Member(member, name));
    DISPATCH(Group, Member);
}

void Group::RemoveGroupMember(uint64_t member) { RemoveGroupMember(member, AcquireLock());}
void Group::RemoveGroupMember(uint64_t member, boost::unique_lock<boost::mutex>& lock)
{
	auto iter = std::find_if(begin(member_list_), end(member_list_), [=](const Member& x)->bool {
		return member == x.object_id;
	});

	if(iter == end(member_list_))
	{
		return;
	}
        
	member_list_.remove(iter);
    DISPATCH(Group, Member);
}
    
std::vector<Member> Group::GetGroupMembers() { return GetGroupMembers(AcquireLock());}
std::vector<Member> Group::GetGroupMembers(boost::unique_lock<boost::mutex>& lock)
{
    return member_list_.raw();
}

void Group::SerializeGroupMembers(swganh::messages::BaseSwgMessage* message) {SerializeGroupMembers(message, AcquireLock());}
void Group::SerializeGroupMembers(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	member_list_.Serialize(message);
}

void Group::SetLootMode(LootMode loot_mode) { SetLootMode(loot_mode, AcquireLock());}
void Group::SetLootMode(LootMode loot_mode, boost::unique_lock<boost::mutex>& lock)
{
    loot_mode_ = loot_mode;
	DISPATCH(Group, LootMode);
}

LootMode Group::GetLootMode() { return GetLootMode(AcquireLock());}
LootMode Group::GetLootMode(boost::unique_lock<boost::mutex>& lock)
{
    uint32_t loot_mode = loot_mode_;
    return (LootMode)loot_mode;
}

void Group::SetDifficulty(uint16_t difficulty) { SetDifficulty(difficulty, AcquireLock());}
void Group::SetDifficulty(uint16_t difficulty, boost::unique_lock<boost::mutex>& lock)
{
    difficulty_ = difficulty;
	DISPATCH(Group, Difficulty);
}

uint16_t Group::GetDifficulty() { return GetDifficulty(AcquireLock());}
uint16_t Group::GetDifficulty(boost::unique_lock<boost::mutex>& lock)
{
    return difficulty_;
}

void Group::SetLootMaster(uint64_t loot_master) { SetLootMaster(loot_master, AcquireLock());}
void Group::SetLootMaster(uint64_t loot_master, boost::unique_lock<boost::mutex>& lock)
{
    loot_master_ = loot_master;
	DISPATCH(Group, LootMaster);
}

uint64_t Group::GetLootMaster() { return GetLootMaster(AcquireLock());}
uint64_t Group::GetLootMaster(boost::unique_lock<boost::mutex>& lock)
{
    return loot_master_;
}

uint16_t Group::GetSize() { return GetSize(AcquireLock());}
uint16_t Group::GetSize(boost::unique_lock<boost::mutex>& lock)
{
    return member_list_.size();
}