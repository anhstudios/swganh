// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

#include "base_combat_command_binding.h"

#include "swganh/scripting/python_shared_ptr.h"
#include <boost/python.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/object/creature/creature.h"
#include "swganh_core/object/tangible/tangible.h"
#include "swganh/scripting/utilities.h"
 
#include "base_combat_command.h"
#include "swganh_core/combat/combat_data.h"

namespace bp = boost::python;
using swganh::command::BaseCombatCommand;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;
using swganh::combat::CombatData;
using swganh::scripting::ScopedGilLock;

struct BaseCombatCommandWrapper : BaseCombatCommand, bp::wrapper<BaseCombatCommand>
{
    BaseCombatCommandWrapper(
        PyObject* obj)
        : self_(bp::handle<>(bp::borrowed(obj)))
    {
        ScopedGilLock lock;
        bp::detail::initialize_wrapper(obj, this);
    }
    

    std::string GetCommandName() const
    {
        std::string command;
        
        ScopedGilLock lock;
        try 
        {
            auto overrider = this->get_override("getCommandName");
            if (overrider)
            {
                bp::object name = overrider();
                command = bp::extract<std::string>(name);
            }
        }
		catch (bp::error_already_set&)
		{
			swganh::scripting::logPythonException();
		}

        return command;
    }

    void Run()
    {
        ScopedGilLock lock;
        try 
        {
            if (auto py_override = this->get_override("run")) {
                py_override();
            }
            
            BaseCombatCommand::Run();
        }
		catch (bp::error_already_set&)
		{
			swganh::scripting::logPythonException();
		}
    }

private:
    bp::object self_;
};

void swganh::command::ExportBaseCombatCommand()
{
    bp::class_<BaseCombatCommand, BaseCombatCommandWrapper, bp::bases<BaseSwgCommand>, boost::noncopyable>
        ("BaseCombatCommand", bp::init<>())
        .def("run", &BaseCombatCommandWrapper::Run)
		.def("postRun", &BaseCombatCommandWrapper::PostRun)
		.def_readwrite("properties", &BaseCombatCommandWrapper::combat_data)
    ;
	bp::class_<CombatData, std::shared_ptr<CombatData>, bp::bases<CommandProperties>>("CombatData", bp::no_init)
		.def_readwrite("min_damage", &CombatData::min_damage)
		.def_readwrite("max_damage", &CombatData::max_damage)
		.def_readwrite("damage_multiplier", &CombatData::damage_multiplier)
		.def_readwrite("accuracy_bonus", &CombatData::accuracy_bonus)
		.def_readwrite("weapon_accuracy", &CombatData::weapon_accuracy)
		.def_readwrite("speed_multiplier", &CombatData::speed_multiplier)
		.def_readwrite("attack_speed", &CombatData::attack_speed)
		.def_readwrite("attack_delay_chance", &CombatData::attack_delay_chance)
		.def_readwrite("dot_duration", &CombatData::dot_duration)
		.def_readwrite("dot_type", &CombatData::dot_type)
		.def_readwrite("dot_pool", &CombatData::dot_pool)
		.def_readwrite("dot_strength", &CombatData::dot_strength)
		.def_readwrite("range", &CombatData::range)
		.def_readwrite("cone_angle", &CombatData::cone_angle)
		.def_readwrite("area_range", &CombatData::area_range)
		.def_readwrite("animation_crc", &CombatData::animation_crc)
		.def_readwrite("combat_spam", &CombatData::combat_spam)
		.def_readwrite("attack_delay_chance", &CombatData::attack_delay_chance)
		.def_readwrite("health_hit_chance", &CombatData::health_hit_chance)
		.def_readwrite("action_hit_chance", &CombatData::action_hit_chance)
		.def_readwrite("mind_hit_chance", &CombatData::mind_hit_chance)
	;	
}
