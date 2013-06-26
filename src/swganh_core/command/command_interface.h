// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include <boost/optional.hpp>

#include "swganh/app/swganh_kernel.h"

namespace swganh {
namespace observer {
	class ObserverInterface;
}
}

namespace swganh {
namespace command {

    struct CommandProperties;
    
    /**
     * A stateful handler that processes a command request.
     */
    class CommandInterface
    {
    public:
        virtual ~CommandInterface() {}

        virtual std::string GetCommandName() const = 0;
        
        /**
         * Serves as the constructor for the CommandInterface and is called by the service that
         * invokes the command creation (ie., CommandService).
         *
         * @param kernel Kernel instance holding application data
         * @param properties Configuration data for this particular command
         */
        virtual void Initialize(swganh::app::SwganhKernel* kernel, const CommandProperties& properties) = 0;

        /**
         * @return The controller associated with this command invocation.
         */
        virtual const std::shared_ptr<swganh::observer::ObserverInterface> GetController() const = 0;
        
        /**
         * Validates the command.
         *
         * @return True if the command request is valid, false if not.
         */
        virtual bool Validate() = 0;

        /**
         * Executes the command handler for the given request.
         *
         * @return Optional callback to be invoked after a given amount of time.
         */
        virtual void Run() = 0;		

		/**
         * Executes the Post Run handler for the request
         *
         * @param success if the Run command was process successfully
         */
		virtual void PostRun(bool success) = 0;
    };

}}
