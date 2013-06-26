// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "combat_data.h"

#include "swganh_core/command/base_combat_command.h"
#include "swganh/scripting/utilities.h"

using namespace swganh::combat;
using namespace boost::python;
using namespace swganh::scripting;

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