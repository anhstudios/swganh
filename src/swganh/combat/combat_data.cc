#include "combat_data.h"

#include <anh/hash_string.h>

using namespace swganh::combat;
using namespace boost::python;

void CombatData::GetPythonData(boost::python::object global)
{
    try {
        // Exception Safe way to pull out data
        //ExtractData<float>(global, "damage_multiplier", damage_multiplier);
        if (global.contains("damage_multiplier"))
        {
            extract<float> dmg_mult_x(global["damage_multiplier"]);
            if (dmg_mult_x.check())
                damage_multiplier = dmg_mult_x();
        }
        if (global.contains("accuracy_bonus"))
        {
            extract<int> acc_bonus_x(global["accuracy_bonus"]);
            if (acc_bonus_x.check())
                accuracy_bonus = acc_bonus_x();
        }
        if (global.contains("speed_multiplier"))
        {
            extract<float> speed_multiplier_x(global["speed_multiplier"]);
            if (speed_multiplier_x.check())
                speed_multiplier = speed_multiplier_x();
        }
        if (global.contains("pool"))
        {
            extract<int> pool_x(global["pool"]);
            if (pool_x.check())
                pool = pool_x();
        }
        if (global.contains("attack_delay_chance"))
        {
            extract<int> attack_delay_x(global["attack_delay_chance"]);
            if (attack_delay_x.check())
                attack_delay_chance = attack_delay_x();
        }
        if (global.contains("state_duration"))
        {
            extract<int> state_duration_x(global["state_duration"]);
            if (state_duration_x.check())
                state_duration = state_duration_x();
        }
        /// DOTS
        if (global.contains("dot_duration"))
        {
            extract<int> dot_duration_x(global["dot_duration"]);
            if (dot_duration_x.check())
                dot_duration = dot_duration_x();
        }
        if (global.contains("dot_type"))
        {
            extract<uint64_t> dot_type_x(global["dot_type"]);
            if (dot_type_x.check())
                dot_type = dot_type_x();
        }
        if (global.contains("dot_pool"))
        {
            extract<uint8_t> dot_pool_x(global["dot_pool"]);
            if (dot_pool_x.check())
                dot_pool = dot_pool_x();
        }
        if (global.contains("dot_strength"))
        {
            extract<int> dot_strength_x(global["dot_strength"]);
            if (dot_strength_x.check())
                dot_strength = dot_strength_x();
        }
        if (global.contains("dot_potency"))
        {
            extract<float> dot_potency_x(global["dot_potency"]);
            if (dot_potency_x.check())
                dot_potency = dot_potency_x();
        }
        if (global.contains("range"))
        {
            extract<int> range_x(global["range"]);
            if (range_x.check())
                range = range_x();
        }
        if (global.contains("cone_angle"))
        {
            extract<int> cone_angle_x(global["cone_angle"]);
            if (cone_angle_x.check())
                cone_angle = cone_angle_x();
        }
        if (global.contains("area_range"))
        {
            extract<int> area_range_x(global["area_range"]);
            if (area_range_x.check())
                area_range = area_range_x();
        }
        if (global.contains("animation_string"))
        {
            extract<std::string> animation_crc_x(global["animation_string"]);
            if (animation_crc_x.check())
                animation_crc = anh::HashString(animation_crc_x());
        }

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