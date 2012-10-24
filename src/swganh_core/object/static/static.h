// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/object.h"

namespace swganh {
namespace object {

  
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

}}  // namespace swganh::object_obj
