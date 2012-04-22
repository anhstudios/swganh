#ifndef SWGANH_COMMAND_COMMAND_BINDING_H_
#define SWGANH_COMMAND_COMMAND_BINDING_H_

#include "anh/python_shared_ptr.h"

#include <boost/python.hpp>
#include "combat_command.h"
#include "swganh/command/command.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/object/creature/creature.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

using namespace swganh::command;
using namespace swganh::combat;
using namespace boost::python;
using namespace std;

struct CombatCommandWrap : CombatCommand, wrapper<CombatCommand>
{
    void Setup()
    {
        if (override Run = this->get_override("SetUp"))
            return (void)this->get_override("SetUp")();
        return CombatCommand::Setup();
    }
    void Run(swganh::app::SwganhKernel* kernel,
	const std::shared_ptr<swganh::object::creature::Creature>& actor, // creature object
	const std::shared_ptr<swganh::object::tangible::Tangible>& target,	// target object
    const std::string& command_string)
    {
        if (override Run = this->get_override("Run"))
            return (void)Run(boost::python::ptr(kernel), actor, target, command_string);
        return CombatCommand::Run(kernel, actor, target, command_string);
    }
};

void exportCombatCommand()
{
    class_<CombatCommandWrap, bases<Command>, std::shared_ptr<CombatCommand>, boost::noncopyable>("CombatCommand", init<Command&>())
        .def_readwrite("min_damage", &CombatCommandWrap::min_damage, "Used for combat to determine minimum damage this ability should do")
        .def_readwrite("max_damage", &CombatCommandWrap::max_damage, "Used for combat to determine maximum damage this ability should do")
        .def_readwrite("damage_multiplier", &CombatCommandWrap::damage_multiplier, "used to determine the damage multiplier this command does")
        .def_readwrite("speed_multiplier", &CombatCommandWrap::speed_multiplier, "used to determine the speed multiplier this command does")
        .def_readwrite("pool", &CombatCommandWrap::pool, "used to determine which pool this command will hit")
        .def_readwrite("combat_spam", &CombatCommandWrap::combat_spam, "used to determine which message will be sent to the client when a combat action occurs")
        .def_readwrite("trail1", &CombatCommandWrap::trail1, "used to show the primary trail animation for this command")
        .def_readwrite("trail2", &CombatCommandWrap::trail2, "used to show the secondary trail animation for this command")
        .def_readwrite("health_hit_chance", &CombatCommandWrap::health_hit_chance, "used to determine how often to hit the health pool by using this command")
        .def_readwrite("action_hit_chance", &CombatCommandWrap::action_hit_chance, "used to determine how often to hit the action pool by using this command")
        .def_readwrite("mind_hit_chance", &CombatCommandWrap::mind_hit_chance, "used to determine how often to hit the mind pool by using this command")
        .def_readwrite("knockdown_hit_chance", &CombatCommandWrap::knockdown_hit_chance, "used to determine what percentage this command will knockdown a target")
        .def_readwrite("dizzy_hit_chance", &CombatCommandWrap::dizzy_hit_chance, "used to determine what percentage this command will dizzy a target")
        .def_readwrite("blind_chance", &CombatCommandWrap::blind_chance, "used to determine what percentage this command will blind a target")
        .def_readwrite("stun_chance", &CombatCommandWrap::stun_chance, "used to determine what percentage this command will stun a target")
        .def_readwrite("intimidate_chance", &CombatCommandWrap::intimidate_chance, "used to determine what percentage this command will intimidate a target")
        .def_readwrite("posture_down_chance", &CombatCommandWrap::posture_down_chance, "used to determine what percentage this command will cause a posture down to a target")
        .def_readwrite("extended_range", &CombatCommandWrap::extended_range, "used to determine what the extended range on this command is")
        .def_readwrite("cone_angle", &CombatCommandWrap::cone_angle, "used to determine what the cone angle hit is on this command")
        .def_readwrite("range", &CombatCommandWrap::range, "used to determine what the base range is for this command")
        .def_readwrite("area_range", &CombatCommandWrap::area_range, "used to determine what the AOE range is for this command (useful only for AOE attacks)")
        .def_readwrite("attack_delay_chance", &CombatCommandWrap::attack_delay_chance, "used to determine what the attack delay chance is for this command ")
        .def_readwrite("state_duration", &CombatCommandWrap::state_duration, "used to determine how long the negative state will last")
        .def_readwrite("dot_duration", &CombatCommandWrap::dot_duration, "used to determine how long the negative damage over time (DOT) will last")
        .def_readwrite("dot_type", &CombatCommandWrap::dot_type, "used to determine what type of damage over time effect to apply")
        .def_readwrite("dot_pool", &CombatCommandWrap::dot_pool, "used to determine which pool the DOT will affect")
        .def_readwrite("dot_potency", &CombatCommandWrap::dot_potency, "used to determine how potent the damage over time ability will be")
    ;
}

#endif // SWGANH_COMMAND_COMMAND_BINDING_H_