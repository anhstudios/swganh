// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/tangible/tangible.h"

namespace swganh {
namespace object {

class WeaponFactory;
class WeaponMessageBuilder;
enum WeaponType {
	MELEE = 0,
	RANGED
};
enum WeaponGroup {
	UNARMED = 1,
	ONEHANDED,
	TWOHANDED,
	POLEARM,
	RIFLE,
	PISTOL,
	CARBINE,
	FLAME_THROWER,
	HEAVY_ACID_BEAM,
	HEAVY_ACID_RIFLE,
	HEAVY_PARTICLE_BEAM,
	HEAVY_ROCKET_LAUNCHER,
	THROWN_WEAPON,
	SABER
};
class Weapon : public swganh::object::Tangible
{
public:
	typedef WeaponFactory FactoryType;
    typedef WeaponMessageBuilder MessageBuilderType;
    Weapon();
	WeaponType GetWeaponType() { return weapon_type_; }
	// WEAO
    virtual uint32_t GetType() const;
    const static uint32_t type = 0x6AAC0CFE;    
private:
	WeaponType weapon_type_;

};
    
}}  // namespace swganh::object
