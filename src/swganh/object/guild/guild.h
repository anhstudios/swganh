
#ifndef SWGANH_OBJECT_GUILD_GUILD_H_
#define SWGANH_OBJECT_GUILD_GUILD_H_

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace guild {

class Guild : public swganh::object::Object 
{
public:
    // GILD
    uint32_t GetType() const;
    const static uint32_t type = 0x47494C44;
};

}}} // swganh::object::guild

#endif //SWGANH_OBJECT_GUILD_GUILD_H_
