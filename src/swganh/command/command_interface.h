// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_COMMAND_INTERFACE_H_
#define SWGANH_COMMAND_COMMAND_INTERFACE_H_

#include <memory>

namespace swganh {
namespace object {
    class ObjectController;
}

namespace command {

    class CommandCallbackInterface
    {
        virtual float GetDelayTimeInMs() = 0;

        virtual void operator()() = 0;
    };

    class CommandInterface
    {
    public:
        virtual ~CommandInterface() {}

        virtual const std::shared_ptr<object::ObjectController>& GetController() const = 0;

        virtual void Setup() = 0;

        virtual bool Validate() = 0;

        virtual void Run() = 0;
    };

}}

#endif  // SWGANH_COMMAND_COMMAND_INTERFACE_H_
