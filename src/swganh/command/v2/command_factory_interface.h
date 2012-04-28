// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_COMMAND_FACTORY_INTERFACE_H_
#define SWGANH_COMMAND_COMMAND_FACTORY_INTERFACE_H_

#include <functional>
#include <memory>

#include "anh/hash_string.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

namespace swganh {
namespace object {
namespace creature {
    class Creature;
}  // namespace creature

namespace tangible {
    class Tangible;
}}  // namespace object::tangible

namespace command {
namespace v2 {

    class CommandInterface;
    struct CommandProperties;
    
    typedef std::function<
        std::unique_ptr<CommandInterface> (CommandProperties* properties)
    > CommandCreator;

    /**
     */
    class CommandFactoryInterface
    {
    public:
        virtual ~CommandFactoryInterface() {}
        
        std::unique_ptr<swganh::command::v2::CommandInterface> CreateCommand(
            const std::shared_ptr<object::creature::Creature>& actor,
            const std::shared_ptr<object::tangible::Tangible>& target,
            messages::controllers::CommandQueueEnqueue message);
        
        std::unique_ptr<swganh::command::v2::CommandInterface> CreateCommand(
            const std::shared_ptr<object::creature::Creature>& actor,
            const std::shared_ptr<object::tangible::Tangible>& target,
            anh::HashString command);

        void AddCommandCreator(anh::HashString command, CommandCreator&& creator);
        
        void RemoveCommandCreator(anh::HashString command);
    };
    
}}}  // namespace swganh::command::v2

#endif  // SWGANH_COMMAND_COMMAND_FACTORY_INTERFACE_H_
