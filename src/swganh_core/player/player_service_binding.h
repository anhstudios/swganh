// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/scripting/python_shared_ptr.h"
#include "player_service_interface.h"
#include "swganh_core/object/creature/creature.h"

#include <boost/python.hpp>

using namespace swganh::player;
using namespace boost::python;
using namespace std;

void OpenContainer(PlayerServiceInterface* player_service, std::shared_ptr<swganh::object::Object> player, std::shared_ptr<swganh::object::Object> object)
{
	player_service->OpenContainer(static_pointer_cast<swganh::object::Creature>(player), object);
}
void exportPlayerService()
{
	class_<PlayerServiceInterface, shared_ptr<PlayerServiceInterface>, boost::noncopyable>("PlayerService", "The player service handles player specific actions", no_init)
        .def("sendTip", &PlayerServiceInterface::SendTip, "Sends a bank tip from the given player to the given player of the alloted amount")
        .def("openContainer", OpenContainer, "Opens the given container for a player")
		.def("hasCalledMount", &PlayerServiceInterface::HasCalledMount)
		.def("storeAllCalledMounts", &PlayerServiceInterface::StoreAllCalledMounts)
		.def("storeAllCalledObjects", &PlayerServiceInterface::StoreAllCalledObjects)
        ;
}
