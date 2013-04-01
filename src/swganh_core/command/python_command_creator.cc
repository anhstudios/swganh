// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "python_command_creator.h"

#include <boost/python.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/command/command_interface.h"
#include "swganh_core/command/command_properties.h"
#include "swganh_core/messages/controllers/command_queue_enqueue.h"
#include "swganh/observer/observer_interface.h"
#include "swganh/scripting/utilities.h"

namespace bp = boost::python;
using swganh::app::SwganhKernel;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;
using swganh::command::PythonCommandCreator;
using swganh::messages::controllers::CommandQueueEnqueue;
using swganh::object::ObjectController;
using swganh::scripting::ScopedGilLock;

PythonCommandCreator::PythonCommandCreator(std::string module_name, std::string class_name)
    : module_name_(module_name)
    , class_name_(class_name)
{
    ScopedGilLock lock;

    try 
    {
        command_module_ = bp::import(module_name_.c_str());
    }
    catch (bp::error_already_set&)
	{
		swganh::scripting::logPythonException();
	}
}

std::shared_ptr<CommandInterface> PythonCommandCreator::operator() ()
{
    std::shared_ptr<CommandInterface> command = nullptr;
    
    ScopedGilLock lock;

    try 
    {

#ifdef _DEBUG
		//If our first attempt to load the module crashed, we want to import instead of reload!
		if(!command_module_.is_none())
		{
			command_module_ = bp::object(bp::handle<>(PyImport_ReloadModule(command_module_.ptr())));
		}
		else
		{
			command_module_ = bp::import(module_name_.c_str());
		}
#endif
        
        // Create an instance of the python class and store it in a shared pointer
        // so it's deletion can be properly wrapped in a GIL lock
        std::shared_ptr<bp::object> new_instance = std::shared_ptr<bp::object>(
            new bp::object(command_module_.attr(class_name_.c_str())()),
            [] (bp::object* obj) { ScopedGilLock lock; delete obj; });

        if (!new_instance->is_none())
        {
            // Extract the C++ base pointer from the python object and store both in a shared pointer
            // to ensure there are no lifetime issues (if the bp::object goes out of scope the interface
            // pointer is invalidated)
            CommandInterface* obj_pointer = bp::extract<CommandInterface*>(*new_instance);
            command.reset(obj_pointer, [new_instance] (CommandInterface*) {});
        }
    }
    catch (bp::error_already_set&)
	{
		swganh::scripting::logPythonException();
	}

    return command;
}
