/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_OBJECT_GROUP_GROUP_H_
#define SWGANH_OBJECT_GROUP_GROUP_H_

#include <atomic>
#include <list>
#include <mutex>
#include <vector>

#include "swganh/object/object.h"
#include "swganh/messages/containers/network_sorted_vector.h"

namespace swganh {
namespace object {
namespace tangible {

    class Tangible;

}}}  // namespace swganh::object::tangible

namespace swganh {
namespace object {
namespace group {

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

protected:
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline3();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline6();

private:
    friend class GroupFactory;
    mutable std::mutex group_mutex_;

    swganh::messages::containers::NetworkSortedVector<Member> member_list_;                     // update 6 variable 1
    std::atomic<uint16_t> difficulty_;                                                                       // update 6 variable 4
    std::atomic<uint64_t> loot_master_;                                                                      // update 6 variable 6
    std::atomic<uint32_t> loot_mode_;                                                                        // update 6 variable 7 
};

}}} // namespace swganh::object::group

#endif // SWGANH_OBJECT_GROUP_GROUP_H_
