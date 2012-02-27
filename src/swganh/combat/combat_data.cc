#include "combat_data.h"

using namespace swganh::combat;
using namespace boost::python;

void CombatData::GetPythonData(boost::python::object global)
{
    try {
        // Exception Safe way to pull out data
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

// Unarmed Default
const uint32_t CombatData::DefaultAttacks[9] = 
{
    0x99476628, 0xF5547B91, 0x3CE273EC, 0x734C00C,0x43C4FFD0, 0x56D7CC78, 0x4B41CAFB, 0x2257D06B,0x306887EB
};