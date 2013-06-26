// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh_core/object/weapon/weapon.h"

#include "swganh_core/messages/deltas_message.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object;
using namespace swganh::object;

Weapon::Weapon()
	: weapon_type_(RANGED)
{}
uint32_t Weapon::GetType() const 
{ 
	return Weapon::type; 
}
