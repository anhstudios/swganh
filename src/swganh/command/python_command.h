
#ifndef SWGANH_COMMAND_PYTHON_COMMAND_H_
#define SWGANH_COMMAND_PYTHON_COMMAND_H_

#ifndef WIN32
#include <Python.h>
#endif

#include <cstdint>
#include <memory>
#include <string>

#include "command.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

namespace swganh {
namespace app {
    class SwganhKernel;
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
    static std::shared_ptr<Command> LoadCommand(const std::string& name, const std::string& full_path);
};


}}  // namespace swganh::command

#endif  // SWGANH_COMMAND_PYTHON_COMMAND_H_
