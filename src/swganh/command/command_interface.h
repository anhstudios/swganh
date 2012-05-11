// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_COMMAND_INTERFACE_H_
#define SWGANH_COMMAND_COMMAND_INTERFACE_H_

#include <functional>
#include <memory>

#include <boost/optional.hpp>

namespace swganh {
namespace object {
    class ObjectController;
}

namespace command {

    class CommandCallback
    {
    public:
        typedef std::function<boost::optional<std::shared_ptr<CommandCallback>> ()> CallbackFunc;

        CommandCallback(CallbackFunc&& callback_func, float delay_time_ms)
            : callback_func_(std::move(callback_func))
            , delay_time_ms_(delay_time_ms)
        {}

        float GetDelayTimeInMs()
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
        float delay_time_ms_;
    };

    class CommandInterface
    {
    public:
        virtual ~CommandInterface() {}

        virtual const std::shared_ptr<object::ObjectController>& GetController() const = 0;

        virtual void Setup() = 0;

        virtual bool Validate() = 0;

        virtual boost::optional<std::shared_ptr<CommandCallback>> Run() = 0;
    };

}}

#endif  // SWGANH_COMMAND_COMMAND_INTERFACE_H_
