// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace factory_crate {

class FactoryCrate : public swganh::object::Object 
{
public:
    // FCYT
    uint32_t GetType() const;
    const static uint32_t type = 0x46435954;
};

}}} // swganh::object::factory_crate
