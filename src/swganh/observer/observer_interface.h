// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/byte_buffer.h"

namespace swganh
{
namespace messages
{
	struct BaseSwgMessage;
}
}

namespace swganh {
namespace observer {

    class ObserverInterface
    {
    public:
        virtual ~ObserverInterface() {}

        virtual uint64_t GetId() const = 0;

        /**
         * Notifies observer that the observable object has changed state.
         *
         * @param message Message containing the updated state of the observable object.
         */
        virtual void Notify(swganh::messages::BaseSwgMessage* message) = 0;
    };

}}  // namespace swganh::observer
