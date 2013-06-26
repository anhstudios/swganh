// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "mission.h"

#include "swganh/crc.h"

using namespace glm;
using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::object;

uint32_t Mission::GetDifficultyLevel() const {
    auto lock = AcquireLock();
    return GetDifficultyLevel(lock);
}

uint32_t Mission::GetDifficultyLevel(boost::unique_lock<boost::mutex>& lock) const
{
    return difficulty_level_;
}

void Mission::SetDifficultyLevel(uint32_t difficulty_level) {
    auto lock = AcquireLock();
    SetDifficultyLevel(difficulty_level, lock);
}

void Mission::SetDifficultyLevel(uint32_t difficulty_level, boost::unique_lock<boost::mutex>& lock)
{
	difficulty_level_ = difficulty_level;
	DISPATCH(Mission, DifficultyLevel);
}

vec3 Mission::GetStartingPosition() const {
    auto lock = AcquireLock();
    return GetStartingPosition(lock);
}

vec3 Mission::GetStartingPosition(boost::unique_lock<boost::mutex>& lock) const
{
    return starting_position_;
}

void Mission::SetStartingPosition(vec3 starting_position) {
    auto lock = AcquireLock();
    SetStartingPosition(starting_position, lock);
}

void Mission::SetStartingPosition(vec3 starting_position, boost::unique_lock<boost::mutex>& lock)
{
	starting_position_ = starting_position;
	DISPATCH(Mission, StartLocation);
}

uint32_t Mission::GetStartingSceneCrc() const {
    auto lock = AcquireLock();
    return GetStartingSceneCrc(lock);
}

uint32_t Mission::GetStartingSceneCrc(boost::unique_lock<boost::mutex>& lock) const
{
    return starting_scene_;
}

void Mission::SetStartingSceneCrc(uint32_t scene_crc) {
    auto lock = AcquireLock();
    SetStartingSceneCrc(scene_crc, lock);
}

void Mission::SetStartingSceneCrc(uint32_t scene_crc, boost::unique_lock<boost::mutex>& lock)
{
	starting_scene_ = scene_crc;
	DISPATCH(Mission, StartLocation);
}

wstring Mission::GetMissionCreator() const {
    auto lock = AcquireLock();
    return GetMissionCreator(lock);
}

wstring Mission::GetMissionCreator(boost::unique_lock<boost::mutex>& lock) const
{
    return creator_;
}

void Mission::SetMissionCreator(wstring mission_creator) {
    auto lock = AcquireLock();
    SetMissionCreator(mission_creator, lock);
}

void Mission::SetMissionCreator(wstring mission_creator, boost::unique_lock<boost::mutex>& lock)
{
	creator_ = mission_creator;
	DISPATCH(Mission, CreatorName);
}

uint32_t Mission::GetMissionReward() const {
    auto lock = AcquireLock();
    return GetMissionReward(lock);
}

uint32_t Mission::GetMissionReward(boost::unique_lock<boost::mutex>& lock) const
{
    return reward_;
}

void Mission::SetMissionReward(uint32_t mission_reward) {
    auto lock = AcquireLock();
    SetMissionReward(mission_reward, lock);
}

void Mission::SetMissionReward(uint32_t mission_reward, boost::unique_lock<boost::mutex>& lock)
{
	reward_ = mission_reward;
	DISPATCH(Mission, CreditReward);
}
vec3 Mission::GetDestinationPosition() const {
    auto lock = AcquireLock();
    return GetDestinationPosition(lock);
}

vec3 Mission::GetDestinationPosition(boost::unique_lock<boost::mutex>& lock) const
{
    return destination_position_;
}

void Mission::SetDestinationPosition(vec3 destination_position) {
    auto lock = AcquireLock();
    SetDestinationPosition(destination_position, lock);
}

void Mission::SetDestinationPosition(vec3 destination_position, boost::unique_lock<boost::mutex>& lock)
{
	destination_position_ = destination_position;
	DISPATCH(Mission, DestinationLocation);
}

uint32_t Mission::GetDestinationSceneCrc() const {
    auto lock = AcquireLock();
    return GetDestinationSceneCrc(lock);
}

uint32_t Mission::GetDestinationSceneCrc(boost::unique_lock<boost::mutex>& lock) const
{
    return destination_scene_;
}

void Mission::SetDestinationSceneCrc(uint32_t scene_crc) {
    auto lock = AcquireLock();
    SetDestinationSceneCrc(scene_crc, lock);
}

void Mission::SetDestinationSceneCrc(uint32_t scene_crc, boost::unique_lock<boost::mutex>& lock)
{
	destination_scene_ = scene_crc;
	DISPATCH(Mission, DestinationLocation);
}

std::string Mission::GetTargetObjectTemplate() const {
    auto lock = AcquireLock();
    return GetTargetObjectTemplate(lock);
}

std::string Mission::GetTargetObjectTemplate(boost::unique_lock<boost::mutex>& lock) const
{
    return target_object_template_;
}

uint32_t Mission::GetTargetObjectTemplateCrc() const {
    auto lock = AcquireLock();
    return GetTargetObjectTemplateCrc(lock);
}

uint32_t Mission::GetTargetObjectTemplateCrc(boost::unique_lock<boost::mutex>& lock) const
{
    return swganh::memcrc(target_object_template_);
}

void Mission::SetTargetObjectTemplate(std::string object_template) {
    auto lock = AcquireLock();
    SetTargetObjectTemplate(object_template, lock);
}

