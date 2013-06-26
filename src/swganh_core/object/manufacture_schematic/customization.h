// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

struct Customization
{
	uint16_t index;
	std::string name;
	uint32_t pallet_selection;
	uint32_t pallet_start_index;
	uint32_t pallet_end_index;

	bool operator==(const Customization& other)
	{
		return other.name == name;
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