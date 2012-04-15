#ifndef SWGANH_APP_SWGANH_EVENT_BINDING_H_
#define SWGANH_APP_SWGANH_EVENT_BINDING_H_

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh_kernel.h"

#include "anh/python_shared_ptr.h"

#include "swganh/scripting/python_event.h"

#include <boost/python.hpp>

using boost::python::class_;

void exportPythonEvent()
{
    class_<swganh::scripting::PythonEvent, std::shared_ptr<swganh::scripting::PythonEvent>, boost::noncopyable>("PythonEvent", init<anh::EventDispatcher*, boost::python::object, float>())
        .def_readwrite("callback", &swganh::scripting::PythonEvent::callback, "callback to set that will finish the event")
        .def_readwrite("timer", &swganh::scripting::PythonEvent::timer, "timer that determines when the event will finish")
        .def("trigger", &swganh::scripting::PythonEvent::Trigger, "triggers the python event")
        ;
}
#endif //SWGANH_APP_SWGANH_EVENT_BINDING_H_