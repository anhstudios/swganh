#ifndef SWGANH_OBJECT_FACTORY_CRATE_FACTORY_CRATE_H_
#define SWGANH_OBJECT_FACTORY_CRATE_FACTORY_CRATE_H_

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace factory_crate {

class FactoryCrate : public swganh::object::Object 
{
public:
    // FCYT
    uint32_t GetType();
    const static uint32_t type = 0x46435954;
};

}}} // swganh::object::factory_crate

#endif //SWGANH_OBJECT_FACTORY_CRATE_FACTORY_CRATE_H_