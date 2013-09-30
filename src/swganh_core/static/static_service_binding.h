// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/scripting/python_shared_ptr.h"
#include "static_service_interface.h"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace swganh::statics;
using namespace boost::python;
using namespace std;

void exportStaticService()
{
    class_<ElevatorData, shared_ptr<ElevatorData>, boost::noncopyable>("ElevatorData", "A type to represent elevator data")
    .def_readwrite("dst_cell", &ElevatorData::dst_cell)
    .def_readwrite("dst_orientation", &ElevatorData::dst_orientation)
    .def_readwrite("dst_position", &ElevatorData::dst_position)
    .def_readwrite("effect_id", &ElevatorData::effect_id)
    .def_readwrite("going_down", &ElevatorData::going_down)
    ;

    class_<std::vector<shared_ptr<ElevatorData>>>("ElevatorDataList", "vector for elevator data")
    .def(vector_indexing_suite<std::vector<shared_ptr<ElevatorData>>, true>());

    class_<StaticServiceInterface, shared_ptr<StaticServiceInterface>, boost::noncopyable>("StaticService", "The static service loads and holds data that never changes.", no_init)
    .def("getElevatorDataForObject", &StaticServiceInterface::GetElevatorDataForObject, "Returns elevator data for a particular terminal id.")
    ;
}
