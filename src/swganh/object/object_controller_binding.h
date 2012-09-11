// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/object/object_controller_interface.h"
#include "swganh_core/messages/controllers/show_fly_text.h"
#include "swganh_core/messages/out_of_band.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

struct ObjectControllerWrapper : ObjectControllerInterface, wrapper<ObjectControllerInterface>
{
};

void exportObjectController()
{
    typedef void (ObjectControllerInterface::*NotifyFunc)(const swganh::ByteBuffer& message);
    class_<ObjectControllerInterface, std::shared_ptr<ObjectControllerInterface>, boost::noncopyable>("ObjectController", "Object that describes the Controller of an object", no_init)
        .def("Notify", NotifyFunc(&ObjectControllerInterface::Notify), "Notifies the controller whent he object has been updated")
    ;
}
