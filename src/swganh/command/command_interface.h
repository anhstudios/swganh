// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include <boost/optional.hpp>

namespace swganh {
namespace observer {
	class ObserverInterface;
}
}

namespace swganh {
namespace command {

    struct CommandProperties;

    /**
     * A general delayed callback implementation.
     */
    class CommandCallback
    {
    public:
        typedef std::function<boost::optional<std::shared_ptr<CommandCallback>> ()> CallbackFunc;

        CommandCallback(CallbackFunc&& callback_func, uint64_t delay_time_ms)
            : callback_func_(std::move(callback_func))
            , delay_time_ms_(delay_time_ms)
        {}

        uint64_t GetDelayTimeInMs()
        {
            return delay_time_ms_;
        }

        boost::optional<std::shared_ptr<CommandCallback>> operator()() 
        {
            return callback_func_();
        }

    private:
        CommandCallback();

        CallbackFunc callback_func_;
        uint64_t delay_time_ms_;
    };

    /**
     * A stateful handler that processes a command request.
     */
    class CommandInterface
    {
    public:
        virtual ~CommandInterface() {}

        /**
         * @return The controller associated with this command invocation.
         */
        virtual const std::shared_ptr<swganh::observer::ObserverInterface> GetController() const = 0;
        
        virtual void SetCommandProperties(const CommandProperties& properties) = 0;

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
        virtual boost::optional<std::shared_ptr<CommandCallback>> Run() = 0;		

		/**
         * Executes the Post Run handler for the request
         *
         * @param success if the Run command was process successfully
         */
		virtual void PostRun(bool success) = 0;
    };

}}
