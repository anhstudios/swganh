// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_COMMAND_FACTORY_INTERFACE_H_
#define SWGANH_COMMAND_COMMAND_FACTORY_INTERFACE_H_

#include <functional>
#include <memory>

#include "anh/hash_string.h"

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

    typedef std::function<std::unique_ptr<CommandInterface> (
        swganh::app::SwganhKernel*,
        const CommandProperties&,
        const std::shared_ptr<object::ObjectController>&,
        const swganh::messages::controllers::CommandQueueEnqueue&)
    > CommandCreator;

    class CommandFactoryInterface
    {
    public:
        virtual ~CommandFactoryInterface() {}

        virtual void AddCommandCreator(anh::HashString command, CommandCreator&& creator) = 0;

        virtual void RemoveCommandCreator(anh::HashString command) = 0;

        virtual std::unique_ptr<CommandInterface> CreateCommand(
            swganh::app::SwganhKernel* kernel,
            const CommandProperties& properties,
            const std::shared_ptr<object::ObjectController>& controller,
            const swganh::messages::controllers::CommandQueueEnqueue& command_request) = 0;
    };

}}

#endif  // SWGANH_COMMAND_COMMAND_FACTORY_INTERFACE_H_
