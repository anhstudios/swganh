// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <unordered_map>
#include <vector>

#include <boost/asio/strand.hpp>
#include <boost/thread/future.hpp>
#include <boost/thread/shared_mutex.hpp>

#include "hash_string.h"

namespace boost {
namespace asio {
    class io_service;
}}  // namespace boost::asio

namespace swganh {

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
            , data_(std::move(data))
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

        virtual boost::unique_future<std::shared_ptr<EventInterface>> Dispatch(const std::shared_ptr<EventInterface>& dispatch_event) = 0;
		// Shutdown the event dispatcher and stops dispatching events immediately
		virtual void Shutdown() = 0;
    };

    class EventDispatcher : public EventDispatcherInterface
    {
    public:
        explicit EventDispatcher(boost::asio::io_service& io_service);
        ~EventDispatcher();

        CallbackId Subscribe(EventType type, EventHandlerCallback callback);
        void Unsubscribe(EventType type, CallbackId identifier);

        boost::unique_future<std::shared_ptr<EventInterface>> Dispatch(const std::shared_ptr<EventInterface>& dispatch_event);

		void Shutdown();

    private:
        typedef std::unordered_map<CallbackId, EventHandlerCallback> EventHandlerList;
        
        typedef std::unordered_map<
            EventType, 
            EventHandlerList
        > EventHandlerMap;

        CallbackId GenerateCallbackId();
        void InvokeCallbacks(const std::shared_ptr<EventInterface>& dispatch_event);

        boost::shared_mutex event_handlers_mutex_;
        EventHandlerMap event_handlers_;
        boost::asio::io_service& io_service_;
    };

}  // namespace swganh
