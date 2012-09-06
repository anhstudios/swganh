// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/object/object_controller.h"
#include "pub14_core/messages/controllers/show_fly_text.h"
#include "pub14_core/messages/out_of_band.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

struct ProsePackageWrapper : ProsePackage, wrapper<ProsePackage>
{
    ProsePackageWrapper() : ProsePackage(){}
};

void exportOutOfBand()
{
}

struct ObjectControllerWrapper : ObjectController, wrapper<ObjectController>
{
};

void exportObjectController()
{
    enum_<controllers::FlyTextColor>("FlyTextColor", "Object that describes the different colors Fly Text Can be")
        .value("RED", controllers::RED)
        .value("GREEN", controllers::GREEN)
        .value("BLUE", controllers::BLUE)
        .value("WHITE", controllers::WHITE)
        .value("MIX", controllers::MIX)
        ;
    typedef void (ObjectController::*NotifyFunc)(const anh::ByteBuffer& message);
    class_<ObjectController, std::shared_ptr<ObjectController>, boost::noncopyable>("ObjectController", "Object that describes the Controller of an object", no_init)
        .def("Notify", NotifyFunc(&ObjectController::Notify), "Notifies the controller whent he object has been updated")
    ;
}
