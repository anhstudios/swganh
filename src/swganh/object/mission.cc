
#include "swganh/object/mission.h"

#include "anh/crc.h"

using namespace glm;
using namespace std;
using namespace swganh::object;

uint32_t Mission::GetDifficultyLevel() const
{
    return difficulty_level_;
}

void Mission::SetDifficultyLevel(uint32_t difficulty_level)
{
    difficulty_level_ = difficulty_level;
}

vec3 Mission::GetStartingPosition() const
{
    return starting_position_;
}

void Mission::SetStartingPosition(vec3 starting_position)
{
    starting_position_ = starting_position;
}

uint32_t Mission::GetStartingSceneCrc() const
{
    return starting_scene_;
}

void Mission::SetStartingSceneCrc(uint32_t scene_crc)
{
    starting_scene_ = scene_crc;
}

wstring Mission::GetMissionCreator() const
{
    return creator_;
}

void Mission::SetMissionCreator(wstring mission_creator)
{
    creator_ = mission_creator;
}

uint32_t Mission::GetMissionReward() const
{
    return reward_;
}

void Mission::SetMissionReward(uint32_t mission_reward)
{
    reward_ = mission_reward;
}

vec3 Mission::GetDestinationPosition() const
{
    return destination_position_;
}

void Mission::SetDestinationPosition(vec3 destination_position)
{
    destination_position_ = destination_position;
}

uint32_t Mission::GetDestinationSceneCrc() const
{
    return destination_scene_;
}

void Mission::SetDestinationSceneCrc(uint32_t scene_crc)
{
    destination_scene_ = scene_crc;
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
}

std::string Mission::GetMissionDescriptionStfFile() const
{
    return mission_description_stf_file_;
}

void Mission::SetMissionDescriptionStfFile(std::string stf_file)
{
    mission_description_stf_file_ = stf_file;
}

std::string Mission::GetMissionDescriptionStfName() const
{
    return mission_description_stf_name_;
}

void Mission::SetMissionDescriptionStfName(std::string stf_name)
{
    mission_description_stf_name_ = stf_name;
}

std::string Mission::GetMissionTitleStfFile() const
{
    return mission_title_stf_file_;
}

void Mission::SetMissionTitleStfFile(std::string stf_file)
{
    mission_title_stf_file_ = stf_file;
}

std::string Mission::GetMissionTitleStfName() const
{
    return mission_title_stf_name_;
}

void Mission::SetMissionTitleStfName(std::string stf_name)
{
    mission_title_stf_name_ = stf_name;
}

uint32_t Mission::GetRepeatCounter() const
{
    return repeat_counter_;
}

void Mission::IncrementRepeatCounter()
{
    ++repeat_counter_;
}

void Mission::SetRepeatCounter(uint32_t counter)
{
    repeat_counter_ = counter;
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
}

std::string Mission::GetTargetName() const
{
    return target_name_;
}

void Mission::SetTargetName(std::string target_name)
{
    target_name_ = target_name;
}

Waypoint Mission::GetWaypoint() const
{
    return waypoint_;
}

void Mission::SetWaypoint(Waypoint waypoint)
{
    waypoint_ = move(waypoint);
}
