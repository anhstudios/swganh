#ifndef SWGANH_COMBAT_COMBAT_SERVICE_BINDING_H_
#define SWGANH_COMBAT_COMBAT_SERVICE_BINDING_H_

#include "anh/python_shared_ptr.h"
#include "swganh/combat/combat_service.h"

#include <boost/python.hpp>

using namespace swganh::combat;
using namespace boost::python;
using namespace std;

void exportCombatService()
{
    class_<CombatService, std::shared_ptr<CombatService>, boost::noncopyable>("CombatService", no_init)
        .def("incapacitate", &CombatService::SetIncapacitated, "sets the target as incapacitated and sends out appropriate system messages")
        .def("kill", &CombatService::SetDead, "Sets the target as dead and sends out the appropriate system messages and begins the cloning process")
        .def("end_duel", &CombatService::EndDuel, "Ends the duel between the two players")
        .def("end_combat", &CombatService::EndCombat, "Ends combat between the attacker and target")
        ;
}

#endif // SWGANH_COMBAT_COMBAT_SERVICE_BINDING_H_