// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/python_shared_ptr.h"
#include "swganh/combat/combat_service_interface.h"

#include <boost/python.hpp>

using namespace swganh::combat;
using namespace boost::python;
using namespace std;

void exportCombatService()
{
    class_<CombatServiceInterface, std::shared_ptr<CombatServiceInterface>, boost::noncopyable>("CombatServiceInterface", no_init)
        .def("incapacitate", &CombatServiceInterface::SetIncapacitated, "sets the target as incapacitated and sends out appropriate system messages")
        .def("kill", &CombatServiceInterface::SetDead, "Sets the target as dead and sends out the appropriate system messages and begins the cloning process")
        .def("end_duel", &CombatServiceInterface::EndDuel, "Ends the duel between the two players")
        .def("end_combat", &CombatServiceInterface::EndCombat, "Ends combat between the attacker and target")
        ;
}
