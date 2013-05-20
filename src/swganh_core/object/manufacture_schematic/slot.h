// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

struct Slot 
{
	uint32_t index;
	std::string slot_stf_file;
	std::string slot_stf_name;
	uint32_t type;
	uint64_t ingredient;
	uint32_t ingredient_quantity;
	uint32_t clean;

	bool operator==(const Slot& other) const
	{
		return other.ingredient == ingredient;
	}
	void Serialize(swganh::messages::BaselinesMessage& message)
	{
	}

	void Serialize(swganh::messages::DeltasMessage& message)
	{
	}
};

}
}