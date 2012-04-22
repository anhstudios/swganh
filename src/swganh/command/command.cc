#include "command.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/object/creature/creature.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"
#include "swganh/command/command_service.h"

using namespace swganh::command;

Command::Command(void)
    : name("")
    , name_crc(0)
    , ability("")
    , ability_crc(0)
    , allow_in_states(0)
    , skill_mods(std::vector<std::string>())
    , script_hook("")
    , fail_script_hook("")
    , admin_level(0)
    , default_time(0.0f)
    , default_priority(0)
    , command_group(0)
    , max_range_to_target(0)
    , add_to_combat_queue(0)
    , target_type(0)
    , health_cost(0)
    , health_cost_multiplier(0)
    , action_cost(0)
    , action_cost_multiplier(0)
    , mind_cost(0)
    , mind_cost_multiplier(0)
    , delay_multiplier(0)
    , force_cost(0)
    , force_cost_multiplier(0)
    , animation("")
    , required_weapon_group(0)
    , deny_in_locomotion(0)
    , allow_in_posture(0)
    {}

void Command::Setup(){}

void Command::Run(swganh::app::SwganhKernel* kernel,
	const std::shared_ptr<swganh::object::creature::Creature>& actor, // creature object
	const std::shared_ptr<swganh::object::tangible::Tangible>& target,	// target object
    const std::string& command_string)
{}

void Command::MergeCommand(Command& command)
{
    // Only overrwrite default values
    MergeValue(ability.length() == 0, ability, command.ability);
    MergeValue(skill_mods.size() == 0, skill_mods, command.skill_mods);
    MergeValue(allow_in_states == 0, allow_in_states, command.allow_in_states);
    MergeValue(allow_in_posture == 0, allow_in_posture, command.allow_in_posture);
    MergeValue(deny_in_locomotion == 0, deny_in_locomotion, command.deny_in_locomotion);
    MergeValue(admin_level == 0, admin_level, command.admin_level);
    MergeValue(default_time == 0.0f, default_time, command.default_time);
    MergeValue(default_priority == 0, default_priority, command.default_priority);
    MergeValue(command_group == 0, command_group, command.command_group);
    MergeValue(max_range_to_target == 0.0f, max_range_to_target, command.max_range_to_target);
    MergeValue(target_type == 0, target_type, command.target_type);
    MergeValue(health_cost == 0, health_cost, command.health_cost);
    MergeValue(health_cost_multiplier == 0, health_cost_multiplier, command.health_cost_multiplier);
    MergeValue(action_cost == 0, action_cost, command.action_cost);
    MergeValue(action_cost_multiplier == 0, action_cost_multiplier, command.action_cost_multiplier);
    MergeValue(mind_cost == 0, mind_cost, command.mind_cost);
    MergeValue(mind_cost_multiplier == 0, mind_cost_multiplier, command.mind_cost_multiplier);
    MergeValue(delay_multiplier == 0.0f, delay_multiplier, command.delay_multiplier);
    MergeValue(force_cost == 0, force_cost, command.force_cost);
    MergeValue(force_cost_multiplier == 0, force_cost_multiplier, command.force_cost_multiplier);
    MergeValue(animation.ident() == 0, animation, command.animation);
    MergeValue(required_weapon_group == 0, required_weapon_group, command.required_weapon_group);
}

CommandHandler Command::GetHandler()
{
    return [=](
            swganh::app::SwganhKernel* kernel,
		    const std::shared_ptr<swganh::object::creature::Creature>& actor, // creature object
		    const std::shared_ptr<swganh::object::tangible::Tangible>& target,	// target object
            const swganh::messages::controllers::CommandQueueEnqueue& cqe){
                Run(kernel, actor, target, std::string(cqe.command_options.begin(), cqe.command_options.end()));
            };
}