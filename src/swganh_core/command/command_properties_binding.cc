// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_properties_binding.h"

#include "swganh/scripting/python_shared_ptr.h"
#include <boost/python.hpp>

#include "command_properties.h"

namespace bp = boost::python;
using swganh::command::CommandProperties;

void swganh::command::ExportCommandProperties()
{
    bp::class_<CommandProperties, boost::noncopyable>("CommandProperties", bp::init<const CommandProperties&>() )
        .add_property("command_name", &CommandProperties::command_name)
        .add_property("default_priority", &CommandProperties::default_priority)
        .add_property("default_time", &CommandProperties::default_time)
        .add_property("character_ability", &CommandProperties::character_ability)
        .add_property("target_type", &CommandProperties::target_type)
        .add_property("call_on_target", &CommandProperties::call_on_target)
        .add_property("command_group", &CommandProperties::command_group)
        .add_property("max_range_to_target", &CommandProperties::max_range_to_target)
        .add_property("god_level", &CommandProperties::god_level)
        .add_property("add_to_combat_queue", &CommandProperties::add_to_combat_queue)
    ;
}
