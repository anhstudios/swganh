// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "mission.h"

#include "swganh/crc.h"

using namespace glm;
using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::object;

uint32_t Mission::GetDifficultyLevel() const { return GetDifficultyLevel(AcquireLock()); }
uint32_t Mission::GetDifficultyLevel(boost::unique_lock<boost::mutex>& lock) const
{
    return difficulty_level_;
}

void Mission::SetDifficultyLevel(uint32_t difficulty_level) { SetDifficultyLevel(difficulty_level, AcquireLock()); }
void Mission::SetDifficultyLevel(uint32_t difficulty_level, boost::unique_lock<boost::mutex>& lock)
{
	difficulty_level_ = difficulty_level;
	DISPATCH(Mission, DifficultyLevel);
}

vec3 Mission::GetStartingPosition() const { return GetStartingPosition(AcquireLock()); }
vec3 Mission::GetStartingPosition(boost::unique_lock<boost::mutex>& lock) const
{
    return starting_position_;
}

void Mission::SetStartingPosition(vec3 starting_position) { SetStartingPosition(starting_position, AcquireLock()); }
void Mission::SetStartingPosition(vec3 starting_position, boost::unique_lock<boost::mutex>& lock)
{
	starting_position_ = starting_position;
	DISPATCH(Mission, StartLocation);
}

uint32_t Mission::GetStartingSceneCrc() const { return GetStartingSceneCrc(AcquireLock()); }
uint32_t Mission::GetStartingSceneCrc(boost::unique_lock<boost::mutex>& lock) const
{
    return starting_scene_;
}

void Mission::SetStartingSceneCrc(uint32_t scene_crc) { SetStartingSceneCrc(scene_crc, AcquireLock()); }
void Mission::SetStartingSceneCrc(uint32_t scene_crc, boost::unique_lock<boost::mutex>& lock)
{
	starting_scene_ = scene_crc;
	DISPATCH(Mission, StartLocation);
}

wstring Mission::GetMissionCreator() const { return GetMissionCreator(AcquireLock()); }
wstring Mission::GetMissionCreator(boost::unique_lock<boost::mutex>& lock) const
{
    return creator_;
}

void Mission::SetMissionCreator(wstring mission_creator) { SetMissionCreator(mission_creator, AcquireLock()); }
void Mission::SetMissionCreator(wstring mission_creator, boost::unique_lock<boost::mutex>& lock)
{
	creator_ = mission_creator;
	DISPATCH(Mission, CreatorName);
}

uint32_t Mission::GetMissionReward() const { return GetMissionReward(AcquireLock()); }
uint32_t Mission::GetMissionReward(boost::unique_lock<boost::mutex>& lock) const
{
    return reward_;
}

void Mission::SetMissionReward(uint32_t mission_reward) { SetMissionReward(mission_reward, AcquireLock()); }
void Mission::SetMissionReward(uint32_t mission_reward, boost::unique_lock<boost::mutex>& lock)
{
	reward_ = mission_reward;
	DISPATCH(Mission, CreditReward);
}
vec3 Mission::GetDestinationPosition() const { return GetDestinationPosition(AcquireLock()); }
vec3 Mission::GetDestinationPosition(boost::unique_lock<boost::mutex>& lock) const
{
    return destination_position_;
}

void Mission::SetDestinationPosition(vec3 destination_position) { SetDestinationPosition(destination_position, AcquireLock()); }
void Mission::SetDestinationPosition(vec3 destination_position, boost::unique_lock<boost::mutex>& lock)
{
	destination_position_ = destination_position;
	DISPATCH(Mission, DestinationLocation);
}

uint32_t Mission::GetDestinationSceneCrc() const { return GetDestinationSceneCrc(AcquireLock()); }
uint32_t Mission::GetDestinationSceneCrc(boost::unique_lock<boost::mutex>& lock) const
{
    return destination_scene_;
}

void Mission::SetDestinationSceneCrc(uint32_t scene_crc) { SetDestinationSceneCrc(scene_crc, AcquireLock()); }
void Mission::SetDestinationSceneCrc(uint32_t scene_crc, boost::unique_lock<boost::mutex>& lock)
{
	destination_scene_ = scene_crc;
	DISPATCH(Mission, DestinationLocation);
}

std::string Mission::GetTargetObjectTemplate() const { return GetTargetObjectTemplate(AcquireLock()); }
std::string Mission::GetTargetObjectTemplate(boost::unique_lock<boost::mutex>& lock) const
{
    return target_object_template_;
}

uint32_t Mission::GetTargetObjectTemplateCrc() const { return GetTargetObjectTempalteCrc(AcquireLock()); }
uint32_t Mission::GetTargetObjectTemplateCrc(boost::unique_lock<boost::mutex>& lock) const
{
    return swganh::memcrc(target_object_template_);
}

void Mission::SetTargetObjectTemplate(std::string object_template) { SetTargetObjectTemplate(object_template, AcquireLock()); }
void Mission::SetTargetObjectTemplate(std::string object_template, boost::unique_lock<boost::mutex>& lock)
{
	target_object_template_ = object_template;
	DISPATCH(Mission, TargetIff);
}

