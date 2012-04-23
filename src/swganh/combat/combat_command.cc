#include "combat_command.h"

using namespace swganh::combat;

CombatCommand::CombatCommand()
    : swganh::command::Command()
    , damage_multiplier(0.0f)
    , accuracy_bonus(0)
    , speed_multiplier(0)
    , pool(-1)
    , combat_spam("")
    , trail1(0)
    , trail2(0)
    , health_hit_chance(0.0f)
    , action_hit_chance(0.0f)
    , mind_hit_chance(0.0f)
    , knockdown_hit_chance(0.0f)
    , dizzy_hit_chance(0.0f)
    , blind_chance(0.0f)
    , stun_chance(0.0f)
    , intimidate_chance(0.0f)
    , posture_down_chance(0.0f)
    , extended_range(0.0f)
    , cone_angle(0.0f)
    , attack_delay_chance(0)
    , state_duration(0)
    , dot_duration(0)
    , dot_type(0)
    , dot_pool(-1)
    , dot_strength(0)
    , dot_potency(0.0f)
    , range(0.0f)
    , area_range(0.0f)
{
}

CombatCommand::CombatCommand(swganh::command::Command& command )
    : swganh::command::Command(command)
    , damage_multiplier(0.0f)
    , accuracy_bonus(0)
    , speed_multiplier(0)
    , pool(-1)
    , combat_spam("")
    , trail1(0)
    , trail2(0)
    , health_hit_chance(0.0f)
    , action_hit_chance(0.0f)
    , mind_hit_chance(0.0f)
    , knockdown_hit_chance(0.0f)
    , dizzy_hit_chance(0.0f)
    , blind_chance(0.0f)
    , stun_chance(0.0f)
    , intimidate_chance(0.0f)
    , posture_down_chance(0.0f)
    , extended_range(0.0f)
    , cone_angle(0.0f)
    , attack_delay_chance(0)
    , state_duration(0)
    , dot_duration(0)
    , dot_type(0)
    , dot_pool(-1)
    , dot_strength(0)
    , dot_potency(0.0f)
    , range(0.0f)
    , area_range(0.0f)
{
}
bool CombatCommand::IsRandomPool()
{
    if (health_hit_chance > 0)
        return false;
    if (action_hit_chance > 0)
        return false;
    if (mind_hit_chance > 0)
        return false;

    return true;
}
// Unarmed Default
const uint32_t CombatCommand::DefaultAttacks[9] = 
{
    0x99476628, 0xF5547B91, 0x3CE273EC, 0x734C00C,0x43C4FFD0, 0x56D7CC78, 0x4B41CAFB, 0x2257D06B,0x306887EB
};