// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_H_
#define SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_H_

#include <cstdint>
#include <string>

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace intangible {

class IntangibleFactory;
class IntangibleMessageBuilder;
    
class Intangible : public swganh::object::Object
{
public:
    typedef IntangibleFactory FactoryType;
    typedef IntangibleMessageBuilder MessageBuilderType;

    // ITNO
    /**
     * @return The type of this object instance.
     */
    virtual uint32_t GetType() const { return Intangible::type; }
    const static uint32_t type = 0x494E534F;
    
	uint32_t GetGenericInt();
	void SetGenericInt(uint32_t generic_int);

protected:
	uint32_t generic_int_;
};
    
}}}  // namespace swganh::object::intangible

#endif  // SWGANH_OBJECT_BASE_OBJECT_H_
