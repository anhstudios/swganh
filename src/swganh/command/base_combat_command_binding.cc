// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "base_combat_command_binding.h"

#include "swganh/python_shared_ptr.h"
#include <boost/python.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/object/creature/creature.h"
#include "swganh_core/object/tangible/tangible.h"
#include "swganh/scripting/utilities.h"

#include "base_combat_command.h"

namespace bp = boost::python;
using swganh::command::BaseCombatCommand;
using swganh::command::CommandCallback;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;
using swganh::scripting::ScopedGilLock;

struct BaseCombatCommandWrapper : BaseCombatCommand, bp::wrapper<BaseCombatCommand>
{
    BaseCombatCommandWrapper(
        PyObject* obj,
        swganh::app::SwganhKernel* kernel,
        const CommandProperties& properties)
        : BaseCombatCommand(kernel, properties)
        , self_(bp::handle<>(bp::borrowed(obj)))
    {
        ScopedGilLock lock;
        bp::detail::initialize_wrapper(obj, this);
    }

    boost::optional<std::shared_ptr<CommandCallback>> Run()
    {
        boost::optional<std::shared_ptr<CommandCallback>> callback;


        ScopedGilLock lock;
        try 
        {
            if (bp::override run = this->get_override("run"))
            {
                bp::object result = run();

                if (!result.is_none())
                {
                    CommandCallback* obj_pointer = bp::extract<CommandCallback*>(result);
                    callback.reset(std::shared_ptr<CommandCallback>(obj_pointer, [result] (CommandCallback*) {}));
                }
            }
            
            this->BaseCombatCommand::Run();
        }
        catch(bp::error_already_set& /*e*/)
        {
            PyErr_Print();
        }

        return callback;
    }

private:
    bp::object self_;
};

void swganh::command::ExportBaseCombatCommand()
{
    bp::class_<BaseCombatCommand, BaseCombatCommandWrapper, bp::bases<BaseSwgCommand>, boost::noncopyable>
        ("BaseCombatCommand", bp::init<swganh::app::SwganhKernel*, const CommandProperties&>())
        .def("run", &BaseCombatCommandWrapper::Run)
		.def_readwrite("min_damage", &BaseCombatCommand::min_damage)
		.def_readwrite("max_damage", &BaseCombatCommand::max_damage)
		.def_readwrite("damage_multiplier", &BaseCombatCommand::damage_multiplier)
		.def_readwrite("accuracy_bonus", &BaseCombatCommand::accuracy_bonus)
		.def_readwrite("speed_multiplier", &BaseCombatCommand::speed_multiplier)
		.def_readwrite("attack_speed", &BaseCombatCommand::attack_speed)
		.def_readwrite("attack_delay_chance", &BaseCombatCommand::attack_delay_chance)
		.def_readwrite("dot_duration", &BaseCombatCommand::dot_duration)
		.def_readwrite("dot_type", &BaseCombatCommand::dot_type)
		.def_readwrite("dot_pool", &BaseCombatCommand::dot_pool)
		.def_readwrite("dot_strength", &BaseCombatCommand::dot_strength)
		.def_readwrite("range", &BaseCombatCommand::range)
		.def_readwrite("cone_angle", &BaseCombatCommand::cone_angle)
		.def_readwrite("area_range", &BaseCombatCommand::area_range)
		.def_readwrite("animation_crc", &BaseCombatCommand::animation_crc)
		.def_readwrite("combat_spam", &BaseCombatCommand::combat_spam)
		.def_readwrite("attack_delay_chance", &BaseCombatCommand::attack_delay_chance)
		.def_readwrite("health_hit_chance", &BaseCombatCommand::health_hit_chance)
		.def_readwrite("action_hit_chance", &BaseCombatCommand::action_hit_chance)
		.def_readwrite("mind_hit_chance", &BaseCombatCommand::mind_hit_chance)
    ;
}
