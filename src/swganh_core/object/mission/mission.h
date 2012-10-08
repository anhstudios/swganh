// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>

#include <glm/glm.hpp>

#include "swganh_core/object/intangible/intangible.h"
#include "swganh_core/object/waypoint/waypoint.h"

namespace swganh {
namespace object {


class MissionFactory;
class MissionMessageBuilder;

class Mission : public swganh::object::Intangible
{
public:
	typedef MissionFactory FactoryType;
    typedef MissionMessageBuilder MessageBuilderType;

    /**
     * @return The type of this object instance.
     */
    virtual uint32_t GetType() const { return Mission::type; }
    const static uint32_t type = 0x4d49534f;

    /**
     * @return The difficulty level of the mission.
     */
    uint32_t GetDifficultyLevel() const;
    
    /**
     * Sets the difficulty level for the mission.
     *
     * @param difficulty_level The difficulty level for the mission.
     */
    void SetDifficultyLevel(uint32_t difficulty_level);

    /**
     * Returns the starting position for the mission.
     *
     * Commonly used in delivery style missions. Destroy missions can
     * have it but is not required.
     */
    glm::vec3 GetStartingPosition() const;

    /**
     * Sets the starting position for the mission.
     * 
     * @param starting_position The starting position for the mission.
     */
    void SetStartingPosition(glm::vec3 starting_position);

    /**
     * @return the crc of the scene where the mission starts.
     */
    uint32_t GetStartingSceneCrc() const;

    /**
     * Sets the starting scene crc.
     *
     * @param scene_crc The crc of the start scene.
     *
     * @TODO consider holding the actual scene instance.
     */
    void SetStartingSceneCrc(uint32_t scene_crc);
    
    /**
     * @return the name of the creator of this mission.
     */
    std::wstring GetMissionCreator() const;

    /**
     * Sets the name of the mission creator.
     *
     * @param mission_creator The creator of the mission.
     */
    void SetMissionCreator(std::wstring mission_creator);
    
    /**
     * @return the amount of credits awarded for completing this mission.
     */
    uint32_t GetMissionReward() const;

    /**
     * Sets the amount of credits to award for this mission.
     *
     * @param mission_reward The amount of credits to award.
     */
    void SetMissionReward(uint32_t mission_reward);
    
    /**
     * @return the destination position of the mission.
     */
    glm::vec3 GetDestinationPosition() const;

    /**
     * Sets the destination position of the mission.
     *
     * @param destination_position The destination position of the mission.
     */
    void SetDestinationPosition(glm::vec3 destination_position);
    
    /**
     * @return the crc of the scene where the destination is located.
     */
    uint32_t GetDestinationSceneCrc() const;
    
    /**
     * Sets the destination scene crc.
     *
     * @param scene_crc The crc of the destination scene.
     *
     * @TODO consider holding the actual scene instance.
     */
    void SetDestinationSceneCrc(uint32_t scene_crc);
    
    /**
     * @return the object template used in the mission description.
     */
    std::string GetTargetObjectTemplate() const;

    /**
     * @return the object template used in the mission description in crc format.
     */
    uint32_t GetTargetObjectTemplateCrc() const;

    /**
     * Sets the object template used in the mission description.
     *
     * @param object_template The iff object template file name.
     */
    void SetTargetObjectTemplate(std::string object_template);
    
    /**
     * @return the stf file containing the mission description.
     */
    std::string GetMissionDescriptionStfFile() const;
    
    /**
     * @return the stf string containing the mission description.
     */
    std::string GetMissionDescriptionStfName() const;

    /**
     * Sets the mission description
     *
     * @param stf_file_name The stf file containing the mission description.
     * @param stf_string The string containing the mission description.
     */
    void SetMissionDescription(const std::string& stf_file_name, const std::string& stf_string);
    
    /**
     * @return the stf file containing the mission title.
     */
    std::string GetMissionTitleStfFile() const;

    /**
     * @return the stf string containing the mission title.
     */
    std::string GetMissionTitleStfName() const;
    
    /**
     * Sets the mission title
     *
     * @param stf_file_name The stf file containing the mission title.
     * @param stf_string The string containing the mission title.
     */
    void SetMissionTitle(const std::string& stf_file_name, const std::string& stf_string);

    /**
     * This counter also allows you to "redisplay" (remember the missions only 
     * display on a delta  that CHANGES the object) by changing it, and nothing 
     * else, you can get it to redisplay the mission similar to an update counter.
     *
     * @return The current repeat counter value.
     */
    uint32_t GetRepeatCounter() const;
    
    /**
     * Increments the repeat counter.
     */
    void IncrementRepeatCounter();
    
    /**
     * Sets the repeat counter to a specific value.
     *
     * @param counter The new counter value.
     */
    void SetRepeatCounter(uint32_t counter);
    
    /**
     * @return the type of the mission.
     */
    std::string GetMissionType();

    /**
     * @return the type of the mission in crc format.
     */
    uint32_t GetMissionTypeCrc();

    /**
     * Sets the mission type.
     *
     * Missions types currently supported: 
     *  destroy
     *  deliver
     *  bounty
     *  assassin
     *  crafting
     *  entertainer
     *  hunting
     *  survey
     *
     * @param mission_type The type of the mision.
     */
    void SetMissionType(std::string mission_type);
    
    /** 
     * @return The name of the target object.
     */
    std::string GetTargetName() const;

    /**
     * Sets the name of the target object.
     *
     * @param target_name The name of the target object.
     *
     * @TODO Consider holding a reference to the actual target object.
     */
    void SetTargetName(std::string target_name);

	std::shared_ptr<swganh::object::Waypoint> GetMissionWaypoint();

	void SetMissionWaypoint(std::shared_ptr<swganh::object::Waypoint> waypoint);

protected:
	typedef swganh::ValueEvent<std::shared_ptr<Mission>> MissionEvent;

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
    std::shared_ptr<swganh::object::Waypoint> waypoint_;
};

}}  // swganh::object
