// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/object/tangible/tangible.h"

namespace swganh {
namespace object {
namespace weapon {
    
class Weapon : public swganh::object::tangible::Tangible
{
public:
    Weapon();

    // WEAO
    virtual uint32_t GetType() const { return Weapon::type; }
    const static uint32_t type = 0x6AAC0CFE;    
protected:
    // baselines
    virtual void GetBaseline3();
private:
    int attack_type;
    std::string weapon_effect;
    int weaponEffectIndex;
    bool certified;
    int armor_piercing;
    int point_blank_acc;
    int point_blank_range;
    int ideal_range;
    int ideal_accuracy;
    int max_range;
    int max_range_accuracy;
    int damage_type;
    float attack_speed;
    float min_damage;
    float max_damage;
    float area;
    float wounds_ratio;
    int health_attack_cost;
    int action_attack_cost;
    int mind_attack_cost;
    int force_cost;
    // weapon customization ??
    uint32_t ukn1;
    uint32_t ukn2;
    uint32_t ukn3;
    float ukn4;
    uint32_t ukn5;
    uint32_t ukn6;
    uint32_t ukn7;
    uint32_t ukn8;

};
    
}}}  // namespace swganh::object::weapon
