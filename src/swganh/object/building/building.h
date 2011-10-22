
#ifndef SWGANH_OBJECT_BUILDING_BUILDING_H_
#define SWGANH_OBJECT_BUILDING_BUILDING_H_

#include <cstdint>
#include <string>
#include <vector>

#include "swganh/object/object.h"
#include "swganh/object/tangible/base_tangible.h"

namespace swganh {
namespace object {
namespace building {

class Building : public swganh::object::tangible::BaseTangible
{
public:
    uint32_t GetType() const;
    const static uint32_t type = 0x4255494f;

private:

};

}}}  // namespace swganh::object::building

#endif  // SWGANH_OBJECT_BUILDING_BUILDING_H_
