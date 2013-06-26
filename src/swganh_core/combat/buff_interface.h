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
}

namespace combat
{

class BuffInterface
{
 public:
	virtual ~BuffInterface() {}
 
	virtual void ApplyBuff(std::shared_ptr<swganh::object::Creature> creature) = 0;

	virtual void RemoveBuff(std::shared_ptr<swganh::object::Creature> creature) = 0;

	virtual std::string GetName() = 0;

	virtual uint32_t GetDuration() = 0;

	std::string filename;
};

}
}