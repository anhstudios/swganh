
#ifndef ANH_OBSERVER_OBSERVER_INTERFACE_H_
#define ANH_OBSERVER_OBSERVER_INTERFACE_H_

#include "anh/byte_buffer.h"

namespace anh {
namespace observer {

    namespace detail {
        template <typename Type>
        class HasObservableId
        {
           class yes { char m;};
           class no { yes m[2];};

           struct BaseMixin
           {
             uint64_t observable_id;
           };

           struct Base : public Type, public BaseMixin {};

           template <typename T, T t>  class Helper {};

           template <typename U>
           static no deduce(U*, Helper<uint64_t BaseMixin::*, &U::observable_id>* = 0);
           static yes deduce(...);

        public:
           static const bool value = sizeof(yes) == sizeof(deduce((Base*)(0)));
        };

        template<typename Type>
        class HasSerialization {
            struct No {};
            struct Yes { No no[2]; };

            struct BaseMixin {
                void serialize(anh::ByteBuffer&) const {}
                void deserialize(anh::ByteBuffer) {}
            };

            struct Base : public Type, public BaseMixin {};

            template<typename T, T t> class Helper{};

            template<typename U>
            static No deduce(U*, Helper<void (BaseMixin::*)(anh::ByteBuffer&) const, &U::serialize>* = 0, 
                Helper<void (BaseMixin::*)(anh::ByteBuffer), &U::deserialize>* = 0);
            static Yes deduce(...);

        public:
            static const bool value = sizeof(Yes) == sizeof(deduce(static_cast<Base*>(0)));
        };
    }

    class ObserverInterface
    {
    public:
        virtual ~ObserverInterface() {}

        template<typename T>
        typename std::enable_if<detail::HasObservableId<T>::value, void>::type
        Serialize(const T& message, anh::ByteBuffer& buffer)
        {
            T tmp = message;
            tmp.observable_id = GetId();

            tmp.serialize(buffer);
        }

        template<typename T>
        typename std::enable_if<!detail::HasObservableId<T>::value, void>::type
        Serialize(const T& message, anh::ByteBuffer& buffer)
        {
            message.serialize(buffer);
        }

        template<typename T>
        typename std::enable_if<detail::HasSerialization<T>::value, void>::type
        Notify(const T& message)
        {
            anh::ByteBuffer buffer;
            
            Serialize(message, buffer);

            Notify(buffer);
        }

        virtual uint64_t GetId() const = 0;

        /**
         * Notifies observer that the observable object has changed state.
         *
         * \param message Message containing the updated state of the observable object.
         */
        virtual void Notify(const anh::ByteBuffer& message) = 0;
    };

}}  // namespace anh::observer

#endif  // ANH_OBSERVER_OBSERVER_INTERFACE_H_