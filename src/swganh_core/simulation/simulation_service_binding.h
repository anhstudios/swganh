// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/scripting/python_shared_ptr.h"
#include "simulation_service_interface.h"

#include <boost/python.hpp>
#include <boost/python/overloads.hpp>

using namespace swganh::simulation;
using namespace swganh::object;
using namespace boost::python;
using namespace std;

boost::python::tuple CreateObjectFromTemplate(const std::string& template_name, 
			PermissionType permission_type = DEFAULT_PERMISSION, bool is_persisted=true, uint64_t object_id=0)
{
	return boost::python::make_tuple(template_name, permission_type, is_persisted, object_id);
}
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CreateOverload, CreateObjectFromTemplate, 1, 4)

void exportSimulationService()
{
    typedef std::shared_ptr<swganh::object::Object> (SimulationServiceInterface::*GetObjectByIdBinding)(uint64_t);
	typedef std::shared_ptr<swganh::object::Object> (SimulationServiceInterface::*GetObjectByCustomNameBinding)(const std::string&);
	typedef void (SimulationServiceInterface::*TransferObjectToSceneBinding)(uint64_t, const std::string&);
	typedef void (SimulationServiceInterface::*TransferObjectToSceneObjectBinding)(shared_ptr<swganh::object::Object>, const std::string&);
	typedef void (SimulationServiceInterface::*TransferObjectToSceneAndPositionBinding)(uint64_t, const std::string&, float, float, float);
	typedef void (SimulationServiceInterface::*TransferObjectToSceneObjectAndPositionBinding)(shared_ptr<swganh::object::Object>, const std::string&, float, float, float);

	enum_<PermissionType>("ContainerPermission")
		.value("DEFAULT", DEFAULT_PERMISSION)
		.value("WORLD", WORLD_PERMISSION)
		.value("STATIC_CONTAINER", STATIC_CONTAINER_PERMISSION)
		.value("WORLD_CELL", WORLD_CELL_PERMISSION)
		.value("CREATURE", CREATURE_PERMISSION)
		.value("CREATURE_CONTAINER", CREATURE_CONTAINER_PERMISSION)
		.value("RIDEABLE", RIDEABLE_PERMISSION)
		.value("NO_VIEW", NO_VIEW_PERMISSION)
		;

    class_<SimulationServiceInterface, std::shared_ptr<SimulationServiceInterface>, boost::noncopyable>("SimulationService", "The simulation service handles the current scenes aka planets", no_init)
        .def("persist", &SimulationServiceInterface::PersistObject, "persists the specified object and it's containing objects")
        .def("findObjectById", GetObjectByIdBinding(&SimulationServiceInterface::GetObjectById), "Finds an object by its id")
		.def("transfer", TransferObjectToSceneBinding(&SimulationServiceInterface::TransferObjectToScene), "transfers the object to a new scene")
		.def("transfer", TransferObjectToSceneObjectBinding(&SimulationServiceInterface::TransferObjectToScene), "transfers the object to a new scene")
		.def("transfer", TransferObjectToSceneAndPositionBinding(&SimulationServiceInterface::TransferObjectToScene), "transfers the object to a new scene and changes the position")
		.def("transfer", TransferObjectToSceneObjectAndPositionBinding(&SimulationServiceInterface::TransferObjectToScene), "transfers the object to a new scene and changes the position")
		.def("findObject", GetObjectByCustomNameBinding(&SimulationServiceInterface::GetObjectByCustomName), "finds the object by their custom name")
		.def("addObjectToScene", &SimulationServiceInterface::AddObjectToScene, "Adds the Object to the specified scene")
        .def("startScene", &SimulationServiceInterface::StartScene, "starts a scene by its label")
        .def("stopScene", &SimulationServiceInterface::StopScene, "stops a scene by the given label")
		.def("createObject", &SimulationServiceInterface::CreateObjectFromTemplate, CreateOverload(args("template_name", "permission_type", "is_persisted", "object_id"), "Creates an object of the given template"))
		.def("removeObject", &SimulationServiceInterface::RemoveObject, "Removes an object from the simulation (delete).")
		.def("removeObjectById", &SimulationServiceInterface::RemoveObjectById, "Removes an object from the simulation by id (delete).")
		;
}
