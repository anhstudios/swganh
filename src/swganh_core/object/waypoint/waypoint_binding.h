// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include "swganh_core/object/waypoint/waypoint.h"

#include <boost/python.hpp>

void exportWaypoint()
{
    namespace bp = boost::python;
    namespace so = swganh::object;
    
    bp::enum_<so::Waypoint::WaypointColor>("WaypointColor", "Different possible waypoint colors")
        .value("BLUE", so::Waypoint::BLUE)
        .value("GREEN", so::Waypoint::GREEN)
        .value("ORANGE", so::Waypoint::ORANGE)
        .value("YELLOW", so::Waypoint::YELLOW)
        .value("PURPLE", so::Waypoint::PURPLE)
        .value("WHITE", so::Waypoint::WHITE)
        .value("SPACE", so::Waypoint::SPACE)
        ;

    bp::class_<so::Waypoint, bp::bases<so::Intangible>, std::shared_ptr<so::Waypoint>, boost::noncopyable>("Waypoint")
        .def("getCoordinates", (glm::vec3(so::Waypoint::*)())&so::Waypoint::GetCoordinates, "Get's waypoint's coordinates")
        .def("setCoordinates", (void(so::Waypoint::*)(const glm::vec3&))&so::Waypoint::SetCoordinates, "Set's the coordinates for the waypoint")
        .def("isActive", (bool(so::Waypoint::*)() const)&so::Waypoint::Active, "Checks if the waypoint is currently active")
        .def("activate", (void(so::Waypoint::*)())&so::Waypoint::Activate, "Activates the waypoint")
        .def("deactivate", (void(so::Waypoint::*)())&so::Waypoint::DeActivate, "Deactivates the waypoint")
        .def("getPlanet", (const std::string&(so::Waypoint::*)())&so::Waypoint::GetPlanet, bp::return_internal_reference<>(), "Get the planet")
        .def("setPlanet", (void(so::Waypoint::*)(const std::string&))&so::Waypoint::SetPlanet, "Set the planet")
        .def("getName", (std::string(so::Waypoint::*)())&so::Waypoint::GetNameStandard, "Get the waypoint name")
        .def("setName", (void(so::Waypoint::*)(const std::wstring&))&so::Waypoint::SetName, "Set's the waypoint's name")
        .def("getColor", (std::string(so::Waypoint::*)())&so::Waypoint::GetColorString, "Get's the color of the waypoint")
        .def("setColor", (void(so::Waypoint::*)(Waypoint::WaypointColor))&so::Waypoint::SetColor, "Set's the color of the waypoint")
;

    bp::implicitly_convertible<std::shared_ptr<so::Waypoint>, std::shared_ptr<so::Intangible>>();
    bp::implicitly_convertible<std::shared_ptr<so::Waypoint>, std::shared_ptr<so::Object>>();
    bp::implicitly_convertible<std::shared_ptr<so::Waypoint>, std::shared_ptr<so::ContainerInterface>>();
}
