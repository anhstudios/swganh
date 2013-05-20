// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

struct HopperItem
{
	uint64_t global_id;
	float quantity;

	void Serialize(swganh::messages::BaselinesMessage& message)
	{
		message.data.write(global_id);
		message.data.write(quantity);
	}

	void Serialize(swganh::messages::DeltasMessage& message)
	{
		message.data.write(global_id);
		message.data.write(quantity);
	}

	bool operator==(const HopperItem& other)
	{
		return other.global_id == global_id;
	}
};

}
}