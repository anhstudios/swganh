// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

struct Property
{
	std::string property_stf_file;
	std::string property_stf_name;
	float value;

	bool operator==(const Property& other)
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