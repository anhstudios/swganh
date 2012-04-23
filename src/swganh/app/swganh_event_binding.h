// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_APP_SWGANH_EVENT_BINDING_H_
#define SWGANH_APP_SWGANH_EVENT_BINDING_H_

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh_kernel.h"

#include "anh/python_shared_ptr.h"

#include "swganh/scripting/python_event.h"

#include <boost/python.hpp>

void exportPythonEvent()
{
    using boost::python::class_;
    using boost::python::bases;
    using boost::python::implicitly_convertible;

    class_<anh::EventInterface, std::shared_ptr<anh::EventInterface>, boost::noncopyable>("EventInterface", no_init)
        ;

    class_<anh::BaseEvent, bases<anh::EventInterface>, std::shared_ptr<anh::BaseEvent>, boost::noncopyable>("BaseEvent", no_init)
        .def("type", &swganh::scripting::PythonEvent::Type, "Returns the type of the event")
        ;

    implicitly_convertible<std::shared_ptr<anh::BaseEvent>, std::shared_ptr<anh::EventInterface>>();

    class_<swganh::scripting::PythonEvent, bases<anh::BaseEvent>, std::shared_ptr<swganh::scripting::PythonEvent>, boost::noncopyable>("PythonEvent", init<boost::python::object, float>())
        .def_readwrite("callback", &swganh::scripting::PythonEvent::callback, "callback to set that will finish the event")
        .def_readwrite("timer", &swganh::scripting::PythonEvent::timer, "timer that determines when the event will finish")
        ;

    implicitly_convertible<std::shared_ptr<swganh::scripting::PythonEvent>, std::shared_ptr<anh::BaseEvent>>();
}

#endif //SWGANH_APP_SWGANH_EVENT_BINDING_H_