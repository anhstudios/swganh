// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <tuple>
#include <cstdint>

namespace swganh {
namespace command {

class CommandInterface;

}}

namespace swganh {
namespace command {

enum ERROR
{
	CANNOT_WHILE_IN_POSTURE = 1,
	INSUFFICIENT_ABILITY = 2,
	INVALID_TARGET = 3,
	OUT_OF_RANGE = 4,
	CANNOT_WHILE_IN_STATE = 5,
	GOD_LEVEL = 6
};

struct CommandFilters {

    static std::tuple<bool, uint32_t, uint32_t> TargetCheckFilter(swganh::command::CommandInterface* command);
    
    static std::tuple<bool, uint32_t, uint32_t> PostureCheckFilter(swganh::command::CommandInterface* command);
    
    static std::tuple<bool, uint32_t, uint32_t> StateCheckFilter(swganh::command::CommandInterface* command);
    
    static std::tuple<bool, uint32_t, uint32_t> AbilityCheckFilter(swganh::command::CommandInterface* command);
    
    static std::tuple<bool, uint32_t, uint32_t> CombatTargetCheckFilter(swganh::command::CommandInterface* command);

    /**
    * gets the lowest common bit from two bit masks.
    *
    * it does this by checking if each bit from the playerMask to see if it
    * matches the cmdPropertiesMask. The cmdPropertiesMask is generally imported
    * from the database, but it could be any mask you want to use. It then
    * returns the first bit that was matched.
    *
    * @param creature_mask
    *   creature_mask is the first mask, which is to be checked against the second.
    * @param cmd_properties_mask
    *   cmdPropertiesMask is the mask the first mask checks against.
    *
    * @return first_common_bit
    *   This returns the the first common bit in between the two masks.
    */
    static uint32_t  GetLowestCommonBit(uint64_t creature_mask, uint64_t cmd_properties_mask);
};

}} // swganh::command
