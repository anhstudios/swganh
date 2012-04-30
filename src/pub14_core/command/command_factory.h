// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_COMMAND_COMMAND_FACTORY_H_
#define PUB14_CORE_COMMAND_COMMAND_FACTORY_H_

#include <map>

#include "anh/hash_string.h"

#include "swganh/command/v2/command_factory_interface.h"

namespace pub14_core {
namespace command {

    class CommandFactory : public swganh::command::v2::CommandFactoryInterface
    {
    public:
        CommandFactory();
        ~CommandFactory();

        std::unique_ptr<swganh::command::v2::CommandInterface> CreateCommand(
            const std::shared_ptr<swganh::object::ObjectController>& controller,
            uint32_t command_crc,
            uint64_t target_id,
            uint32_t action_counter,
            std::wstring command_options);
        
        void AddCommandCreator(anh::HashString command, swganh::command::v2::CommandCreator&& creator);
        
        void RemoveCommandCreator(anh::HashString command);

        bool HasCreatorForCommand(anh::HashString command);

    private:
        typedef std::map<
            anh::HashString,
            swganh::command::v2::CommandCreator
        > CommandCreatorMap;

        CommandCreatorMap command_creator_map_;
    };

}}  // namespace pub14_core::command

#endif  // PUB14_CORE_COMMAND_COMMAND_FACTORY_H_
