// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_COMMAND_PROPERTIES_H_
#define SWGANH_COMMAND_COMMAND_PROPERTIES_H_

#include <cstdint>
#include <string>
#include <map>

#include "anh/hash_string.h"

namespace swganh {
namespace command {

	struct CommandProperties
    {
        anh::HashString command_name;
        uint32_t default_priority;
        float default_time;
        anh::HashString character_ability;
        uint64_t allow_in_locomotion;
        uint64_t allow_in_state;
        uint32_t target_type;
        uint32_t call_on_target;
        uint32_t command_group;
        uint32_t max_range_to_target;
        uint32_t god_level;
        uint32_t add_to_combat_queue;
    };

	typedef std::map<
		uint32_t,
		CommandProperties
	> CommandPropertiesMap;

}}  // namespace swganh::command::v2

#endif  // SWGANH_COMMAND_COMMAND_PROPERTIES_H_
