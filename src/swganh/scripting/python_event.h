// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SCRIPTING_PYTHON_EVENT_H_
#define SWGANH_SCRIPTING_PYTHON_EVENT_H_

#include <boost/python/object.hpp>

#include "anh/event_dispatcher.h"

namespace swganh {
namespace scripting {

    struct PythonEvent : anh::BaseEvent
    {
        PythonEvent()
            : anh::BaseEvent("PythonEvent")
        {}

        PythonEvent(boost::python::object callback_, float timer_)
            : anh::BaseEvent("PythonEvent")
            , callback(callback_)
            , timer(timer_)
        {}

        boost::python::object callback;
        boost::python::object globals;
        float timer;
    };

}} // swganh::scripting

#endif // SWGANH_SCRIPTING_PYTHON_EVENT_H_