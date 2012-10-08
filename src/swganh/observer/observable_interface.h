// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>

namespace swganh
{
namespace messages
{
	struct BaseSwgMessage;
}
}

namespace swganh {
namespace observer {

    class ObserverInterface;

    class ObservableInterface
    {
    public:
        virtual ~ObservableInterface() {}

        /**
         * Returns whether or not this observable object has any observers.
         *
         * @return True if has observers, false if not.
         */
        virtual bool HasObservers()  = 0;

        /**
         * Start receiving notifications when the observable object changes state.
         *
         * @param observer The object interested in receiving state change notifications.
         */
        virtual void Subscribe(const std::shared_ptr<ObserverInterface>& observer) = 0;

        /**
         * Stop receiving state notification changes for the observable object.
         *
         * @param observer The object that no longer wants state change notifications.
         */
        virtual void Unsubscribe(const std::shared_ptr<ObserverInterface>& observer) = 0;

        /**
         * Notifies observers that the observable object has changed state.
         *
         * @param message Message containing the updated state of the observable object.
         */
        virtual void NotifyObservers(swganh::messages::BaseSwgMessage* message) = 0;
    };

}}  // namespace swganh::observer
