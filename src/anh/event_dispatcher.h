
#ifndef ANH_EVENT_DISPATCHER_H_
#define ANH_EVENT_DISPATCHER_H_

#include <cstdint>
#include <functional>
#include <future>
#include <memory>

#include <boost/asio/strand.hpp>

#ifdef WIN32
#include <concurrent_unordered_map.h>
#include <concurrent_vector.h>
#else
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_vector.h>

namespace Concurrency {
    using ::tbb::concurrent_unordered_map;
    using ::tbb::concurrent_vector;
}

#endif

#include "hash_string.h"

namespace boost {
    namespace asio {
        class io_service;
    }  // namespace asio
}  // namespace boost

namespace anh {

    class EventInterface;

    typedef uint32_t CallbackId;
    typedef HashString EventType;
    typedef std::function<void (const std::shared_ptr<EventInterface>&)> EventHandlerCallback;

    class EventInterface
    {
    public:
        virtual ~EventInterface() {}

        virtual EventType Type() const = 0;
    };

    class BaseEvent : public EventInterface
    {
    public:
        explicit BaseEvent(EventType type);

        EventType Type() const;

    private:
        EventType type_;
    };

    template<typename T>
    class ValueEvent : public BaseEvent
    {
    public:
        ValueEvent(EventType type)
            : BaseEvent(type)
        {}

        ValueEvent(EventType type, T data)
            : BaseEvent(type)
            , data_(move(data))
        {}

        const T& Get() const
        {
            return data_;
        }

        T& Get()
        {
            return data_;
        }

    private:
        T data_;
    };
    
    class EventDispatcherInterface
    {
    public:
        virtual ~EventDispatcherInterface() {}
        
        virtual CallbackId Subscribe(EventType type, EventHandlerCallback callback) = 0;
        virtual void Unsubscribe(EventType type, CallbackId identifier) = 0;

        virtual std::future<std::shared_ptr<EventInterface>> Dispatch(const std::shared_ptr<EventInterface>& dispatch_event) = 0;
    };

    class EventDispatcher : public EventDispatcherInterface
    {
    public:
        EventDispatcher(boost::asio::io_service& io_service);

        CallbackId Subscribe(EventType type, EventHandlerCallback callback);
        void Unsubscribe(EventType type, CallbackId identifier);

        std::future<std::shared_ptr<EventInterface>> Dispatch(const std::shared_ptr<EventInterface>& dispatch_event);

    private:
        struct EventHandler
        {
            EventHandler(CallbackId id, EventHandlerCallback callback)
                : id(id)
                , callback(callback)
            {}

            CallbackId id;
            EventHandlerCallback callback;
        };
        
        typedef Concurrency::concurrent_vector<EventHandler> EventHandlerList;
        
        typedef Concurrency::concurrent_unordered_map<
            EventType, 
            EventHandlerList
        > EventHandlerMap;

        CallbackId GenerateCallbackId();
        void InvokeCallbacks(const std::shared_ptr<EventInterface>& dispatch_event);
                
        EventHandlerMap event_handlers_;
        boost::asio::strand strand_;
    };

}  // namespace anh

#endif  // ANH_EVENT_DISPATCHER_H_
