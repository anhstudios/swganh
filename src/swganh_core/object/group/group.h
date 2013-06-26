// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <atomic>
#include <list>
#include <vector>

#include <boost/thread/mutex.hpp>

#include "swganh_core/object/object.h"
#include "swganh_core/messages/containers/network_vector.h"

#include "loot_mode.h"
#include "member.h"

namespace swganh {
namespace object {

	class Tangible;

}}  // namespace swganh::object

namespace swganh {
namespace object {

class GroupMessageBuilder;
class Group : public swganh::object::Object
{
public:
	typedef GroupMessageBuilder MessageBuilderType;

    Group();
    Group(uint32_t max_member_size);
    ~Group();

    // Group Members
    void AddGroupMember(uint64_t member, std::string name);
	void AddGroupMember(uint64_t member, std::string name, boost::unique_lock<boost::mutex>& lock);

    void RemoveGroupMember(uint64_t member);
	void RemoveGroupMember(uint64_t member, boost::unique_lock<boost::mutex>& lock);

    std::vector<Member> GetGroupMembers();
	std::vector<Member> GetGroupMembers(boost::unique_lock<boost::mutex>& lock);
    
	void SerializeGroupMembers(swganh::messages::BaseSwgMessage* message);
	void SerializeGroupMembers(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    // Loot Mode
    void SetLootMode(LootMode loot_mode);
	void SetLootMode(LootMode loot_mode, boost::unique_lock<boost::mutex>& lock);

    LootMode GetLootMode();
	LootMode GetLootMode(boost::unique_lock<boost::mutex>& lock);

    // Difficulty
    void SetDifficulty(uint16_t group_level);
	void SetDifficulty(uint16_t group_level, boost::unique_lock<boost::mutex>& lock);

    uint16_t GetDifficulty();
	uint16_t GetDifficulty(boost::unique_lock<boost::mutex>& lock);

    // Loot Master
    void SetLootMaster(uint64_t loot_master);
	void SetLootMaster(uint64_t loot_master, boost::unique_lock<boost::mutex>& lock);

    uint64_t GetLootMaster();
	uint64_t GetLootMaster(boost::unique_lock<boost::mutex>& lock);

    uint16_t GetSize();
	uint16_t GetSize(boost::unique_lock<boost::mutex>& lock);

    uint32_t GetType() const { return type; }
    const static uint32_t type = 0x47525550;

private:
	typedef swganh::ValueEvent<std::shared_ptr<Group>> GroupEvent;
    swganh::containers::NetworkVector<Member> member_list_;
    uint16_t difficulty_;
    uint64_t loot_master_;
    uint32_t loot_mode_;
};

}} // namespace swganh::object
