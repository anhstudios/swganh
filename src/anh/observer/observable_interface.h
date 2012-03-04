
#ifndef ANH_OBSERVER_OBSERVABLE_INTERFACE_H_
#define ANH_OBSERVER_OBSERVABLE_INTERFACE_H_

#include <memory>

#include "anh/byte_buffer.h"

namespace anh {
namespace observer {

    class ObserverInterface;

    class ObservableInterface
    {
    public:
        virtual ~ObservableInterface() {}

        /**
         * Returns whether or not this observable object has any observers.
         *
         * \return True if has observers, false if not.
         */
        virtual bool HasObservers()  = 0;

        /**
         * Start receiving notifications when the observable object changes state.
         *
         * \param observer The object interested in receiving state change notifications.
         */
        virtual void Subscribe(const std::shared_ptr<ObserverInterface>& observer) = 0;

        /**
         * Stop receiving state notification changes for the observable object.
         *
         * \param observer The object that no longer wants state change notifications.
         */
        virtual void Unsubscribe(const std::shared_ptr<ObserverInterface>& observer) = 0;

        /**
         * Notifies observers that the observable object has changed state.
         *
         * \param message Message containing the updated state of the observable object.
         */
        virtual void NotifyObservers(const anh::ByteBuffer& message) = 0;
    };

}}  // namespace anh::observer

#endif  // ANH_OBSERVER_OBSERVABLE_INTERFACE_H_
