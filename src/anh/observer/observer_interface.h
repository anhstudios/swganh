
#ifndef ANH_OBSERVER_OBSERVER_INTERFACE_H_
#define ANH_OBSERVER_OBSERVER_INTERFACE_H_

#include "anh/byte_buffer.h"

namespace anh {
namespace observer {

    class ObserverInterface
    {
    public:
        virtual ~ObserverInterface() {}

        template<typename T>
        void Notify(const T& message)
        {
            anh::ByteBuffer buffer;
            message.serialize(buffer);
            
            Notify(buffer);
        }

        virtual uint64_t GetId() const = 0;

        /**
         * Notifies observer that the observable object has changed state.
         *
         * @param message Message containing the updated state of the observable object.
         */
        virtual void Notify(const anh::ByteBuffer& message) = 0;
    };

}}  // namespace anh::observer

#endif  // ANH_OBSERVER_OBSERVER_INTERFACE_H_