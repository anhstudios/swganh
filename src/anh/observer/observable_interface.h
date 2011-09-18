
#ifndef ANH_OBSERVER_OBSERVABLE_INTERFACE_H_
#define ANH_OBSERVER_OBSERVABLE_INTERFACE_H_

#include <memory>

#include "anh/byte_buffer.h"

namespace anh {
namespace observer {

    template<typename T>
    class ObserverInterface;

    template<typename T>
    class ObservableInterface
    {
    public:
        virtual ~ObservableInterface() {}

        /**
         * Returns whether or not this observable object has any observers.
         *
         * @return True if has observers, false if not.
         */
        virtual bool HasObservers() const = 0;

        /**
         * Start receiving notifications when the observable object changes state.
         *
         * @param observer The object interested in receiving state change notifications.
         */
        virtual void Subscribe(const std::shared_ptr<ObserverInterface<T>>& observer) = 0;

        /**
         * Stop receiving state notification changes for the observable object.
         *
         * @param observer The object that no longer wants state change notifications.
         */
        virtual void Unsubscribe(const std::shared_ptr<ObserverInterface<T>>& observer) = 0;

        /**
         * Notifies subscribers that the observable object has changed state.
         *
         * @param observable The observable object that has changed state.
         */
        virtual void NotifySubscribers(const T& observable) = 0;
    };

}}  // namespace anh::observer

#endif  // ANH_OBSERVER_OBSERVABLE_INTERFACE_H_
