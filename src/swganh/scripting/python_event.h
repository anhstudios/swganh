#ifndef SWGANH_SCRIPTING_PYTHON_EVENT_H_
#define SWGANH_SCRIPTING_PYTHON_EVENT_H_

#include "anh/event_dispatcher.h"
#include <boost/python.hpp>

namespace swganh {
namespace scripting {

struct PythonEvent : anh::BaseEvent, public std::enable_shared_from_this<PythonEvent>
    {
        PythonEvent() : BaseEvent("PythonEvent") {}
        PythonEvent(anh::EventDispatcher* dispatcher_, boost::python::object callback_, float timer_)
            : BaseEvent("PythonEvent")
            , dispatcher(dispatcher_)
            , callback(callback_)
            , timer(timer_)
        {
        }
        void Trigger()
        {
            dispatcher->Dispatch(shared_from_this());
        }
        anh::EventDispatcher* dispatcher;
        boost::python::object callback;
        boost::python::object globals;
        float timer;
    };

}} // swganh::scripting

#endif // SWGANH_SCRIPTING_PYTHON_EVENT_H_