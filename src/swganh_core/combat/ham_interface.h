// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <swganh/hash_string.h>
#include "boost/date_time/posix_time/posix_time_types.hpp"

namespace swganh
{
namespace object
{
class Creature;
class Object;
}

namespace combat
{

class HamInterface 
{
public:
    virtual ~HamInterface() {}

    virtual int32_t		UpdateCurrentHitpoints(const std::shared_ptr<swganh::object::Creature> creature, const uint16_t statIndex, const int32_t update) = 0;

	virtual int32_t		ApplyWound(std::shared_ptr<swganh::object::Creature> creature, uint16_t statIndex, int32_t update) = 0;

	virtual int32_t		RemoveWound(std::shared_ptr<swganh::object::Creature> creature, uint16_t statIndex, int32_t update) = 0;

	virtual bool		ApplyHamCost(std::shared_ptr<swganh::object::Creature> creature, uint16_t statIndex, uint32_t cost) = 0;

	virtual bool		ApplyHamCosts(std::shared_ptr<swganh::object::Creature> creature, uint32_t healthCost, uint32_t actionCost, uint32_t mindCost) = 0;

    
	
};

}
}