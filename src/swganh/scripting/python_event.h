// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/python/object.hpp>

#include "swganh/event_dispatcher.h"

namespace swganh {
namespace scripting {

    struct PythonEvent : swganh::BaseEvent
    {
        PythonEvent()
            : swganh::BaseEvent("PythonEvent")
        {}

        PythonEvent(boost::python::object callback_, float timer_)
            : swganh::BaseEvent("PythonEvent")
            , callback(callback_)
            , timer(timer_)
        {}

        boost::python::object callback;
        boost::python::object globals;
        float timer;
    };

}} // swganh::scripting
