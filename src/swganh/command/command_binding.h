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

    struct CommandWrap : Command, wrapper<Command>
    {
        void Setup()
        {
            return (void)this->get_override("SetUp")();
        }
        void Run()
        {
            if (override Run = this->get_override("Run"))
                return (void)Run();
            return Command::Run();
        }
    };
    
    class_<std::vector<std::string>>("SkillMods")
        .def(vector_indexing_suite<std::vector<std::string>>())
        ;
    class_<CommandWrap, boost::noncopyable>("Command")
        .def("SetUp", &CommandWrap::Setup)
        .def("Run", &CommandWrap::Run)
        .def_readwrite("name", &CommandWrap::name)
        .def_readwrite("ability", &CommandWrap::ability)
        .def_readwrite("skill_mods", &CommandWrap::skill_mods)
        .def_readwrite("default_priority", &CommandWrap::default_priority)
        .def_readwrite("target_type", &CommandWrap::target_type)
        .def_readwrite("admin_level", &CommandWrap::admin_level)
        .def_readwrite("default_time", &CommandWrap::default_time)
        .def_readwrite("command_group", &CommandWrap::command_group)
        .def_readwrite("max_range_to_target", &CommandWrap::max_range_to_target)
        .def_readwrite("health_cost", &CommandWrap::health_cost)
        .def_readwrite("health_cost_multiplier", &CommandWrap::health_cost_multiplier)
        .def_readwrite("action_cost", &CommandWrap::name)
        .def_readwrite("action_cost_multiplier", &CommandWrap::action_cost_multiplier)
        .def_readwrite("mind_cost", &CommandWrap::name)
        .def_readwrite("mind_cost_multiplier", &CommandWrap::mind_cost_multiplier)
        .def_readwrite("force_cost", &CommandWrap::force_cost)
        .def_readwrite("force_cost_multiplier", &CommandWrap::force_cost_multiplier)
        .def_readwrite("animation_crc", &CommandWrap::animation_crc)
        .def_readwrite("required_weapon_group", &CommandWrap::required_weapon_group)
        .def_readwrite("deny_in_locomotion", &CommandWrap::deny_in_locomotion)
        .def_readwrite("allow_in_state", &CommandWrap::allow_in_states)
        .def_readwrite("allow_in_posture", &CommandWrap::allow_in_posture)
    ;
}

#endif // SWGANH_COMMAND_COMMAND_BINDING_H_