// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SIMULATION_SIMULATION_SERVICE_BINDING_H_
#define SWGANH_SIMULATION_SIMULATION_SERVICE_BINDING_H_

#ifndef WIN32
#include <Python.h>
#endif

#include "anh/python_shared_ptr.h"
#include "simulation_service_interface.h"

#include <boost/python.hpp>

using namespace swganh::simulation;
using namespace boost::python;
using namespace std;

void exportSimulationService()
{
    typedef std::shared_ptr<swganh::object::Object> (SimulationServiceInterface::*GetObjectByIdBinding)(uint64_t);
	typedef std::shared_ptr<swganh::object::Object> (SimulationServiceInterface::*GetObjectByCustomNameBinding)(const std::string&);
	typedef void (SimulationServiceInterface::*TransferObjectToSceneBinding)(uint64_t, const std::string&);
	typedef void (SimulationServiceInterface::*TransferObjectToSceneObjectBinding)(shared_ptr<swganh::object::Object>, const std::string&);
	typedef void (SimulationServiceInterface::*TransferObjectToSceneAndPositionBinding)(uint64_t, const std::string&, float, float, float);
	typedef void (SimulationServiceInterface::*TransferObjectToSceneObjectAndPositionBinding)(shared_ptr<swganh::object::Object>, const std::string&, float, float, float);

    class_<SimulationServiceInterface, std::shared_ptr<SimulationServiceInterface>, boost::noncopyable>("SimulationService", "The simulation service handles the current scenes aka planets", no_init)
        .def("persist", &SimulationServiceInterface::PersistObject, "persists the specified object and it's containing objects")
        .def("findObjectById", GetObjectByIdBinding(&SimulationServiceInterface::GetObjectById), "Finds an object by its id")
		.def("transfer", TransferObjectToSceneBinding(&SimulationServiceInterface::TransferObjectToScene), "transfers the object to a new scene")
		.def("transfer", TransferObjectToSceneObjectBinding(&SimulationServiceInterface::TransferObjectToScene), "transfers the object to a new scene")
		.def("transfer", TransferObjectToSceneAndPositionBinding(&SimulationServiceInterface::TransferObjectToScene), "transfers the object to a new scene and changes the position")
		.def("transfer", TransferObjectToSceneObjectAndPositionBinding(&SimulationServiceInterface::TransferObjectToScene), "transfers the object to a new scene and changes the position")
		.def("findObject", GetObjectByCustomNameBinding(&SimulationServiceInterface::GetObjectByCustomName), "finds the object by their custom name")
        .def("startScene", &SimulationServiceInterface::StartScene, "starts a scene by its label")
        .def("stopScene", &SimulationServiceInterface::StopScene, "stops a scene by the given label")
        ;
}

#endif //SWGANH_SIMULATION_SIMULATION_SERVICE_BINDING_H_