void Mission::SetTargetObjectTemplate(std::string object_template, boost::unique_lock<boost::mutex>& lock)
{
	target_object_template_ = object_template;
	DISPATCH(Mission, TargetIff);
}

std::string Mission::GetMissionDescriptionStfFile() const {
    auto lock = AcquireLock();
    return GetMissionDescriptionStfFile(lock);
}

std::string Mission::GetMissionDescriptionStfFile(boost::unique_lock<boost::mutex>& lock) const
{
    return mission_description_stf_file_;
}

std::string Mission::GetMissionDescriptionStfName() const {
    auto lock = AcquireLock();
    return GetMissionDescriptionStfName(lock);
}

std::string Mission::GetMissionDescriptionStfName(boost::unique_lock<boost::mutex>& lock) const
{
    return mission_description_stf_name_;
}

void Mission::SetMissionDescription(const std::string& stf_file_name, const std::string& stf_string) {
    auto lock = AcquireLock();
    SetMissionDescription(stf_file_name, stf_string, lock);
}

void Mission::SetMissionDescription(const std::string& stf_file_name, const std::string& stf_string, boost::unique_lock<boost::mutex>& lock)
{
	mission_description_stf_file_ = stf_file_name;
	mission_description_stf_name_ = stf_string;
	DISPATCH(Mission, MissionDescription);
}

std::string Mission::GetMissionTitleStfFile() const {
    auto lock = AcquireLock();
    return GetMissionTitleStfFile(lock);
}

std::string Mission::GetMissionTitleStfFile(boost::unique_lock<boost::mutex>& lock) const
{
    return mission_title_stf_file_;
}

std::string Mission::GetMissionTitleStfName() const {
    auto lock = AcquireLock();
    return GetMissionTitleStfName(lock);
}

std::string Mission::GetMissionTitleStfName(boost::unique_lock<boost::mutex>& lock) const
{
    return mission_title_stf_name_;
}

void Mission::SetMissionTitle(const std::string& stf_file_name, const std::string& stf_string) {
    auto lock = AcquireLock();
    SetMissionTitle(stf_file_name, stf_string, lock);
}

void Mission::SetMissionTitle(const std::string& stf_file_name, const std::string& stf_string, boost::unique_lock<boost::mutex>& lock)
{
	mission_title_stf_file_ = stf_file_name;
	mission_title_stf_name_ = stf_string;
	DISPATCH(Mission, MissionTitle);
}

uint32_t Mission::GetRepeatCounter() const {
    auto lock = AcquireLock();
    return GetRepeatCounter(lock);
}

uint32_t Mission::GetRepeatCounter(boost::unique_lock<boost::mutex>& lock) const
{
    return repeat_counter_;
}

void Mission::IncrementRepeatCounter() {
    auto lock = AcquireLock();
    IncrementRepeatCounter(lock);
}

void Mission::IncrementRepeatCounter(boost::unique_lock<boost::mutex>& lock)
{
	++repeat_counter_;
	DISPATCH(Mission, RepeatCounter);
}

void Mission::SetRepeatCounter(uint32_t counter) {
    auto lock = AcquireLock();
    SetRepeatCounter(counter, lock);
}

void Mission::SetRepeatCounter(uint32_t counter, boost::unique_lock<boost::mutex>& lock)
{
	repeat_counter_ = counter;
	DISPATCH(Mission, RepeatCounter);
}

std::string Mission::GetMissionType() {
    auto lock = AcquireLock();
    return GetMissionType(lock);
}

std::string Mission::GetMissionType(boost::unique_lock<boost::mutex>& lock)
{
    return mission_type_;
}

uint32_t Mission::GetMissionTypeCrc() {
    auto lock = AcquireLock();
    return GetMissionTypeCrc(lock);
}

uint32_t Mission::GetMissionTypeCrc(boost::unique_lock<boost::mutex>& lock)
{
    return swganh::memcrc(mission_type_);
}

void Mission::SetMissionType(std::string mission_type) {
    auto lock = AcquireLock();
    SetMissionType(mission_type, lock);
}

void Mission::SetMissionType(std::string mission_type, boost::unique_lock<boost::mutex>& lock)
{
	mission_type_ = mission_type;
	DISPATCH(Mission, MissionType);
}

std::string Mission::GetTargetName() const {
    auto lock = AcquireLock();
    return GetTargetName(lock);
}

std::string Mission::GetTargetName(boost::unique_lock<boost::mutex>& lock) const
{
    return target_name_;
}

void Mission::SetTargetName(std::string target_name) {
    auto lock = AcquireLock();
    SetTargetName(target_name, lock);
}

void Mission::SetTargetName(std::string target_name, boost::unique_lock<boost::mutex>& lock)
{
	target_name_ = target_name;
	DISPATCH(Mission, TargetName);
}

std::shared_ptr<swganh::object::Waypoint> Mission::GetMissionWaypoint() {
    auto lock = AcquireLock();
    return GetMissionWaypoint(lock);
}

std::shared_ptr<swganh::object::Waypoint> Mission::GetMissionWaypoint(boost::unique_lock<boost::mutex>& lock)
{
	return waypoint_;
}

void Mission::SetMissionWaypoint(std::shared_ptr<swganh::object::Waypoint> waypoint) {
    auto lock = AcquireLock();
    SetMissionWaypoint(waypoint, lock);
}

void Mission::SetMissionWaypoint(std::shared_ptr<swganh::object::Waypoint> waypoint, boost::unique_lock<boost::mutex>& lock)
{
	waypoint_ = waypoint;
	DISPATCH(Mission, Waypoint);
}
