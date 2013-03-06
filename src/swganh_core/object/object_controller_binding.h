// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/observer/observer_interface.h"
#include "swganh_core/messages/controllers/show_fly_text.h"
#include "swganh_core/messages/out_of_band.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;
using namespace swganh::messages;
using namespace swganh::observer;

struct ObserverInterfaceWrapper : ObserverInterface, wrapper<ObserverInterface>
{
};

void exportObjectController()
{
	//typedef void (ObserverInterface::*NotifyFunc)(const swganh::ByteBuffer& message);
    //class_<ObserverInterface, std::shared_ptr<ObserverInterface>, boost::noncopyable>("ObserverInterface", "Object that describes the observer of an object", no_init)
    //    .def("notify", NotifyFunc(&ObserverInterface::Notify), "Notifies the controller whent he object has been updated")
    //;	
}
