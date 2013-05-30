// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include "swganh_core/object/waypoint/waypoint.h"

#include <boost/python.hpp>

void exportWaypoint()
{
    namespace bp = boost::python;
    namespace so = swganh::object;

    bp::class_<so::Waypoint, bp::bases<so::Intangible>, std::shared_ptr<so::Waypoint>, boost::noncopyable>("Waypoint")
        .def("isActive", (bool(so::Waypoint::*)() const)&so::Waypoint::Active, "Checks if the waypoint is currently active")
        .def("activate", (void(so::Waypoint::*)())&so::Waypoint::Activate, "Activates the waypoint")
        .def("deactivate", (void(so::Waypoint::*)())&so::Waypoint::DeActivate, "Deactivates the waypoint")
;

    bp::implicitly_convertible<std::shared_ptr<so::Waypoint>, std::shared_ptr<so::Intangible>>();
}
