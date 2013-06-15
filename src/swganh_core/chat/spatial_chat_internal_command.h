// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>

#include "swganh_core/command/base_swg_command.h"

namespace swganh {
namespace chat {

    class ChatService;

	/**
	* An command implementation for spatial chat
	*/
    class SpatialChatInternalCommand : public swganh::command::BaseSwgCommand
    {
    public:

        virtual std::string GetCommandName() const { return "spatialchatinternal"; }

		/**
			Creates a new instance
		*/
        virtual void Initialize(
            swganh::app::SwganhKernel* kernel,
            const swganh::command::CommandProperties& properties);

		/**
		* Runs this command
		*/
        virtual void Run();

    private:
        ChatService* chat_service_;
    };

}}
