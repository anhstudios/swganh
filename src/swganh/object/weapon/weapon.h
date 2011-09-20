
#ifndef SWGANH_OBJECT_WEAPON_WEAPON_H_
#define SWGANH_OBJECT_WEAPON_WEAPON_H_

#include <set>
#include "swganh/object/object.h"
#include "swganh/object/tangible/base_tangible.h"

namespace swganh {
namespace object {
namespace weapon {
    
class Weapon : public swganh::object::tangible::BaseTangible
{
public:
    Weapon();

    // WEAO
    virtual uint32_t GetType() { return Weapon::type; }
    const static uint32_t type = 0xE8000000;    
protected:
    // baselines
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline3();
private:
    
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

#endif  // SWGANH_OBJECT_WEAPON_WEAPON_H_
