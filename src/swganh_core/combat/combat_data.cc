// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "combat_data.h"

#include "swganh/command/base_combat_command.h"

using namespace swganh::combat;
using namespace boost::python;

CombatData::CombatData(boost::python::object p_object, swganh::command::CommandProperties& properties )
    : swganh::command::CommandProperties(properties)
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
    GetPythonData(p_object);
}

CombatData::CombatData(swganh::command::BaseCombatCommand* command)
    : min_damage(command->GetMinDamage())
    , max_damage(command->GetMaxDamage())
    , damage_multiplier(command->GetDamageMultiplier())
    , accuracy_bonus(command->GetAccuracyBonus())
    , speed_multiplier(command->GetSpeedMultiplier())
    , health_hit_chance(command->GetHealthHitChance())
    , action_hit_chance(command->GetActionHitChance())
    , mind_hit_chance(command->GetMindHitChance())
{}

void CombatData::GetPythonData(boost::python::object global)
{
    try {
        // Exception Safe way to pull out data
        ExtractData(global, "min_damage", min_damage);
        ExtractData(global, "max_damage", max_damage);
        ExtractData(global, "damage_multiplier", damage_multiplier);
        ExtractData(global, "accuracy_bonus", accuracy_bonus);
        ExtractData(global, "speed_multiplier", speed_multiplier);
        ExtractData(global, "pool", pool);
        ExtractData(global, "attack_delay_chance", attack_delay_chance);
        ExtractData(global, "state_duration", state_duration);
        /// DOTS
        ExtractData(global, "dot_duration", dot_duration);
        ExtractData(global, "dot_type", dot_type);
        ExtractData(global, "dot_pool", dot_pool);
        ExtractData(global, "dot_strength", dot_strength);
        ExtractData(global, "dot_potency", dot_potency);
        /// End DOTS
        ExtractData(global, "range", range);
        ExtractData(global, "cone_angle", cone_angle);
        ExtractData(global, "area_range", area_range);
        ExtractData(global, "animation_string", animation_crc);
    }
    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
    }


}
bool CombatData::IsRandomPool()
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
const uint32_t CombatData::DefaultAttacks[9] = 
{
    0x99476628, 0xF5547B91, 0x3CE273EC, 0x734C00C,0x43C4FFD0, 0x56D7CC78, 0x4B41CAFB, 0x2257D06B,0x306887EB
};