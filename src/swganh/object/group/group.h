
#ifndef SWGANH_OBJECT_GROUP_GROUP_H_
#define SWGANH_OBJECT_GROUP_GROUP_H_

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace group {

class Group : public swganh::object::Object 
{
public:
    // GRUP
    uint32_t GetType();
    const static uint32_t type = 0x47525550;
};

}}} // swganh::object::group

#endif //SWGANH_OBJECT_GROUP_GROUP_H_
