// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_FACTORY_CRATE_FACTORY_CRATE_H_
#define SWGANH_OBJECT_FACTORY_CRATE_FACTORY_CRATE_H_

#include "swganh/object/tangible/tangible.h"

namespace swganh {
namespace object {
namespace factory_crate {

class FactoryCrate : public swganh::object::tangible::Tangible 
{
public:
    // FCYT
    uint32_t GetType() const;
    const static uint32_t type = 0x46435954;
};

}}} // swganh::object::factory_crate

#endif //SWGANH_OBJECT_FACTORY_CRATE_FACTORY_CRATE_H_