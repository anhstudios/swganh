// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

struct Experiment
{
	uint16_t index;
	std::string experiment_stf_file; 
	std::string experiment_stf_name;
	float value;
	float offset;
	float size;
	float max_value;
	bool operator==(const Experiment& other)
	{
		return other.value == value;
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