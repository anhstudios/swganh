
#ifndef SWGANH_OBJECT_STATIC_STATIC_H_
#define SWGANH_OBJECT_STATIC_STATIC_H_

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace static_obj {
  
class Static : public swganh::object::Object
{
public:
    // STAO
    uint32_t GetType() const;
    const static uint32_t type = 0x5354414F;
};

}}}  // namespace swganh::object::static_obj

#endif  // SWGANH_OBJECT_STATIC_STATIC_H_
