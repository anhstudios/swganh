// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_CHAT_SPATIAL_CHAT_INTERNAL_COMMAND_H_
#define SWGANH_CHAT_SPATIAL_CHAT_INTERNAL_COMMAND_H_

#include <memory>

#include "swganh/command/command_interface.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

namespace swganh {
namespace object {
namespace creature {
    class Creature;
}  // namespace creature
namespace tangible {
    class Tangible;
}}  // namespace object::tangible

namespace chat {

    class ChatService;

    class SpatialChatInternalCommand : public command::CommandInterface
    {
    public:
        SpatialChatInternalCommand(
            ChatService* chat_service,
            const std::shared_ptr<swganh::object::creature::Creature>& actor,
            const std::shared_ptr<swganh::object::tangible::Tangible>& target,
            const swganh::messages::controllers::CommandQueueEnqueue& command);

        virtual ~SpatialChatInternalCommand();

        virtual void Setup();

        virtual bool Validate();

        virtual void Run();

    private:
        ChatService* chat_service_;
        std::shared_ptr<swganh::object::creature::Creature> actor_;
        std::shared_ptr<swganh::object::tangible::Tangible> target_;
        swganh::messages::controllers::CommandQueueEnqueue command_;
    };

}}

#endif  // SWGANH_COMMAND_COMMAND_INTERFACE_H_
