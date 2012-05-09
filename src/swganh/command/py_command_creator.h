// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_PY_COMMAND_CREATOR_H_
#define SWGANH_COMMAND_PY_COMMAND_CREATOR_H_

#include <memory>
#include <string>

namespace swganh {
namespace app {
    class SwganhKernel;
}

namespace messages {
namespace controllers {
    class CommandQueueEnqueue;
}}

namespace object {
    class ObjectController;
}

namespace command {

    class CommandInterface;
    struct CommandProperties;

    class PyCommandCreator
    {
    public:
        PyCommandCreator(std::string module_name, std::string class_name);

        std::unique_ptr<CommandInterface> operator() (
            swganh::app::SwganhKernel*,
            const CommandProperties&,
            const std::shared_ptr<object::ObjectController>&,
            const swganh::messages::controllers::CommandQueueEnqueue&);

    private:
        std::string module_name_;
        std::string class_name_;
    };

}}

#endif  // SWGANH_COMMAND_PY_COMMAND_CREATOR_H_
