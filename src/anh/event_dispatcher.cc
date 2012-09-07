// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "event_dispatcher.h"

#include <algorithm>

#ifdef WIN32
#include <chrono>
using namespace std::chrono;
#else
#include <boost/chrono.hpp>
using namespace boost::chrono;
#endif

#include <boost/asio/io_service.hpp>

using namespace swganh;
using namespace std;

namespace ba = boost::asio;

BaseEvent::BaseEvent(EventType type)
: type_(type)
{}

EventType BaseEvent::Type() const
{
    return type_;
}

EventDispatcher::EventDispatcher(ba::io_service& io_service)
: io_service_(io_service)
{}

EventDispatcher::~EventDispatcher()
{
    Shutdown();
}

CallbackId EventDispatcher::Subscribe(EventType type, EventHandlerCallback callback)
{
    auto handler_id = GenerateCallbackId();

    boost::lock_guard<boost::shared_mutex> lg(event_handlers_mutex_);

    auto find_iter = event_handlers_.find(type);

    if (find_iter == end(event_handlers_))
    {
        find_iter = event_handlers_.insert(make_pair(type, EventHandlerList())).first;
    }
    
    find_iter->second.insert(std::make_pair(handler_id, move(callback)));

    return handler_id;
}

void EventDispatcher::Unsubscribe(EventType type, CallbackId identifier)
{
    boost::lock_guard<boost::shared_mutex> lg(event_handlers_mutex_);

    auto event_type_iter = event_handlers_.find(type);
    
    if (event_type_iter != end(event_handlers_))
    {
        auto type_handlers = event_type_iter->second;
        type_handlers.erase(identifier);
        type_handlers.clear();
    }
}

boost::unique_future<shared_ptr<EventInterface>> EventDispatcher::Dispatch(const shared_ptr<EventInterface>& dispatch_event)
{
    auto task = make_shared<boost::packaged_task<shared_ptr<EventInterface>>>(
        [this, dispatch_event] () -> shared_ptr<EventInterface>
    {
        InvokeCallbacks(dispatch_event);
        
        return dispatch_event;
    });

    io_service_.post([task] () {
        (*task)();
    });

    return task->get_future();
}

CallbackId EventDispatcher::GenerateCallbackId()
{
    return CallbackId(steady_clock::now().time_since_epoch().count());
}

void EventDispatcher::InvokeCallbacks(const shared_ptr<EventInterface>& dispatch_event)
{
    boost::lock_guard<boost::shared_mutex> lg(event_handlers_mutex_);

    auto event_type_iter = event_handlers_.find(dispatch_event->Type());
    
    if (event_type_iter != end(event_handlers_))
    {
        for_each(
            begin(event_type_iter->second), 
            end(event_type_iter->second), 
            [&dispatch_event] (const EventHandlerList::value_type& handler) 
        {
            handler.second(dispatch_event);
        });
    }
}

void EventDispatcher::Shutdown()
{
	boost::lock_guard<boost::shared_mutex> lg(event_handlers_mutex_);
    for (auto& item : event_handlers_)
    {
        item.second.clear();
    }
    event_handlers_.clear();
}