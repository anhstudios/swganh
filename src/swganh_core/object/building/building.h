// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "swganh_core/object/object.h"
#include "swganh_core/object/tangible/tangible.h"

namespace swganh {
namespace object {

class BuildingFactory;
class BuildingMessageBuilder;

class Building : public swganh::object::Tangible
{
public:
	typedef BuildingFactory FactoryType;
    typedef BuildingMessageBuilder MessageBuilderType;

    uint32_t GetType() const;
    const static uint32_t type = 0x4255494f;

private:

};

}}  // namespace swganh::object
