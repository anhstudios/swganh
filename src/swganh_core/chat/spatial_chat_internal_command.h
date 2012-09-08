// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>

#include "swganh/command/base_swg_command.h"

namespace swganh {
namespace chat {

    class ChatService;

    class SpatialChatInternalCommand : public swganh::command::BaseSwgCommand
    {
    public:
        SpatialChatInternalCommand(
            swganh::app::SwganhKernel* kernel,
            const swganh::command::CommandProperties& properties);

        virtual ~SpatialChatInternalCommand();

        virtual boost::optional<std::shared_ptr<swganh::command::CommandCallback>> Run();

    private:
        ChatService* chat_service_;
    };

}}
