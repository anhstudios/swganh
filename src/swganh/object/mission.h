
#ifndef SWGANH_OBJECT_INTANGIBLE_MISSION_H_
#define SWGANH_OBJECT_INTANGIBLE_MISSION_H_

#include <cstdint>
#include <string>

#include <glm/glm.hpp>

#include "swganh/object/base_object.h"
#include "swganh/object/waypoint.h"

namespace swganh {
namespace object {

class Mission : public BaseObject
{
public:
    virtual uint32_t GetType() { return Mission::type; }
    const static uint32_t type = 0x4d49534f;

    uint32_t GetDifficultyLevel() const;
    void SetDifficultyLevel(uint32_t difficulty_level);

    glm::vec3 GetStartingPosition() const;
    void SetStartingPosition(glm::vec3 starting_position);

    uint32_t GetStartingSceneCrc() const;
    void SetStartingSceneCrc(uint32_t scene_crc);

    std::wstring GetMissionCreator() const;
    void SetMissionCreator(std::wstring mission_creator);

    uint32_t GetMissionReward() const;
    void SetMissionReward(uint32_t mission_reward);
    
    glm::vec3 GetDestinationPosition() const;
    void SetDestinationPosition(glm::vec3 destination_position);

    uint32_t GetDestinationSceneCrc() const;
    void SetDestinationSceneCrc(uint32_t scene_crc);
    
    std::string GetTargetObjectTemplate() const;
    uint32_t GetTargetObjectTemplateCrc() const;
    void SetTargetObjectTemplate(std::string object_template);
    
    std::string GetMissionDescriptionStfFile() const;
    void SetMissionDescriptionStfFile(std::string stf_file);

    std::string GetMissionDescriptionStfName() const;
    void SetMissionDescriptionStfName(std::string stf_name);
    
    std::string GetMissionTitleStfFile() const;
    void SetMissionTitleStfFile(std::string stf_file);

    std::string GetMissionTitleStfName() const;
    void SetMissionTitleStfName(std::string stf_name);

    uint32_t GetRepeatCounter() const;
    void IncrementRepeatCounter();
    void SetRepeatCounter(uint32_t counter);

    std::string GetMissionType();
    uint32_t GetMissionTypeCrc();
    void SetMissionType(std::string mission_type);
    
    std::string GetTargetName() const;
    void SetTargetName(std::string target_name);

    Waypoint GetWaypoint() const;
    void SetWaypoint(Waypoint waypoint);

private:
    virtual void OnReliableUpdate() {

    }

    uint32_t difficulty_level_;
    glm::vec3 starting_position_;
    uint32_t starting_scene_;
    std::wstring creator_;
    uint32_t reward_;
    glm::vec3 destination_position_;
    uint32_t destination_scene_;
    std::string target_object_template_;
    std::string mission_description_stf_file_;
    std::string mission_description_stf_name_;
    std::string mission_title_stf_file_;
    std::string mission_title_stf_name_;
    uint32_t repeat_counter_;
    std::string mission_type_;
    std::string target_name_;
    Waypoint waypoint_;
};

}}  // swganh::object

#endif  // SWGANH_OBJECT_MISSION_H_
