#ifndef SWGANH_COMBAT_COMBAT_SERVICE_BINDING_H_
#define SWGANH_COMBAT_COMBAT_SERVICE_BINDING_H_

#include "swganh/combat/combat_service.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;

namespace swganh {
namespace combat {

struct CombatServiceWrapper : CombatService, wrapper<CombatService>
{
};

void exportCombatService()
{
    class_<CombatServiceWrapper, boost::noncopyable>("CombatService", no_init)
        .def("incapacitate", &CombatServiceWrapper::SetIncapacitated, "sets the target as incapacitated and sends out appropriate system messages")
        .def("kill", &CombatServiceWrapper::SetDead, "Sets the target as dead and sends out the appropriate system messages and begins the cloning process")
        .def("end_duel", &CombatServiceWrapper::EndDuel, "Ends the duel between the two players")
        .def("end_combat", &CombatServiceWrapper::EndCombat, "Ends combat between the attacker and target")
        ;
}

}}
#endif // SWGANH_COMBAT_COMBAT_SERVICE_BINDING_H_