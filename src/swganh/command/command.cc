#include "command.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/object/creature/creature.h"

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
    , animation_crc(0)
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