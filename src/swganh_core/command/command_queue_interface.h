// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>

namespace swganh {

namespace object {
    class Creature;
    class Tangible;
}  // namespace object

namespace messages {
namespace controllers {
    class CommandQueueEnqueue;
}}  // namespace messages::controllers

namespace command {

    class CommandInterface;
    struct CommandProperties;

    /**
     * A queue for managing the flow of command processing for a single controlled object.
     */
    class CommandQueueInterface
    {
    public:
        virtual ~CommandQueueInterface() {}
        
        /**
         * Enqueues a command for processing.
         *
         * @param A command to process.
         */
        virtual void EnqueueCommand(const std::shared_ptr<CommandInterface>& command) = 0;
        virtual void SetDefaultCommand(const std::shared_ptr<swganh::command::CommandInterface>& command) = 0;
        virtual void ClearDefaultCommand() = 0;
        virtual bool HasDefaultCommand() = 0;
    };

}}  // namespace swganh::command
