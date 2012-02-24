#include "combat_data.h"

#include <anh/hash_string.h>

using namespace swganh::combat;
using namespace boost::python;

void CombatData::GetPythonData(boost::python::object global)
{
    // Exception Safe way to pull out data
    extract<float> dmg_mult_x(global.attr("damage_multiplier"));
    if (dmg_mult_x.check())
        damage_multiplier = dmg_mult_x();
    extract<int> acc_bonus_x(global.attr("accuracy_bonus"));
    if (acc_bonus_x.check())
        accuracy_bonus = acc_bonus_x();
    extract<float> speed_multiplier_x(global.attr("speed_multiplier"));
    if (speed_multiplier_x.check())
        speed_multiplier = speed_multiplier_x();
    extract<int> pool_x(global.attr("pool"));
    if (pool_x.check())
        pool = pool_x();
    extract<int> attack_delay_x(global.attr("attack_delay_chance"));
    if (attack_delay_x.check())
        attack_delay_chance = attack_delay_x();
    extract<int> state_duration_x(global.attr("state_duration"));
    if (state_duration_x.check())
        state_duration = state_duration_x();
    /// DOTS
    extract<int> dot_duration_x(global.attr("dot_duration"));
    if (dot_duration_x.check())
        dot_duration = dot_duration_x();
    extract<uint64_t> dot_type_x(global.attr("dot_type"));
    if (dot_type_x.check())
        dot_type = dot_type_x();
    extract<uint8_t> dot_pool_x(global.attr("dot_pool"));
    if (dot_pool_x.check())
        dot_pool = dot_pool_x();
    extract<int> dot_strength_x(global.attr("dot_strength"));
    if (dot_strength_x.check())
        dot_strength = dot_strength_x();
    extract<float> dot_potency_x(global.attr("dot_potency"));
    if (dot_potency_x.check())
        dot_potency = dot_potency_x();
    extract<int> range_x(global.attr("range"));
    if (range_x.check())
        range = range_x();
    extract<int> cone_angle_x(global.attr("cone_angle"));
    if (cone_angle_x.check())
        cone_angle = cone_angle_x();
    extract<int> area_range_x(global.attr("area_range"));
    if (area_range_x.check())
        area_range = area_range_x();
    extract<std::string> animation_crc_x(global.attr("animation_string"));
    if (animation_crc_x.check())
        animation_crc = anh::HashString(animation_crc_x());


}
// Unarmed Default
const uint32_t CombatData::DefaultAttacks[9] = 
{
    0x99476628, 0xF5547B91, 0x3CE273EC, 0x734C00C,0x43C4FFD0, 0x56D7CC78, 0x4B41CAFB, 0x2257D06B,0x306887EB
};