
#ifndef ANH_OBSERVER_OBSERVER_INTERFACE_H_
#define ANH_OBSERVER_OBSERVER_INTERFACE_H_

#include "anh/byte_buffer.h"

namespace anh {
namespace observer {

    template<typename T>
    class ObserverInterface
    {
    public:
        virtual ~ObserverInterface() {}

        /**
         * Observers receive a const reference handle to the observable object that
         * has changed state.
         *
         * @param observable Observable object that has changed state.
         */
        virtual void Notify(const T& observable) = 0;
    };

}}  // namespace anh::observer

#endif  // ANH_OBSERVER_OBSERVER_INTERFACE_H_