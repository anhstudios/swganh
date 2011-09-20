
#ifndef SWGANH_OBJECT_TANGIBLE_TANGIBLE_H_
#define SWGANH_OBJECT_TANGIBLE_TANGIBLE_H_

#include <set>

#include "swganh/object/object.h"
#include "swganh/object/tangible/base_tangible.h"

namespace swganh {
namespace object {
namespace tangible {
/**
*   @brief Tangible Object
*   @inherits BaseTangible
*/
class Tangible : public BaseTangible
{
public:
    Tangible();

    // TANO
    virtual uint32_t GetType() { return Tangible::type; }
    const static uint32_t type = 0x54414e4f;

private:
    
};
    
}}}  // namespace swganh::object::tangible

#endif  // SWGANH_OBJECT_TANGIBLE_TANGIBLE_H_
