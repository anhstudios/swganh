#ifndef SWGANH_OBJECT_PLAYER_PLAYER_H_
#define SWGANH_OBJECT_PLAYER_PLAYER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>

#include "swganh/object/object.h"
#include "swganh/object/waypoint/waypoint.h"

namespace swganh {
namespace object {
namespace player {

class PlayerMessageBuilder;

class Player : public swganh::object::Object
{
public:

    Player();
	
    enum Gender
    {
        FEMALE = 0,
        MALE
    };
    enum StatusFlags
    {
        LFG = 1,
        HELPER = 2,
        ROLEPLAYER = 4,
        AFK = 80,
        LD = 100,
        FACTION_RANK = 200,
        ANON = 80000000
    };
    struct XpData
    {
        std::string type;
        uint32_t value;
    };

    struct QuestJournalData
    {
        uint32_t quest_crc;
        uint64_t owner_id;
        uint16_t active_step_bitmask;
        uint16_t completed_step_bitmask;
        bool completed_flag;
        uint32_t counter;
    };

    struct DraftSchematicData
    {
        uint32_t schematic_id;
        uint32_t schematic_crc;
    };
   
    virtual uint32_t GetType() const { return Player::type; }
    const static uint32_t type = 0x504c4159;

    uint32_t GetStatusFlags() const { return status_flags_; }
    void AddStatusFlag(uint32_t flag);
    void RemoveStatusFlag(uint32_t flag);
    void ToggleStatusFlag(uint32_t flag);
    void ClearStatusFlags();
    
    uint32_t GetProfileFlags() const;
    void AddProfileFlag(uint32_t flag);
    void RemoveProfileFlag(uint32_t flag);
    void ToggleProfileFlag(uint32_t flag);
    void ClearProfileFlags();

    std::string GetProfessionTag() const;
    void SetProfessionTag(std::string profession_tag);

    uint32_t GetBornDate() const;
    void SetBornDate(uint32_t born_date);

    uint32_t GetTotalPlayTime() const;
    void SetTotalPlayTime(uint32_t play_time);
    void IncrementTotalPlayTime(uint32_t increment);

    uint8_t GetAdminTag() const;
    void SetAdminTag(uint8_t tag);

    uint32_t GetRegionId() const;
    void SetRegionId(uint32_t region);

    std::vector<XpData> GetXp() const;
    void AddExperience(std::string type, uint32_t amount);
    void DeductXp(std::string type, uint32_t amount);
    void ClearXpType(std::string type);
    void ResetXp(std::vector<XpData> experience);
    void ClearAllXp();
    
    std::vector<std::shared_ptr<swganh::object::waypoint::Waypoint>> GetWaypoints() const;
    void AddWaypoint(std::shared_ptr<swganh::object::waypoint::Waypoint> waypoint);
    void RemoveWaypoint(uint64_t waypoint_id);
    void ModifyWaypoint(std::shared_ptr<swganh::object::waypoint::Waypoint> waypoint);
    void ResetWaypoints(std::vector<std::shared_ptr<swganh::object::waypoint::Waypoint>> waypoints);
    void ClearAllWaypoints();

    uint32_t GetCurrentForcePower() const;
    void IncreaseForcePower(uint32_t force_power);
    void DecreaseForcePower(uint32_t force_power);
    void SetCurrentForcePower(uint32_t force_power);
    
    uint32_t GetMaxForcePower() const;
    void SetMaxForcePower(uint32_t force_power);

    uint32_t GetCurrentForceSensitiveQuests();
    void AddCurrentForceSensitiveQuest(uint32_t quest_mask);
    void RemoveCurrentForceSensitiveQuest(uint32_t quest_mask);
    void ClearCurrentForceSensitiveQuests();
    
    uint32_t GetCompletedForceSensitiveQuests();
    void AddCompletedForceSensitiveQuest(uint32_t quest_mask);
    void RemoveCompletedForceSensitiveQuest(uint32_t quest_mask);
    void ClearCompletedForceSensitiveQuests();

    std::vector<QuestJournalData> GetQuests() const;
    void AddQuest(QuestJournalData quest);
    void RemoveQuest(QuestJournalData quest);
    void UpdateQuest(QuestJournalData quest);
    void ResetQuests(std::vector<QuestJournalData> quests);
    void ClearAllQuests();
    
    std::vector<std::string> GetAbilityList() const;
    void AddAbility(std::string ability);
    void RemoveAbility(std::string ability);
    void ClearAllAbilities();
    void ResetAbilities(std::vector<std::string> abilities);
    
    uint32_t GetExperimentationFlag() const;
    void SetExperimentationFlag(uint32_t experimentation_flag);
    
