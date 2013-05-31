// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_WEATHER_SERVICE_BINDING_H_
#define SWGANH_WEATHER_SERVICE_BINDING_H_

#ifndef WIN32
#include <Python.h>
#endif

#include "waypoint_service_interface.h"
#include <boost/python.hpp>

using namespace swganh::waypoint;
using namespace boost::python;

void exportWaypointService()
{
	class_<WaypointServiceInterface, std::shared_ptr<WaypointServiceInterface>, boost::noncopyable>("WaypointService", "The waypoint service processes in-game waypoint features.", no_init)
        .def("createWaypoint", &WaypointServiceInterface::CreateWaypointInStorage, "Creates a new waypoint for the specified owner and returns it")
        ;
}

#endif //SWGANH_WEATHER_SERVICE_BINDING_H_