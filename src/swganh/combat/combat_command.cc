#include "combat_command.h"

using namespace swganh::combat;

CombatCommand::CombatCommand(swganh::command::Command& command )
    : swganh::command::Command(command)
    , damage_multiplier(0.0f)
    , accuracy_bonus(0)
    , speed_multiplier(0)
    , pool(-1)
    , attack_delay_chance(0)
    , state_duration(0)
    , dot_duration(0)
    , dot_type(0)
    , dot_pool(-1)
    , dot_strength(0)
    , dot_potency(0)
    , range(0)
    , cone_angle(0)
    , area_range(0)
    , animation_crc("")
{
}
bool CombatCommand::IsRandomPool()
{
    //if (health_hit_chance > 0)
    //    return false;
    //if (action_hit_chance > 0)
    //    return false;
    //if (mind_hit_chance > 0)
    //    return false;

    return true;
}
// Unarmed Default
const uint32_t CombatCommand::DefaultAttacks[9] = 
{
    0x99476628, 0xF5547B91, 0x3CE273EC, 0x734C00C,0x43C4FFD0, 0x56D7CC78, 0x4B41CAFB, 0x2257D06B,0x306887EB
};