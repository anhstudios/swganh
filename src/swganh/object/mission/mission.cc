// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/object/mission/mission.h"

#include "anh/crc.h"

using namespace glm;
using namespace std;
using namespace swganh::object;
using namespace swganh::object::mission;
using namespace swganh::object::waypoint;

uint32_t Mission::GetDifficultyLevel() const
{
    return difficulty_level_;
}

void Mission::SetDifficultyLevel(uint32_t difficulty_level)
{
    difficulty_level_ = difficulty_level;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::DifficultyLevel", static_pointer_cast<Mission>(shared_from_this())));
}

vec3 Mission::GetStartingPosition() const
{
    return starting_position_;
}

void Mission::SetStartingPosition(vec3 starting_position)
{
    starting_position_ = starting_position;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::StartLocation", static_pointer_cast<Mission>(shared_from_this())));
}

uint32_t Mission::GetStartingSceneCrc() const
{
    return starting_scene_;
}

void Mission::SetStartingSceneCrc(uint32_t scene_crc)
{
    starting_scene_ = scene_crc;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::StartLocation", static_pointer_cast<Mission>(shared_from_this())));
}

wstring Mission::GetMissionCreator() const
{
    return creator_;
}

void Mission::SetMissionCreator(wstring mission_creator)
{
    creator_ = mission_creator;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::CreatorName", static_pointer_cast<Mission>(shared_from_this())));
}

uint32_t Mission::GetMissionReward() const
{
    return reward_;
}

void Mission::SetMissionReward(uint32_t mission_reward)
{
    reward_ = mission_reward;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::CreditReward", static_pointer_cast<Mission>(shared_from_this())));
}

vec3 Mission::GetDestinationPosition() const
{
    return destination_position_;
}

void Mission::SetDestinationPosition(vec3 destination_position)
{
    destination_position_ = destination_position;
	
	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::DestinationLocation", static_pointer_cast<Mission>(shared_from_this())));
}

uint32_t Mission::GetDestinationSceneCrc() const
{
    return destination_scene_;
}

void Mission::SetDestinationSceneCrc(uint32_t scene_crc)
{
    destination_scene_ = scene_crc;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::DestinationLocation", static_pointer_cast<Mission>(shared_from_this())));
}

std::string Mission::GetTargetObjectTemplate() const
{
    return target_object_template_;
}

uint32_t Mission::GetTargetObjectTemplateCrc() const
{
    return anh::memcrc(target_object_template_);
}

void Mission::SetTargetObjectTemplate(std::string object_template)
{
    target_object_template_ = object_template;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::TargetIff", static_pointer_cast<Mission>(shared_from_this())));
}

std::string Mission::GetMissionDescriptionStfFile() const
{
    return mission_description_stf_file_;
}

std::string Mission::GetMissionDescriptionStfName() const
{
    return mission_description_stf_name_;
}

void Mission::SetMissionDescription(const std::string& stf_file_name, const std::string& stf_string)
{
    mission_description_stf_file_ = stf_file_name;
    mission_description_stf_name_ = stf_string;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::MissionDescription", static_pointer_cast<Mission>(shared_from_this())));
}

std::string Mission::GetMissionTitleStfFile() const
{
    return mission_title_stf_file_;
}

std::string Mission::GetMissionTitleStfName() const
{
    return mission_title_stf_name_;
}

void Mission::SetMissionTitle(const std::string& stf_file_name, const std::string& stf_string)
{
    mission_title_stf_file_ = stf_file_name;
    mission_title_stf_name_ = stf_string;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::MissionTitle", static_pointer_cast<Mission>(shared_from_this())));
}

uint32_t Mission::GetRepeatCounter() const
{
    return repeat_counter_;
}

void Mission::IncrementRepeatCounter()
{
    ++repeat_counter_;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::RepeatCounter", static_pointer_cast<Mission>(shared_from_this())));
}

void Mission::SetRepeatCounter(uint32_t counter)
{
    repeat_counter_ = counter;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::RepeatCounter", static_pointer_cast<Mission>(shared_from_this())));
}

std::string Mission::GetMissionType()
{
    return mission_type_;
}

uint32_t Mission::GetMissionTypeCrc()
{
    return anh::memcrc(mission_type_);
}

void Mission::SetMissionType(std::string mission_type)
{
    mission_type_ = mission_type;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::MissionType", static_pointer_cast<Mission>(shared_from_this())));
}

std::string Mission::GetTargetName() const
{
    return target_name_;
}

void Mission::SetTargetName(std::string target_name)
{
    target_name_ = target_name;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::TargetName", static_pointer_cast<Mission>(shared_from_this())));
}

std::shared_ptr<swganh::object::waypoint::Waypoint> Mission::GetMissionWaypoint()
{
	return waypoint_;
}

void Mission::SetMissionWaypoint(std::shared_ptr<swganh::object::waypoint::Waypoint> waypoint)
{
	waypoint_ = waypoint;

	GetEventDispatcher()->Dispatch(make_shared<MissionEvent>
        ("Mission::Waypoint", static_pointer_cast<Mission>(shared_from_this())));
}