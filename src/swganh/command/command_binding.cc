// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_binding.h"

#include "swganh/python_shared_ptr.h"
#include <boost/python.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/object/creature/creature.h"
#include "swganh_core/object/tangible/tangible.h"
#include "swganh/scripting/utilities.h"

#include "base_swg_command.h"
#include "command_interface.h"

namespace bp = boost::python;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandCallback;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;
using swganh::scripting::ScopedGilLock;

struct BaseSwgCommandWrapper : BaseSwgCommand, bp::wrapper<BaseSwgCommand>
{
    BaseSwgCommandWrapper(
        PyObject* obj,
        swganh::app::SwganhKernel* kernel,
        const CommandProperties& properties)
        : BaseSwgCommand(kernel, properties)
    {
        ScopedGilLock lock;
        bp::detail::initialize_wrapper(obj, this);
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
                validated = this->BaseSwgCommand::Validate();
            }
        }
        catch(bp::error_already_set& /*e*/)
        {
            PyErr_Print();
        }

        return validated;
    }

    boost::optional<std::shared_ptr<CommandCallback>> Run()
    {
        boost::optional<std::shared_ptr<CommandCallback>> callback;


        ScopedGilLock lock;
        try 
        {
            bp::object result = this->get_override("run")();

            if (!result.is_none())
            {
                CommandCallback* obj_pointer = bp::extract<CommandCallback*>(result);
                callback.reset(std::shared_ptr<CommandCallback>(obj_pointer, [result] (CommandCallback*) {}));  
            }
        }
        catch(bp::error_already_set& /*e*/)
        {
            PyErr_Print();
        }

        return callback;
    }
};

class CommandCallbackWrapper : public CommandCallback, bp::wrapper<CommandCallback>
{
public:
    CommandCallbackWrapper(
        PyObject* obj,
        boost::python::object function,
        uint64_t delay_timer)
        : CommandCallback([function] () -> boost::optional<std::shared_ptr<CommandCallback>>
    {
        boost::optional<std::shared_ptr<CommandCallback>> callback;

        {
            ScopedGilLock lock;
            
            try 
            {
                bp::object result = function();
                
                if (!result.is_none())
                {
                    CommandCallback* obj_pointer = bp::extract<CommandCallback*>(result);
                    callback.reset(std::shared_ptr<CommandCallback>(obj_pointer, [result] (CommandCallback*) {}));  
                }
            }
            catch(bp::error_already_set& /*e*/)
            {
                PyErr_Print();
            }
        }

        return callback;
    }, delay_timer)
    {    
        ScopedGilLock lock;
        bp::detail::initialize_wrapper(obj, this);
    }
};

void swganh::command::ExportCommand()
{
    bp::class_<CommandCallback, std::shared_ptr<CommandCallbackWrapper>, boost::noncopyable>
        ("Callback", bp::init<bp::object, uint64_t>())
        .def("getDelayTimeInMs", &CommandCallbackWrapper::GetDelayTimeInMs)
        .def("execute", &CommandCallbackWrapper::operator())
    ;

    bp::class_<CommandInterface, boost::noncopyable>("CommandInterface", bp::no_init)
        .def("validate", bp::pure_virtual(&CommandInterface::Validate))
        .def("run", bp::pure_virtual(&CommandInterface::Run))
    ;
    
    bp::class_<BaseSwgCommand, BaseSwgCommandWrapper, bp::bases<CommandInterface>, boost::noncopyable>
        ("BaseSwgCommand", bp::init<swganh::app::SwganhKernel*, const CommandProperties&>())
        .def("validate", &BaseSwgCommandWrapper::Validate)
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
