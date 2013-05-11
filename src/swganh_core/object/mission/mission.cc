// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "mission.h"

#include "swganh/crc.h"

using namespace glm;
using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::object;

uint32_t Mission::GetDifficultyLevel() const
{
	auto lock = AcquireLock();
    return difficulty_level_;
}

void Mission::SetDifficultyLevel(uint32_t difficulty_level)
{
	{
		auto lock = AcquireLock();
		difficulty_level_ = difficulty_level;
	}
	DISPATCH(Mission, DifficultyLevel);
}

vec3 Mission::GetStartingPosition() const
{
	auto lock = AcquireLock();
    return starting_position_;
}

void Mission::SetStartingPosition(vec3 starting_position)
{
	{
		auto lock = AcquireLock();
		starting_position_ = starting_position;
	}
	DISPATCH(Mission, StartLocation);
}

uint32_t Mission::GetStartingSceneCrc() const
{
	auto lock = AcquireLock();
    return starting_scene_;
}

void Mission::SetStartingSceneCrc(uint32_t scene_crc)
{
	{
		auto lock = AcquireLock();
		starting_scene_ = scene_crc;
	}
	DISPATCH(Mission, StartLocation);
}

wstring Mission::GetMissionCreator() const
{
	auto lock = AcquireLock();
    return creator_;
}

void Mission::SetMissionCreator(wstring mission_creator)
{
	{
		auto lock = AcquireLock();
		creator_ = mission_creator;
	}
	DISPATCH(Mission, CreatorName);
}

uint32_t Mission::GetMissionReward() const
{
	auto lock = AcquireLock();
    return reward_;
}

void Mission::SetMissionReward(uint32_t mission_reward)
{
	{
		auto lock = AcquireLock();
		reward_ = mission_reward;
	}
	DISPATCH(Mission, CreditReward);
}

vec3 Mission::GetDestinationPosition() const
{
	auto lock = AcquireLock();
    return destination_position_;
}

void Mission::SetDestinationPosition(vec3 destination_position)
{
	{
		auto lock = AcquireLock();
		destination_position_ = destination_position;
	}
	DISPATCH(Mission, DestinationLocation);
}

uint32_t Mission::GetDestinationSceneCrc() const
{
	auto lock = AcquireLock();
    return destination_scene_;
}

void Mission::SetDestinationSceneCrc(uint32_t scene_crc)
{
	{
		auto lock = AcquireLock();
		destination_scene_ = scene_crc;
	}
	DISPATCH(Mission, DestinationLocation);
}

std::string Mission::GetTargetObjectTemplate() const
{
	auto lock = AcquireLock();
    return target_object_template_;
}

uint32_t Mission::GetTargetObjectTemplateCrc() const
{
	auto lock = AcquireLock();
    return swganh::memcrc(target_object_template_);
}

void Mission::SetTargetObjectTemplate(std::string object_template)
{
	{
		auto lock = AcquireLock();
		target_object_template_ = object_template;
	}
	DISPATCH(Mission, TargetIff);
}

std::string Mission::GetMissionDescriptionStfFile() const
{
	auto lock = AcquireLock();
    return mission_description_stf_file_;
}

std::string Mission::GetMissionDescriptionStfName() const
{
	auto lock = AcquireLock();
    return mission_description_stf_name_;
}

void Mission::SetMissionDescription(const std::string& stf_file_name, const std::string& stf_string)
{
	{
		auto lock = AcquireLock();
		mission_description_stf_file_ = stf_file_name;
		mission_description_stf_name_ = stf_string;
	}
	DISPATCH(Mission, MissionDescription);
}

std::string Mission::GetMissionTitleStfFile() const
{
	auto lock = AcquireLock();
    return mission_title_stf_file_;
}

std::string Mission::GetMissionTitleStfName() const
{
	auto lock = AcquireLock();
    return mission_title_stf_name_;
}

void Mission::SetMissionTitle(const std::string& stf_file_name, const std::string& stf_string)
{
	{
		auto lock = AcquireLock();
		mission_title_stf_file_ = stf_file_name;
		mission_title_stf_name_ = stf_string;
	}
	DISPATCH(Mission, MissionTitle);
}

uint32_t Mission::GetRepeatCounter() const
{
	auto lock = AcquireLock();
    return repeat_counter_;
}

void Mission::IncrementRepeatCounter()
{
	{
		auto lock = AcquireLock();
		++repeat_counter_;
	}
	DISPATCH(Mission, RepeatCounter);
}

void Mission::SetRepeatCounter(uint32_t counter)
{
	{
		auto lock = AcquireLock();
		repeat_counter_ = counter;
	}
	DISPATCH(Mission, RepeatCounter);
}

std::string Mission::GetMissionType()
{
	auto lock = AcquireLock();
    return mission_type_;
}

uint32_t Mission::GetMissionTypeCrc()
{
	auto lock = AcquireLock();
    return swganh::memcrc(mission_type_);
}

void Mission::SetMissionType(std::string mission_type)
{
	{
		auto lock = AcquireLock();
		mission_type_ = mission_type;
	}
	DISPATCH(Mission, MissionType);
}

std::string Mission::GetTargetName() const
{
	auto lock = AcquireLock();
    return target_name_;
}

void Mission::SetTargetName(std::string target_name)
{
	{
		auto lock = AcquireLock();
		target_name_ = target_name;
	}
	DISPATCH(Mission, TargetName);
}

std::shared_ptr<swganh::object::Waypoint> Mission::GetMissionWaypoint()
{
	auto lock = AcquireLock();
	return waypoint_;
}

void Mission::SetMissionWaypoint(std::shared_ptr<swganh::object::Waypoint> waypoint)
{
	{
		auto lock = AcquireLock();
		waypoint_ = waypoint;
	}
	DISPATCH(Mission, Waypoint);
}
