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
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return difficulty_level_;
}

void Mission::SetDifficultyLevel(uint32_t difficulty_level)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		difficulty_level_ = difficulty_level;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::DifficultyLevel", static_pointer_cast<Mission>(shared_from_this())));
}

vec3 Mission::GetStartingPosition() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return starting_position_;
}

void Mission::SetStartingPosition(vec3 starting_position)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		starting_position_ = starting_position;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::StartLocation", static_pointer_cast<Mission>(shared_from_this())));
}

uint32_t Mission::GetStartingSceneCrc() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return starting_scene_;
}

void Mission::SetStartingSceneCrc(uint32_t scene_crc)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		starting_scene_ = scene_crc;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::StartLocation", static_pointer_cast<Mission>(shared_from_this())));
}

wstring Mission::GetMissionCreator() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return creator_;
}

void Mission::SetMissionCreator(wstring mission_creator)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		creator_ = mission_creator;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::CreatorName", static_pointer_cast<Mission>(shared_from_this())));
}

uint32_t Mission::GetMissionReward() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return reward_;
}

void Mission::SetMissionReward(uint32_t mission_reward)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		reward_ = mission_reward;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::CreditReward", static_pointer_cast<Mission>(shared_from_this())));
}

vec3 Mission::GetDestinationPosition() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return destination_position_;
}

void Mission::SetDestinationPosition(vec3 destination_position)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		destination_position_ = destination_position;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::DestinationLocation", static_pointer_cast<Mission>(shared_from_this())));
}

uint32_t Mission::GetDestinationSceneCrc() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return destination_scene_;
}

void Mission::SetDestinationSceneCrc(uint32_t scene_crc)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		destination_scene_ = scene_crc;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::DestinationLocation", static_pointer_cast<Mission>(shared_from_this())));
}

std::string Mission::GetTargetObjectTemplate() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return target_object_template_;
}

uint32_t Mission::GetTargetObjectTemplateCrc() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return swganh::memcrc(target_object_template_);
}

void Mission::SetTargetObjectTemplate(std::string object_template)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		target_object_template_ = object_template;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::TargetIff", static_pointer_cast<Mission>(shared_from_this())));
}

std::string Mission::GetMissionDescriptionStfFile() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return mission_description_stf_file_;
}

std::string Mission::GetMissionDescriptionStfName() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return mission_description_stf_name_;
}

void Mission::SetMissionDescription(const std::string& stf_file_name, const std::string& stf_string)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		mission_description_stf_file_ = stf_file_name;
		mission_description_stf_name_ = stf_string;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::MissionDescription", static_pointer_cast<Mission>(shared_from_this())));
}

std::string Mission::GetMissionTitleStfFile() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return mission_title_stf_file_;
}

std::string Mission::GetMissionTitleStfName() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return mission_title_stf_name_;
}

void Mission::SetMissionTitle(const std::string& stf_file_name, const std::string& stf_string)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		mission_title_stf_file_ = stf_file_name;
		mission_title_stf_name_ = stf_string;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::MissionTitle", static_pointer_cast<Mission>(shared_from_this())));
}

uint32_t Mission::GetRepeatCounter() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return repeat_counter_;
}

void Mission::IncrementRepeatCounter()
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		++repeat_counter_;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::RepeatCounter", static_pointer_cast<Mission>(shared_from_this())));
}

void Mission::SetRepeatCounter(uint32_t counter)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		repeat_counter_ = counter;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::RepeatCounter", static_pointer_cast<Mission>(shared_from_this())));
}

std::string Mission::GetMissionType()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return mission_type_;
}

uint32_t Mission::GetMissionTypeCrc()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return swganh::memcrc(mission_type_);
}

void Mission::SetMissionType(std::string mission_type)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		mission_type_ = mission_type;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::MissionType", static_pointer_cast<Mission>(shared_from_this())));
}

std::string Mission::GetTargetName() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return target_name_;
}

void Mission::SetTargetName(std::string target_name)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		target_name_ = target_name;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::TargetName", static_pointer_cast<Mission>(shared_from_this())));
}

std::shared_ptr<swganh::object::Waypoint> Mission::GetMissionWaypoint()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	return waypoint_;
}

void Mission::SetMissionWaypoint(std::shared_ptr<swganh::object::Waypoint> waypoint)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		waypoint_ = waypoint;
	}

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::Waypoint", static_pointer_cast<Mission>(shared_from_this())));
}