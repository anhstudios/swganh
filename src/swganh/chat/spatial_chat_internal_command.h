// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_CHAT_SPATIAL_CHAT_INTERNAL_COMMAND_H_
#define SWGANH_CHAT_SPATIAL_CHAT_INTERNAL_COMMAND_H_

#include <memory>

#include "swganh/command/base_swg_command.h"

namespace swganh {
namespace chat {

    class ChatService;

    class SpatialChatInternalCommand : public command::BaseSwgCommand
    {
    public:
        SpatialChatInternalCommand(
            app::SwganhKernel* kernel,
            const command::CommandProperties& properties);

        virtual ~SpatialChatInternalCommand();

        virtual boost::optional<std::shared_ptr<command::CommandCallback>> Run();

    private:
        ChatService* chat_service_;
    };

}}

#endif  // SWGANH_COMMAND_COMMAND_INTERFACE_H_
