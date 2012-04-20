#ifndef SWGANH_COMMAND_COMMAND_BINDING_H_
#define SWGANH_COMMAND_COMMAND_BINDING_H_

#include "anh/python_shared_ptr.h"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "command.h"
#include "command_properties.h"

using namespace swganh::command;
using namespace boost::python;
using namespace std;

void exportCommand()
{
    class_<std::vector<std::string>>("SkillMods")
        .def(vector_indexing_suite<std::vector<std::string>>())
        ;
    class_<Command, boost::noncopyable>("Command")
        .def("SetUp", &Command::SetupCommand)
        .def("Run", &Command::RunCommand)
        .def_readwrite("name", &Command::name)
        .def_readwrite("ability", &Command::ability)
        .def_readwrite("skill_mods", &Command::skill_mods)
        .def_readwrite("default_priority", &Command::default_priority)
        .def_readwrite("target_type", &Command::target_type)
        .def_readwrite("admin_level", &Command::admin_level)
        .def_readwrite("default_time", &Command::default_time)
        .def_readwrite("command_group", &Command::command_group)
        .def_readwrite("max_range_to_target", &Command::max_range_to_target)
        .def_readwrite("health_cost", &Command::health_cost)
        .def_readwrite("health_cost_multiplier", &Command::health_cost_multiplier)
        .def_readwrite("action_cost", &Command::name)
        .def_readwrite("action_cost_multiplier", &Command::action_cost_multiplier)
        .def_readwrite("mind_cost", &Command::name)
        .def_readwrite("mind_cost_multiplier", &Command::mind_cost_multiplier)
        .def_readwrite("force_cost", &Command::force_cost)
        .def_readwrite("force_cost_multiplier", &Command::force_cost_multiplier)
        .def_readwrite("animation_crc", &Command::animation_crc)
        .def_readwrite("required_weapon_group", &Command::required_weapon_group)
        .def_readwrite("deny_in_locomotion", &Command::deny_in_locomotion)
        .def_readwrite("allow_in_state", &Command::allow_in_states)
        .def_readwrite("allow_in_posture", &Command::allow_in_posture)
    ;
}

#endif // SWGANH_COMMAND_COMMAND_BINDING_H_