
#ifndef SWGANH_OBJECT_OBJECT_H_
#define SWGANH_OBJECT_OBJECT_H_

#include "anh/observer/observable_interface.h"

namespace swganh {
namespace object {

    class Object : public anh::observer::ObservableInterface<Object>
    {
    public:        
        /**
         * Start receiving notifications when the observable object changes state.
         *
         * @param observer The object interested in receiving state change notifications.
         */
        void Subscribe(const std::shared_ptr<anh::observer::ObserverInterface<Object>>& observer);

        /**
         * Stop receiving state notification changes for the observable object.
         *
         * @param observer The object that no longer wants state change notifications.
         */
        void Unsubscribe(const std::shared_ptr<anh::observer::ObserverInterface<Object>>& observer);

        /**
         * Notifies subscribers that the observable object has changed state.
         *
         * @param observable The observable object that has changed state.s
         */
        void NotifySubscribers(const Object& observable);

    private:
        typedef std::vector<
            std::shared_ptr<anh::observer::ObserverInterface<Object>>
        > ObserverContainer;

        ObserverContainer observers_;
    };

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_OBJECT_H_
