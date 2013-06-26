// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

struct MissionCriticalObject
{
    MissionCriticalObject(uint64_t mission_owner_id, uint16_t critical_object_id)
        : mission_owner_id_(mission_owner_id)
        , critical_object_id_(critical_object_id)
    {}

    ~MissionCriticalObject(void)
    {}

    uint64_t mission_owner_id_;
    uint64_t critical_object_id_;

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint64_t>(mission_owner_id_);
        message.data.write<uint64_t>(critical_object_id_);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint64_t>(mission_owner_id_);
        message.data.write<uint64_t>(critical_object_id_);
    }

    bool operator==(const MissionCriticalObject& object)
    {
        if(mission_owner_id_ != object.mission_owner_id_)
            return false;

        if(critical_object_id_ != object.critical_object_id_)
            return false;

        return true;
    }
	
	bool operator<(const MissionCriticalObject& b) const
	{
		if(mission_owner_id_ < b.mission_owner_id_)
		{
			return true;
		} 
		else if(mission_owner_id_ == b.mission_owner_id_)
		{
			if(critical_object_id_ < b.critical_object_id_)
			{
				return true;
			}
		}
		return false;
	}
};

}
}