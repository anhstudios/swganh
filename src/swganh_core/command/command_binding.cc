// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

#include "command_binding.h"

#include "swganh/scripting/python_shared_ptr.h"
#include <boost/python.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/object/creature/creature.h"
#include "swganh_core/object/tangible/tangible.h"
#include "swganh/scripting/utilities.h"

#include "base_swg_command.h"
#include "command_interface.h"

namespace bp = boost::python;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;
using swganh::scripting::ScopedGilLock;

struct BaseSwgCommandWrapper : BaseSwgCommand, bp::wrapper<BaseSwgCommand>
{
    BaseSwgCommandWrapper(PyObject* obj)
        : self_(obj)
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
            command = bp::call_method<std::string>(self_, "getCommandName");
        }
		catch (bp::error_already_set&)
		{
			swganh::scripting::logPythonException();
		}

        return command;
    }

    bool Validate()
    {
        bool validated = false;

        ScopedGilLock lock;
        try 
        {
            auto validate = this->get_override("validate");
            if (validate)
            {
                validated = validate();
            }
            else
            {
                validated = BaseSwgCommand::Validate();
            }
        }
		catch (bp::error_already_set&)
		{
			swganh::scripting::logPythonException();
		}

        return validated;
    }

    void Run()
    {
        ScopedGilLock lock;
        try 
        {
            if (auto py_override = this->get_override("run")) {
                py_override();
            }
        }
		catch (bp::error_already_set&)
		{
			swganh::scripting::logPythonException();
		}
    }

	void PostRun(bool success)
	{
		ScopedGilLock lock;
        try 
        {
            auto postRun = this->get_override("postRun");
            if (postRun)
            {
                postRun(success);
            }
            else
            {
                this->BaseSwgCommand::PostRun(success);
            }
        }
		catch (bp::error_already_set&)
		{
			swganh::scripting::logPythonException();
		}      
	}

private:
    PyObject* self_;
};


void swganh::command::ExportCommand()
{
    bp::class_<CommandInterface, boost::noncopyable>("CommandInterface", bp::no_init)
        .def("validate", bp::pure_virtual(&CommandInterface::Validate))
        .def("run", bp::pure_virtual(&CommandInterface::Run))
		.def("postRun", bp::pure_virtual(&CommandInterface::PostRun))
        .def("getCommandName", bp::pure_virtual(&CommandInterface::GetCommandName))
    ;
    
	bp::class_<CommandProperties>("CommandProperties", bp::no_init)
		.def_readwrite("default_time", &CommandProperties::default_time)
		.def_readwrite("ability", &CommandProperties::character_ability)
		.def_readwrite("client_effect_self", &CommandProperties::client_effect_self)
		.def_readwrite("client_effect_target", &CommandProperties::client_effect_target)
		.def_readwrite("allow_in_locomotion", &CommandProperties::allow_in_locomotion)
		.def_readwrite("allow_in_state", &CommandProperties::allow_in_state)
		.def_readwrite("target_type", &CommandProperties::target_type)
		.def_readwrite("call_on_target", &CommandProperties::call_on_target)
		.def_readwrite("command_group", &CommandProperties::command_group)
		.def_readwrite("max_range_to_target", &CommandProperties::max_range_to_target)
		.def_readwrite("god_level", &CommandProperties::god_level)
		.def_readwrite("add_to_combat_queue", &CommandProperties::add_to_combat_queue)
	;

    bp::class_<BaseSwgCommand, BaseSwgCommandWrapper, bp::bases<CommandInterface>, boost::noncopyable>
        ("BaseSwgCommand")
        .def("getCommandName", &BaseSwgCommandWrapper::GetCommandName)
        .def("validate", &BaseSwgCommandWrapper::Validate)
		.def("setup", &BaseSwgCommand::SetCommandProperties)
		.def("postRun", &BaseSwgCommand::PostRun)
        .def("getKernel", &BaseSwgCommandWrapper::GetKernel, bp::return_internal_reference<>())
        .def("getCommandName", &BaseSwgCommandWrapper::GetCommandName)
        .def("getActionCounter", &BaseSwgCommandWrapper::GetActionCounter)
        .def("getPriority", &BaseSwgCommandWrapper::GetPriority)
        .def("getCommandGroup", &BaseSwgCommandWrapper::GetCommandGroup)
		.def("getCommandString", &BaseSwgCommandWrapper::GetCommandString, bp::return_value_policy<bp::copy_const_reference>())
        .def("getTargetRequiredType", &BaseSwgCommandWrapper::GetTargetRequiredType)
        .def("getMaxRangeToTarget", &BaseSwgCommandWrapper::GetMaxRangeToTarget)
        .def("getDefaultTime", &BaseSwgCommandWrapper::GetDefaultTime)
        .def("getRequiredAbility", &BaseSwgCommandWrapper::GetRequiredAbility)
        .def("isQueuedCommand", &BaseSwgCommandWrapper::IsQueuedCommand)
        .def("getActor", &BaseSwgCommandWrapper::GetActor, bp::return_value_policy<bp::copy_const_reference>())
        .def("getTarget", &BaseSwgCommandWrapper::GetTarget, bp::return_value_policy<bp::copy_const_reference>())
		.def("getTargetCreature", &BaseSwgCommandWrapper::GetTargetCreature)
    ;
}
