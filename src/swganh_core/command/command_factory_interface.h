// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <functional>
#include <memory>

#include "swganh/hash_string.h"

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

    typedef std::function<
        std::shared_ptr<CommandInterface> ()
    > CommandCreator;

    /**
     * Used by the CommandService to create command instances for incoming requests.
     */
    class CommandFactoryInterface
    {
    public:
        virtual ~CommandFactoryInterface() {}

        virtual void Initialize(swganh::app::SwganhKernel* kernel) = 0;

        virtual bool IsRegistered(swganh::HashString command) = 0;

        /**
         * Adds a creator for a given command type. Only the most recently added creator for
         * a type is used.
         *
         * @param command The name/crc of the command.
         * @param creator The creator associated with the specified command.
         */
        virtual void AddCommandCreator(swganh::HashString command, CommandCreator creator) = 0;

        /**
         * Removes the creator for a given command type if one is set.
         *
         * @param command The name/crc of the command.
         */
        virtual void RemoveCommandCreator(swganh::HashString command) = 0;

        /**
         * Creates a new instance of a command.
         *
         * @param command The name/crc of the command.
         * @return New instance of the specified command.
         */
        virtual std::shared_ptr<CommandInterface> CreateCommand(swganh::HashString command) = 0;
    };

}}
