// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <atomic>
#include <list>
#include <vector>

#include <boost/thread/mutex.hpp>

#include "swganh_core/object/object.h"
#include "swganh_core/messages/containers/network_sorted_vector.h"

namespace swganh {
namespace object {


    class Tangible;

}}  // namespace swganh::object

namespace swganh {
namespace object {


enum LootMode
{
    FREE_LOOT,
    MASTER_LOOTER,
    LOTTERY,
    RANDOM
};

struct Member
{
    Member(uint64_t object_id_, std::string name_)
        : object_id(object_id_)
        , name(name_)
    {}

    ~Member()
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint64_t>(object_id);
        message.data.write<std::string>(name);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint64_t>(object_id);
        message.data.write<std::string>(name);
    }

    bool operator==(const Member& other)
    {
        return object_id == other.object_id;
    }

    uint64_t object_id;
    std::string name;
};

class GroupFactory;
class GroupMessageBuilder;
class Group : public swganh::object::Object
{
public:
    Group();
    Group(uint32_t max_member_size);
    ~Group();

    // Group Members
    void AddGroupMember(uint64_t member, std::string name);
    void RemoveGroupMember(uint64_t member);
    swganh::messages::containers::NetworkSortedVector<Member> GetGroupMembers();
    
    // Loot Mode
    void SetLootMode(LootMode loot_mode);
    LootMode GetLootMode(void);

    // Difficulty
    void SetDifficulty(uint16_t group_level);
    uint16_t GetDifficulty(void);

    // Loot Master
    void SetLootMaster(uint64_t loot_master);
    uint64_t GetLootMaster(void);

    uint16_t GetSize();
    uint16_t GetCapacity();

    uint32_t GetType() const { return type; }
    const static uint32_t type = 0x47525550;

private:
	typedef swganh::ValueEvent<std::shared_ptr<Group>> GroupEvent;
    swganh::messages::containers::NetworkSortedVector<Member> member_list_;                     // update 6 variable 1
    std::atomic<uint16_t> difficulty_;                                                                       // update 6 variable 4
    std::atomic<uint64_t> loot_master_;                                                                      // update 6 variable 6
    std::atomic<uint32_t> loot_mode_;                                                                        // update 6 variable 7 
};

}} // namespace swganh::object
