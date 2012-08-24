// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_STATIC_STATIC_H_
#define SWGANH_OBJECT_STATIC_STATIC_H_

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace static_object {
  
class StaticFactory;
class StaticMessageBuilder;

class Static : public swganh::object::Object
{
public:
	typedef StaticFactory FactoryType;
    typedef StaticMessageBuilder MessageBuilderType;

    // STAO
    uint32_t GetType() const;
    const static uint32_t type = 0x5354414F;
};

}}}  // namespace swganh::object::static_obj

#endif  // SWGANH_OBJECT_STATIC_STATIC_H_
