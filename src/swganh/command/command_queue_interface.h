// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_COMMAND_QUEUE_INTERFACE_H_
#define SWGANH_COMMAND_COMMAND_QUEUE_INTERFACE_H_

#include <memory>

#include "command_service.h"

namespace swganh {

namespace object {
namespace creature {
    class Creature;
}  // namespace creature
namespace tangible {
    class Tangible;
}}  // namespace object::tangible

namespace messages {
namespace controllers {
    class CommandQueueEnqueue;
}}  // namespace messages::controllers

namespace command {

    struct CommandProperties;

    class CommandQueueInterface
    {
    public:
        virtual ~CommandQueueInterface() {}

        virtual void EnqueueCommand(
            const std::shared_ptr<object::creature::Creature>& actor,
            const std::shared_ptr<object::tangible::Tangible>& target,
            const swganh::messages::controllers::CommandQueueEnqueue& command,
            const CommandProperties& properties,
            const CommandHandler& handler) = 0;
    };

}}  // namespace swganh::command

#endif  // SWGANH_COMMAND_COMMAND_QUEUE_INTERFACE_H_
