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

#ifndef SWGANH_OBJECT_GROUP_H_
#define SWGANH_OBJECT_GROUP_H_

#include "swganh/object/universe.h"

#include <map>
#include <stack>

namespace swganh {
namespace object {

// FORWARD DECLARATIONS
class Tangible;

class Group : public Universe
{
public:
    enum LOOT_MODE
    {
        FREE_LOOT,
        MASTER_LOOTER,
        LOTTERY,
        RANDOM
    };

    Group();
    ~Group();

    /**
     * Adds a new member to the group.
     */
    void AddGroupMember(std::shared_ptr<Tangible> member);

    /**
     * Removes a member from the group.
     */
    void RemoveGroupMember(std::shared_ptr<Tangible> member);
    
    /**
     * Updates the loot mode.
     */
    void SetLootMode(LOOT_MODE loot_mode);

    /**
     * Sets the group difficulty level.
     */
    void SetGroupLevel(uint16_t group_level);

    /**
     * Sets the Master Looter of the group (by id).
     */
    void SetMasterLooter(uint64_t master_looter);

    virtual uint32_t GetType() { return type; }
    const static uint32_t type = 0x47525550;

private:

    uint32_t member_list_counter_;                                  // update 6 variable 1
    std::map<uint16_t, std::shared_ptr<Tangible>> member_list_;     // update 6 variable 1
    uint16_t group_level_;                                          // update 6 variable 4
    uint64_t master_looter_;                                        // update 6 variable 6
    uint32_t loot_mode_;                                            // update 6 variable 7 

    uint16_t last_member_index_;
    std::stack<uint16_t> member_index_free_list_;

    /**
     * Returns the next available member index in the group.
     */
    uint16_t GetNextMemberIndex_();
};

}} // namespace

#endif // SWGANH_OBJECT_UNIVERSE_GROUP_H_