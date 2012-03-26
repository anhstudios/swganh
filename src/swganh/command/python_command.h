
#ifndef SWGANH_COMMAND_PYTHON_COMMAND_H_
#define SWGANH_COMMAND_PYTHON_COMMAND_H_

#include <cstdint>
#include <memory>
#include <string>

#include "swganh/scripting/python_script.h"

#include "command_properties.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

namespace anh {
namespace app {
    class KernelInterface;
}}

namespace swganh {
namespace object {
	class Object;
namespace tangible {
    class Tangible;
}
namespace creature {
	class Creature;
}}
namespace command {

class PythonCommand
{
public:
    PythonCommand(const CommandProperties& command_properties, const std::string& script_path);

    void operator()(anh::app::KernelInterface* kernel, std::shared_ptr<swganh::object::creature::Creature> actor, std::shared_ptr<swganh::object::tangible::Tangible> target, const swganh::messages::controllers::CommandQueueEnqueue& command_queue_message);
private:
    swganh::scripting::PythonScript script_;
};


}}  // namespace swganh::command

#endif  // SWGANH_COMMAND_PYTHON_COMMAND_H_
