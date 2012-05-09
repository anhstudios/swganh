
#include "py_command_creator.h"

#include <boost/python.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh/command/command_interface.h"
#include "swganh/command/command_properties.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"
#include "swganh/object/object_controller.h"
#include "swganh/scripting/utilities.h"

namespace bp = boost::python;
using swganh::app::SwganhKernel;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;
using swganh::command::PyCommandCreator;
using swganh::messages::controllers::CommandQueueEnqueue;
using swganh::object::ObjectController;
using swganh::scripting::ScopedGilLock;

PyCommandCreator::PyCommandCreator(std::string module_name, std::string class_name)
    : module_name_(module_name)
    , class_name_(class_name)
{
    ScopedGilLock lock;
    command_module_ = bp::import(module_name_.c_str());
}

std::unique_ptr<CommandInterface> PyCommandCreator::operator() (
    swganh::app::SwganhKernel* kernel,
    const CommandProperties& properties,
    const std::shared_ptr<ObjectController>& controller,
    const CommandQueueEnqueue& command_request)
{
    ScopedGilLock lock;

#ifdef _DEBUG
    command_module_ = bp::object(bp::handle<>(PyImport_ReloadModule(command_module_.ptr())));
#endif

    std::unique_ptr<CommandInterface> command = nullptr;

    auto new_instance = command_module_.attr(class_name_.c_str())(kernel, properties, controller, command_request);

    if (new_instance != nullptr)
    {
        command.reset(bp::extract<CommandInterface*>(new_instance));
    }

    return command;
}
