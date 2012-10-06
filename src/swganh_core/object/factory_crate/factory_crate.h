// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/tangible/tangible.h"

namespace swganh {
namespace object {

class FactoryCrateFactory;
class FactoryCrateMessageBuilder;

class FactoryCrate : public swganh::object::Tangible 
{
public:
	typedef FactoryCrateFactory FactoryType;
	typedef FactoryCrateMessageBuilder MessageBuilderType;
    // FCYT
    uint32_t GetType() const;
    const static uint32_t type = 0x46435954;
};

}} // swganh::object