std::string Mission::GetMissionDescriptionStfFile() const { return GetMissionDescriptionStfFile(AcquireLock()); }
std::string Mission::GetMissionDescriptionStfFile(boost::unique_lock<boost::mutex>& lock) const
{
    return mission_description_stf_file_;
}

std::string Mission::GetMissionDescriptionStfName() const { return GetMissionDescriptionStfName(AcquireLock()); }
std::string Mission::GetMissionDescriptionStfName(boost::unique_lock<boost::mutex>& lock) const
{
    return mission_description_stf_name_;
}

void Mission::SetMissionDescription(const std::string& stf_file_name, const std::string& stf_string) { SetMissionDescription(stf_file_name, stf_string, AcquireLock()); }
void Mission::SetMissionDescription(const std::string& stf_file_name, const std::string& stf_string, boost::unique_lock<boost::mutex>& lock)
{
	mission_description_stf_file_ = stf_file_name;
	mission_description_stf_name_ = stf_string;
	DISPATCH(Mission, MissionDescription);
}

std::string Mission::GetMissionTitleStfFile() const { return GetMissionTitleStfFile(AcquireLock()); }
std::string Mission::GetMissionTitleStfFile(boost::unique_lock<boost::mutex>& lock) const
{
    return mission_title_stf_file_;
}

std::string Mission::GetMissionTitleStfName() const { return GetMissionTitleStfName(AcquireLock()); }
std::string Mission::GetMissionTitleStfName(boost::unique_lock<boost::mutex>& lock) const
{
    return mission_title_stf_name_;
}

void Mission::SetMissionTitle(const std::string& stf_file_name, const std::string& stf_string) { SetMissionTitle(stf_file_name, stf_string, AcquireLock()); }
void Mission::SetMissionTitle(const std::string& stf_file_name, const std::string& stf_string, boost::unique_lock<boost::mutex>& lock)
{
	mission_title_stf_file_ = stf_file_name;
	mission_title_stf_name_ = stf_string;
	DISPATCH(Mission, MissionTitle);
}

uint32_t Mission::GetRepeatCounter() const { return GetRepeatCounter(AcquireLock()); }
uint32_t Mission::GetRepeatCounter(boost::unique_lock<boost::mutex>& lock) const
{
    return repeat_counter_;
}

void Mission::IncrementRepeatCounter() { IncrementRepeatCounter(AcquireLock()); }
void Mission::IncrementRepeatCounter(boost::unique_lock<boost::mutex>& lock)
{
	++repeat_counter_;
	DISPATCH(Mission, RepeatCounter);
}

void Mission::SetRepeatCounter(uint32_t counter) { SetRepeatCounter(counter, AcquireLock()); }
void Mission::SetRepeatCounter(uint32_t counter, boost::unique_lock<boost::mutex>& lock)
{
	repeat_counter_ = counter;
	DISPATCH(Mission, RepeatCounter);
}

std::string Mission::GetMissionType() { return GetMissionType(AcquireLock()); }
std::string Mission::GetMissionType(boost::unique_lock<boost::mutex>& lock)
{
    return mission_type_;
}

uint32_t Mission::GetMissionTypeCrc() { return GetMissionTypeCrc(AcquireLock()); }
uint32_t Mission::GetMissionTypeCrc(boost::unique_lock<boost::mutex>& lock)
{
    return swganh::memcrc(mission_type_);
}

void Mission::SetMissionType(std::string mission_type) { SetMissionType(mission_type, AcquireLock()); }
void Mission::SetMissionType(std::string mission_type, boost::unique_lock<boost::mutex>& lock)
{
	mission_type_ = mission_type;
	DISPATCH(Mission, MissionType);
}

std::string Mission::GetTargetName() const { return GetTargetName(AcquireLock()); }
std::string Mission::GetTargetName(boost::unique_lock<boost::mutex>& lock) const
{
    return target_name_;
}

void Mission::SetTargetName(std::string target_name) { SetTargetName(target_name, AcquireLock()); }
void Mission::SetTargetName(std::string target_name, boost::unique_lock<boost::mutex>& lock)
{
	target_name_ = target_name;
	DISPATCH(Mission, TargetName);
}

std::shared_ptr<swganh::object::Waypoint> Mission::GetMissionWaypoint() { return GetMissionWaypoint(AcquireLock()); }
std::shared_ptr<swganh::object::Waypoint> Mission::GetMissionWaypoint(boost::unique_lock<boost::mutex>& lock)
{
	return waypoint_;
}

void Mission::SetMissionWaypoint(std::shared_ptr<swganh::object::Waypoint> waypoint) { SetMissionWaypoint(waypoint, AcquireLock()); }
void Mission::SetMissionWaypoint(std::shared_ptr<swganh::object::Waypoint> waypoint, boost::unique_lock<boost::mutex>& lock)
{
	waypoint_ = waypoint;
	DISPATCH(Mission, Waypoint);
}
