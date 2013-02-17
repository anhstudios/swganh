// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/python_shared_ptr.h"
#include "spawn_service_interface.h"
#include "fsm_state_interface.h"
#include "fsm_bundle_interface.h"
#include "finite_state_machine_interface.h"

#include <boost/python.hpp>

using namespace swganh::spawn;
namespace bp = boost::python;
using namespace std;

void exportSpawnService()
{
    bp::class_<SpawnServiceInterface>("SpawnService", bp::no_init)
		.def("CreateFsm", SpawnServiceInterface::CreateFsm, "Creates a new finite state machine with the given name")
		.def("RemoveFsm", SpawnServiceInterface::UnregisterFsm, "Removes a finite state machine with the given name")
		.def("StartManagingObject", SpawnServiceInterface::StartManagingObject, "Has the machine with the given name manage the given object.")
		.def("StopManagingObject", SpawnServiceInterface::StopManagingObject, "Has the AI sub system stop managing the given object.")
		;

	bp::class_<FsmStateInterface, std::shared_ptr<FsmStateInterface>>("State", "The base unit of a finite state machine.")
		;

	bp::class_<FsmBundleInterface, std::shared_ptr<FsmBundleInterface>>("Bundle", "The context of a particular object in the FSM.")
		;

	bp::class_<FiniteStateMachineInterface, std::shared_ptr<FiniteStateMachineInterface>>("Machine", "A basic Finite State Machine.")
		;

}
