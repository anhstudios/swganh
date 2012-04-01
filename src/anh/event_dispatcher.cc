
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

using namespace anh;
using namespace std;

namespace ba = boost::asio;

using std::packaged_task;
using std::future;

BaseEvent::BaseEvent(EventType type)
: type_(type)
{}

EventType BaseEvent::Type() const
{
    return type_;
}

EventDispatcher::EventDispatcher(ba::io_service& io_service)
: strand_(io_service)
{}

CallbackId EventDispatcher::Subscribe(EventType type, EventHandlerCallback callback)
{
    auto find_iter = event_handlers_.find(type);

    if (find_iter == end(event_handlers_))
    {
        find_iter = event_handlers_.insert(make_pair(type, EventHandlerList())).first;
    }

    auto handler_id = GenerateCallbackId();

    find_iter->second.push_back(EventHandler(handler_id, move(callback)));

    return handler_id;
}

void EventDispatcher::Unsubscribe(EventType type, CallbackId identifier)
{
    auto event_type_iter = event_handlers_.find(type);
    
    if (event_type_iter != end(event_handlers_))
    {
        EventHandlerList tmp_list;

        copy_if(
            begin(event_type_iter->second), 
            end(event_type_iter->second), 
            begin(tmp_list),
            [identifier] (const EventHandler& handler) 
        {
            return handler.id != identifier;
        });
    }
}

future<shared_ptr<EventInterface>> EventDispatcher::Dispatch(const shared_ptr<EventInterface>& dispatch_event)
{
    auto task = make_shared<packaged_task<shared_ptr<EventInterface>()>>(
        [this, dispatch_event] () -> shared_ptr<EventInterface>
    {
        InvokeCallbacks(dispatch_event);
        
        return dispatch_event;
    });

    strand_.post([task] () {
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
    auto event_type_iter = event_handlers_.find(dispatch_event->Type());
    
    if (event_type_iter != end(event_handlers_))
    {
        for_each(
            begin(event_type_iter->second), 
            end(event_type_iter->second), 
            [&dispatch_event] (const EventHandler& handler) 
        {
            handler.callback(dispatch_event);
        });
    }
}