    uint32_t GetCraftingStage() const;
    void SetCraftingStage(uint32_t crafting_stage);

    uint64_t GetNearestCraftingStation() const;
    void SetNearestCraftingStation( uint64_t crafting_station_id);
    
    std::vector<DraftSchematicData> GetDraftSchematics() const;
    void AddDraftSchematic(DraftSchematicData schematic);
    void RemoveDraftSchematic(uint32_t schematic_crc);
    void ClearDraftSchematics();
    void ResetDraftSchematics(std::vector<DraftSchematicData> draft_schematics);
    
    uint32_t GetExperimentationPoints() const;
    void AddExperimentationPoints(uint32_t points);
    void RemoveExperimentationPoints(uint32_t points);
    void ResetExperimentationPoints(uint32_t points);
    
    uint32_t GetAccomplishmentCounter() const;
    void ResetAccomplishmentCounter(uint32_t counter);
    void IncrementAccomplishmentCounter();

    std::vector<std::string> GetFriends();
    void AddFriend(std::string friend_name);
    void RemoveFriend(std::string friend_name);
    void ClearFriends();
    
    std::vector<std::string> GetIgnoredPlayers();
    void IgnorePlayer(std::string player_name);
    void StopIgnoringPlayer(std::string player_name);
    void ClearIgnored();
    
    uint32_t GetLanguage() const;
    void SetLanguage(uint32_t language_id);
    
    uint32_t GetCurrentStomach() const;
    void IncreaseCurrentStomach(uint32_t stomach);
    void DecreaseCurrentStomach(uint32_t stomach);
    void ResetCurrentStomach(uint32_t stomach);

    uint32_t GetMaxStomach() const;
    void ResetMaxStomach(uint32_t stomach);
    
    uint32_t GetCurrentDrink() const;
    void IncreaseCurrentDrink(uint32_t drink);
    void DecreaseCurrentDrink(uint32_t drink);
    void ResetCurrentDrink(uint32_t drink);

    uint32_t GetMaxDrink() const;
    void ResetMaxDrink(uint32_t drink);
    
    uint32_t GetJediState() const;
    void SetJediState(uint32_t jedi_state);

    // Gender Set during creation
    Gender GetGender() const;
    void SetGender(Gender value);

    // baselines
    //virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline3();
    //virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline6();
    //virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline8();
    //virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline9();

private:
    friend PlayerMessageBuilder;

    void SetDeltaBitmask_(uint32_t bitmask, uint16_t update_type, swganh::object::Object::ViewType view_type);

    uint32_t status_flags_;
    uint32_t profile_flags_;
    std::string profession_tag_;
    uint32_t born_date_;
    uint32_t total_playtime_;
    uint8_t admin_tag_;
    uint32_t region_;
    std::vector<XpData> experience_;
    uint32_t experience_counter_;
    std::list<uint16_t> experience_free_list_;
    std::vector<std::shared_ptr<swganh::object::waypoint::Waypoint>> waypoints_;
    std::list<uint16_t> waypoint_free_list_;
    uint32_t waypoint_counter_;
    uint32_t current_force_power_;
    uint32_t max_force_power_;
    uint32_t current_force_sensitive_quests_;
    uint32_t completed_force_sensitive_quests_;
    std::vector<QuestJournalData> quest_journal_;
    std::vector<std::string> abilities_;
    std::list<uint16_t> abilities_free_list_;
    std::vector<std::string>::iterator GetAbilityIter_(std::string ability);
    uint32_t abilities_counter_;
    uint32_t experimentation_flag_;
    uint32_t crafting_stage_;
    uint64_t nearest_crafting_station_;
    std::vector<DraftSchematicData> draft_schematics_;
    std::list<uint16_t> draft_schematics_free_list_;
    uint32_t draft_schematics_counter_;
    std::vector<DraftSchematicData>::iterator GetSchematicIter_(uint32_t schematic_crc);
    uint32_t experimentation_points_;
    uint32_t accomplishment_counter_;
    std::vector<std::string> friends_;
    std::list<uint16_t> friends_free_list_;
    std::vector<std::string>::iterator GetFriendsIter_(std::string friend_name);
    std::vector<std::string> ignored_players_;
    std::list<uint16_t> ignored_free_list_;
    std::vector<std::string>::iterator GetIgnoredIter_(std::string ignored_name);
    uint32_t language_;
    uint32_t current_stomach_;
    uint32_t max_stomach_;
    uint32_t current_drink_;
    uint32_t max_drink_;
    uint32_t jedi_state_;
    Gender gender_;
};

}}}  // namespace swganh::object::player

#endif  // SWGANH_OBJECT_PLAYER_PLAYER_H_
