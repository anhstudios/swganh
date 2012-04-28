// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_COMMAND_COMMAND_FACTORY_H_
#define PUB14_CORE_COMMAND_COMMAND_FACTORY_H_

#include "swganh/command/v2/command_factory_interface.h"

namespace pub14_core {
namespace command {

    class CommandFactory : public swganh::command::v2::CommandFactoryInterface
    {
    public:
        std::unique_ptr<swganh::command::v2::CommandInterface> CreateCommand(
            const std::shared_ptr<swganh::object::creature::Creature>& actor,
            const std::shared_ptr<swganh::object::tangible::Tangible>& target,
            swganh::messages::controllers::CommandQueueEnqueue message);
        
        std::unique_ptr<swganh::command::v2::CommandInterface> CreateCommand(
            const std::shared_ptr<swganh::object::creature::Creature>& actor,
            const std::shared_ptr<swganh::object::tangible::Tangible>& target,
            anh::HashString command);

        void AddCommandCreator(anh::HashString command, swganh::command::v2::CommandCreator&& creator);
        
        void RemoveCommandCreator(anh::HashString command);        
    };

}}  // namespace pub14_core::command

#endif  // PUB14_CORE_COMMAND_COMMAND_FACTORY_H_
