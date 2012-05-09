
#include "py_command_creator.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/command/command_interface.h"
#include "swganh/command/command_properties.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"
#include "swganh/object/object_controller.h"

using swganh::app::SwganhKernel;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;
using swganh::command::PyCommandCreator;
using swganh::messages::controllers::CommandQueueEnqueue;
using swganh::object::ObjectController;

PyCommandCreator::PyCommandCreator(std::string module_name, std::string class_name)
    : module_name_(module_name)
    , class_name_(class_name)
{}

std::unique_ptr<CommandInterface> PyCommandCreator::operator() (
    swganh::app::SwganhKernel*,
    const CommandProperties&,
    const std::shared_ptr<ObjectController>&,
    const CommandQueueEnqueue&)
{
    return nullptr;
}
