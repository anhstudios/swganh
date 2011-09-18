
#ifndef SWGANH_OBJECT_WEAPON_H_
#define SWGANH_OBJECT_WEAPON_H_

#include <set>
#include "swganh/object/base_object.h"
#include "swganh/object/base_tangible.h"

namespace swganh {
namespace object {
    
class Weapon : public BaseTangible
{
public:
    Weapon();

    // WEAO
    virtual uint32_t GetType() { return Weapon::type; }
    const static uint32_t type = 0x5745414F;    

private:
    virtual void OnReliableUpdate() {}
    // weapon customization
	uint32_t ukn1;
    uint32_t ukn2;
    uint32_t ukn3;
    float ukn4;
    uint32_t ukn5;
    uint32_t ukn6;
    uint32_t ukn7;
    uint32_t ukn8;

};
    
}}  // namespace

#endif  // SWGANH_OBJECT_WEAPON_H_
