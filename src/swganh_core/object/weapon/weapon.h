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
    virtual uint32_t GetType() const;
    const static uint32_t type = 0x6AAC0CFE;    
};
    
}}}  // namespace swganh::object::weapon
