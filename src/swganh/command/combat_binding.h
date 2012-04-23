#ifndef SWGANH_COMMAND_COMBATCOMMAND_BINDING_H_
#define SWGANH_COMMAND_COMBATCOMMAND_BINDING_H_

#ifndef WIN32
#include <Python.h>
#endif

#include "anh/python_shared_ptr.h"

#include <boost/python.hpp>
#include "swganh/combat/combat_command.h"
#include "swganh/command/command.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/object/creature/creature.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

using namespace swganh::command;
using namespace swganh::combat;
using namespace boost::python;
using namespace std;

void exportCombatCommand()
{
    class_<CombatCommand, bases<CommandWrap>, std::shared_ptr<CombatCommand>, boost::noncopyable>("CombatCommand")
        .def_readwrite("min_damage", &CombatCommand::min_damage, "Used for combat to determine minimum damage this ability should do")
        .def_readwrite("max_damage", &CombatCommand::max_damage, "Used for combat to determine maximum damage this ability should do")
        .def_readwrite("damage_multiplier", &CombatCommand::damage_multiplier, "used to determine the damage multiplier this command does")
        .def_readwrite("speed_multiplier", &CombatCommand::speed_multiplier, "used to determine the speed multiplier this command does")
        .def_readwrite("pool", &CombatCommand::pool, "used to determine which pool this command will hit")
        .def_readwrite("combat_spam", &CombatCommand::combat_spam, "used to determine which message will be sent to the client when a combat action occurs")
        .def_readwrite("trail1", &CombatCommand::trail1, "used to show the primary trail animation for this command")
        .def_readwrite("trail2", &CombatCommand::trail2, "used to show the secondary trail animation for this command")
        .def_readwrite("health_hit_chance", &CombatCommand::health_hit_chance, "used to determine how often to hit the health pool by using this command")
        .def_readwrite("action_hit_chance", &CombatCommand::action_hit_chance, "used to determine how often to hit the action pool by using this command")
        .def_readwrite("mind_hit_chance", &CombatCommand::mind_hit_chance, "used to determine how often to hit the mind pool by using this command")
        .def_readwrite("knockdown_hit_chance", &CombatCommand::knockdown_hit_chance, "used to determine what percentage this command will knockdown a target")
        .def_readwrite("dizzy_hit_chance", &CombatCommand::dizzy_hit_chance, "used to determine what percentage this command will dizzy a target")
        .def_readwrite("blind_chance", &CombatCommand::blind_chance, "used to determine what percentage this command will blind a target")
        .def_readwrite("stun_chance", &CombatCommand::stun_chance, "used to determine what percentage this command will stun a target")
        .def_readwrite("intimidate_chance", &CombatCommand::intimidate_chance, "used to determine what percentage this command will intimidate a target")
        .def_readwrite("posture_down_chance", &CombatCommand::posture_down_chance, "used to determine what percentage this command will cause a posture down to a target")
        .def_readwrite("extended_range", &CombatCommand::extended_range, "used to determine what the extended range on this command is")
        .def_readwrite("cone_angle", &CombatCommand::cone_angle, "used to determine what the cone angle hit is on this command")
        .def_readwrite("range", &CombatCommand::range, "used to determine what the base range is for this command")
        .def_readwrite("area_range", &CombatCommand::area_range, "used to determine what the AOE range is for this command (useful only for AOE attacks)")
        .def_readwrite("attack_delay_chance", &CombatCommand::attack_delay_chance, "used to determine what the attack delay chance is for this command ")
        .def_readwrite("state_duration", &CombatCommand::state_duration, "used to determine how long the negative state will last")
        .def_readwrite("dot_duration", &CombatCommand::dot_duration, "used to determine how long the negative damage over time (DOT) will last")
        .def_readwrite("dot_type", &CombatCommand::dot_type, "used to determine what type of damage over time effect to apply")
        .def_readwrite("dot_pool", &CombatCommand::dot_pool, "used to determine which pool the DOT will affect")
        .def_readwrite("dot_potency", &CombatCommand::dot_potency, "used to determine how potent the damage over time ability will be")
    ;
}

#endif // SWGANH_COMMAND_COMBATCOMMAND_BINDING_H_