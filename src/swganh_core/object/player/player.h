// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <array>
#include <atomic>
#include <list>
#include <string>
#include <bitset>
#include <queue>
#include <boost/thread/mutex.hpp>

#include "swganh_core/messages/containers/network_map.h"
#include "swganh_core/messages/containers/network_vector.h"
#include "swganh_core/object/intangible/intangible.h"

#include "gender.h"
#include "admin_tag.h"
#include "draft_schematic_data.h"
#include "profile_flags.h"
#include "quest_journal_data.h"
#include "status_flags.h"
#include "status_index.h"
#include "waypoint_serializer.h"
#include "xp_data.h"



namespace swganh {
namespace badge {
	struct Badge;
} // namespace swganh::badge

namespace object {

class PlayerFactory;
class PlayerMessageBuilder;

class Player : public swganh::object::Intangible
{
public:
    typedef PlayerFactory FactoryType;
    typedef PlayerMessageBuilder MessageBuilderType;

    Player();
    
    /**
     * @return The type of the object.
     */    
    virtual uint32_t GetType() const { return Player::type; }
    const static uint32_t type = 0x504c4159;

    /**
     * @return The status flags for the player object.
     */
    std::array<uint32_t, 4> GetStatusFlags();
	std::array<uint32_t, 4> GetStatusFlags(boost::unique_lock<boost::mutex>& lock);

    /**
     * Adds a flag to the status flags.
     * [INT1]
     * 1 - LFG(can combine with HELPER)
     * 2 - HELPER(can combine with LFG)
     * 4 - ROLEPLAYER (no tag)
     * 80 - AFK (overrides previous tags)
     * 100 - LD (overrides other tags)
     * 200 - Faction Rank(seperate tag)
     * 
     * [INT4]
     * 80000000 - Anonymous (no tag) 
     * 
     * @param flag The value of the flag.
     * @param index The index of the flag (generally just left at default).
     */
    void AddStatusFlag(StatusFlags flag, StatusIndex index = DEFAULT);
	void AddStatusFlag(StatusFlags flag, boost::unique_lock<boost::mutex>& lock, StatusIndex index = DEFAULT);

    /**
     * Removes a status flag
     *
     * @param flag The value of the flag.
     * @param index The index of the flag (generally just left at default).
     */
    void RemoveStatusFlag(StatusFlags flag, StatusIndex index = DEFAULT);
	void RemoveStatusFlag(StatusFlags flag, boost::unique_lock<boost::mutex>& lock, StatusIndex index = DEFAULT);

    /**
     * Clears all status flags.
     */
    void ClearStatusFlags();
    void ClearStatusFlags(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return The profile flags for the player object.
     */
    std::array<uint32_t, 4> GetProfileFlags();
    std::array<uint32_t, 4> GetProfileFlags(boost::unique_lock<boost::mutex>& lock);

    /**
     * Adds a flag to the profile flags.
     *
     * @param flag The value of the flag.
     * @param index The index of the flag (generally just left at default).
     */
    void AddProfileFlag(ProfileFlags flag, StatusIndex index = DEFAULT);
	void AddProfileFlag(ProfileFlags flag, boost::unique_lock<boost::mutex>& lock, StatusIndex index = DEFAULT);
    
    /**
     * Removes a profile flag
     *
     * @param flag The value of the flag.
     * @param index The index of the flag (generally just left at default).
     */
    void RemoveProfileFlag(ProfileFlags flag, StatusIndex index = DEFAULT);
	void RemoveProfileFlag(ProfileFlags flag, boost::unique_lock<boost::mutex>& lock, StatusIndex index = DEFAULT);
    
    /**
     * Clears all profile flags.
     */
    void ClearProfileFlags();
	void ClearProfileFlags(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return The profession tag currently set for the player.
     */
    std::string GetProfessionTag() ;
	std::string GetProfessionTag(boost::unique_lock<boost::mutex>& lock) ;
    
    /**
     * Sets the profession tag for the player.
     *
     * @param profession_tag The profession tag name.
     */
    void SetProfessionTag(std::string profession_tag);
	void SetProfessionTag(std::string profession_tag, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return the date the character was created.
     */
    uint32_t GetBornDate() ;
	uint32_t GetBornDate(boost::unique_lock<boost::mutex>& lock) ;

    /**
     * Sets the born date for the character.
     *
     * The integer of the time when the character was created to be used 
     * as born date. This has been implemented by simply using yyyymmdd.
     * Client ignores - / (spaces) in this, so a simple date without any 
     * characters makes this work. 
     *
     * Example: 20080110
     *
     * @param born_date The born date for the character.
     */
    void SetBornDate(uint32_t born_date);
	void SetBornDate(uint32_t born_date, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return the total playtime for the player.
     */
    uint32_t GetTotalPlayTime() ;
	uint32_t GetTotalPlayTime(boost::unique_lock<boost::mutex>& lock) ;

    /**
     * Sets the total playtime for the player.
     *
     * @param play_time the new play time for the player.
     */
    void SetTotalPlayTime(uint32_t play_time);
	void SetTotalPlayTime(uint32_t play_time, boost::unique_lock<boost::mutex>& lock);


    /**
     * Increments the play time by the specified amount.
     *
     * @param increment the amount of play time to increment by.
     */
    void IncrementTotalPlayTime(uint32_t increment);
	void IncrementTotalPlayTime(uint32_t increment, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return the admin tag.
     */
    uint8_t GetAdminTag() ;
	uint8_t GetAdminTag(boost::unique_lock<boost::mutex>& lock) ;

    /**
     * Sets the admin tag, can be one of the following:
     *
     * 0 = no tag
     * 1 = CSR
     * 2 = Developer
     *
     * @param tag The new admin tag.
     *
     * @TODO consider making this an enum.
     */
    void SetAdminTag(uint8_t tag);
	void SetAdminTag(uint8_t tag, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return the current experience for the player.
     */
	std::map<std::string, XpData> GetXp() ;
	std::map<std::string, XpData> GetXp(boost::unique_lock<boost::mutex>& lock) ;
    
    /**
     * Adds experience to the player.
     *
     * @param experience The experience to add.
     */
    void AddExperience(std::string type, uint32_t value);
	void AddExperience(std::string type, uint32_t value, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Removes experience from the player.
     *
     * @param experience The experience to remove.
     */
    void DeductXp(std::string type, uint32_t value);
	void DeductXp(std::string type, uint32_t value, boost::unique_lock<boost::mutex>& lock);

	void SerializeXp(swganh::messages::BaseSwgMessage* message);
	void SerializeXp(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    /**
     * Clears all experience of a specific type.
     *
     * @param type The type of experience to clear.
     */
    void ClearXpType(std::string type);
	void ClearXpType(std::string type, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return The waypoints currently held by the player.
     */
	std::vector<std::shared_ptr<Waypoint>> GetWaypoints() ;
	std::vector<std::shared_ptr<Waypoint>> GetWaypoints(boost::unique_lock<boost::mutex>& lock) ;
    
    /**
     * Adds a waypoint to the player.
     *
     * @param waypoint The waypoint to add to the player.
     */
    void AddWaypoint(const std::shared_ptr<Waypoint>& waypoint);
	void AddWaypoint(const std::shared_ptr<Waypoint>& waypoint, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Removes a waypoint from the player.
     * 
     * @param waypoint_id The id of the waypoint to remove.
     */
	void RemoveWaypoint(uint64_t waypoint_id);
    void RemoveWaypoint(uint64_t waypoint_id, boost::unique_lock<boost::mutex>& lock);

    /**
     * Modifies an existing waypoint.
     *
     * @param waypoint The new waypoint data.
     */
    void ModifyWaypoint(uint64_t way_object_id);
	void ModifyWaypoint(uint64_t way_object_id, boost::unique_lock<boost::mutex>& lock);
    
	void SerializeWaypoints(swganh::messages::BaseSwgMessage* message);
	void SerializeWaypoints(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return the current force power of the player.
     */
    int32_t GetCurrentForcePower() ;
	int32_t GetCurrentForcePower(boost::unique_lock<boost::mutex>& lock) ;
    
    /**
     * Increments the force power of the player. Can be a negative value.
     *
     * @param force_power The amount of force to increment.
     */
    void IncrementForcePower(int32_t force_power);
	void IncrementForcePower(int32_t force_power, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Sets the current force power.
     *
     * @param force_power The new force power.
     */
    void SetCurrentForcePower(int32_t force_power);
	void SetCurrentForcePower(int32_t force_power, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return the max force power of the player.
     */
    int32_t GetMaxForcePower() ;
	int32_t GetMaxForcePower(boost::unique_lock<boost::mutex>& lock) ;
    
    /**
     * Sets the max force power.
     *
     * @param force_power The new max force power.
     */
    void SetMaxForcePower(int32_t force_power);
	void SetMaxForcePower(int32_t force_power, boost::unique_lock<boost::mutex>& lock);

    uint32_t GetCurrentForceSensitiveQuests();
	uint32_t GetCurrentForceSensitiveQuests(boost::unique_lock<boost::mutex>& lock);

    void AddCurrentForceSensitiveQuest(uint32_t quest_mask);
	void AddCurrentForceSensitiveQuest(uint32_t quest_mask, boost::unique_lock<boost::mutex>& lock);

    void RemoveCurrentForceSensitiveQuest(uint32_t quest_mask);
	void RemoveCurrentForceSensitiveQuest(uint32_t quest_mask, boost::unique_lock<boost::mutex>& lock);

    void ClearCurrentForceSensitiveQuests();
	void ClearCurrentForceSensitiveQuests(boost::unique_lock<boost::mutex>& lock);
    
    uint32_t GetCompletedForceSensitiveQuests();
	uint32_t GetCompletedForceSensitiveQuests(boost::unique_lock<boost::mutex>& lock);

    void AddCompletedForceSensitiveQuest(uint32_t quest_mask);
	void AddCompletedForceSensitiveQuest(uint32_t quest_mask, boost::unique_lock<boost::mutex>& lock);

    void RemoveCompletedForceSensitiveQuest(uint32_t quest_mask);
	void RemoveCompletedForceSensitiveQuest(uint32_t quest_mask, boost::unique_lock<boost::mutex>& lock);

    void ClearCompletedForceSensitiveQuests();
	void ClearCompletedForceSensitiveQuests(boost::unique_lock<boost::mutex>& lock);

    /**
     * @return The quests currently in the quest journal.
     */
	std::map<uint32_t, QuestJournalData> GetQuests() ;
	std::map<uint32_t, QuestJournalData> GetQuests(boost::unique_lock<boost::mutex>& lock) ;
    
    /**
     * Adds a quest to the journal.
     *
     * @param quest the quest to add.
     */
    void AddQuest(QuestJournalData quest);
	void AddQuest(QuestJournalData quest, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Removes a quest from the journal.
     *
     * @param quest to remove.
     */
    void RemoveQuest(QuestJournalData quest);
	void RemoveQuest(QuestJournalData quest, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Updates a quest in the journal.
     * 
     * @param quest to update.
     */
    void UpdateQuest(QuestJournalData quest);
	void UpdateQuest(QuestJournalData quest, boost::unique_lock<boost::mutex>& lock);

	void SerializeQuests(swganh::messages::BaseSwgMessage* message);
	void SerializeQuests(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return whether player can experiment or not.
     */
    uint32_t GetExperimentationFlag() ;
	uint32_t GetExperimentationFlag(boost::unique_lock<boost::mutex>& lock) ;

    /**
     * Sets the experimentation flag for the player.
     *
     * @param experimentation_flag The new experimentation flag.
     *
     * @TODO Consider making this interface boolean.
     */
    void SetExperimentationFlag(uint32_t experimentation_flag);
	void SetExperimentationFlag(uint32_t experimentation_flag, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return The current crafting stage of the player's crafting session.
     */
    uint32_t GetCraftingStage() ;
	uint32_t GetCraftingStage(boost::unique_lock<boost::mutex>& lock) ;
    
    /**
     * Sets the crafting stage for the player.
     *
     * @param crafting_stage the new crafting stage.
     */
    void SetCraftingStage(uint32_t crafting_stage);
	void SetCraftingStage(uint32_t crafting_stage, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return the ID of the nearest crafting station.
     */
    uint64_t GetNearestCraftingStation() ;
	uint64_t GetNearestCraftingStation(boost::unique_lock<boost::mutex>& lock) ;
    
    /**
     * Sets the id for the nearest crafting station.
     *
     * @param crafting_station_id ID of the nearest crafting station.
     */
    void SetNearestCraftingStation( uint64_t crafting_station_id);
	void SetNearestCraftingStation( uint64_t crafting_station_id, boost::unique_lock<boost::mutex>& lock);

    
    /**
     * @return The draft schematics assigned to this player.
     */
    std::vector<DraftSchematicData> GetDraftSchematics();
	std::vector<DraftSchematicData> GetDraftSchematics(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Adds a draft schematic.
     *
     * @param schematic the schematic to add.
     */
    void AddDraftSchematic(DraftSchematicData schematic);
	void AddDraftSchematic(DraftSchematicData schematic, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Removes a draft schematic.
     *
     * @param schematic_id The id of the schematic to remove.
     */
    void RemoveDraftSchematic(uint32_t schematic_id);
	void RemoveDraftSchematic(uint32_t schematic_id, boost::unique_lock<boost::mutex>& lock);
    
	void SerializeDraftSchematics(swganh::messages::BaseSwgMessage* message);
	void SerializeDraftSchematics(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    /**
     * Clears all draft schematics.
     */
    void ClearDraftSchematics();
    void ClearDraftSchematics(boost::unique_lock<boost::mutex>& lock);

    /**
     * @return The number of experimentation points available in the
     *  current crafting session.
     */
    uint32_t GetExperimentationPoints() ;
	uint32_t GetExperimentationPoints(boost::unique_lock<boost::mutex>& lock) ;

    /**
     * Adds experimentation points to the current crafting session.
     *
     * @param points experimentation points to add.
     */ 
    void AddExperimentationPoints(uint32_t points);
	void AddExperimentationPoints(uint32_t points, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Removes experimentation points from the current crafting session.
     *
     * @param points experimentation points to remove.
     */ 
    void RemoveExperimentationPoints(uint32_t points);
	void RemoveExperimentationPoints(uint32_t points, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Resets experimentation points for the current crafting session.
     *
     * @param points experimentation points to reset to.
     */ 
    void ResetExperimentationPoints(uint32_t points);
	void ResetExperimentationPoints(uint32_t points, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return the number of accomplishments attained by this player.
     */
    uint32_t GetAccomplishmentCounter() ;
    uint32_t GetAccomplishmentCounter(boost::unique_lock<boost::mutex>& lock) ;

    /**
     * Resets the accomplishment counter.
     *
     * @param counter New accomplishment count.
     */
    void ResetAccomplishmentCounter(uint32_t counter);
	void ResetAccomplishmentCounter(uint32_t counter, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Increments the accomplishment counter.
     */
    void IncrementAccomplishmentCounter();
	void IncrementAccomplishmentCounter(boost::unique_lock<boost::mutex>& lock);

    /**
     * @return the list of friends.
     */
    std::vector<std::string> GetFriends();
	std::vector<std::string> GetFriends(boost::unique_lock<boost::mutex>& lock);

    /**
     * Checks to see if the name is already a friend
     *
     * @param friend_name name of friend to check
     * @return bool true if the friend is found, false else
     */
    bool IsFriend(std::string friend_name);
	bool IsFriend(std::string friend_name, boost::unique_lock<boost::mutex>& lock);

    /**
     * Adds a player to the friend list.
     *
     * @param friend_name Name of the friend to add.
     */ 
    void AddFriend(std::string friend_name);
	void AddFriend(std::string friend_name, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Removes a friend from the friend list.
     *
     * @param friend_name Name of the friend to remove.
     */ 
    void RemoveFriend(std::string friend_name);
	void RemoveFriend(std::string friend_name, boost::unique_lock<boost::mutex>& lock);

	void SerializeFriends(swganh::messages::BaseSwgMessage* message);
	void SerializeFriends(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    /**
     * Clears the friends list.
     */
    void ClearFriends();
	void ClearFriends(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return the list of ignored players.
     */
    std::vector<std::string> GetIgnoredPlayers();
	std::vector<std::string> GetIgnoredPlayers(boost::unique_lock<boost::mutex>& lock);

    /**
     * Checks to see if the name is already being ignored
     *
     * @param name name to check
     * @return bool true if the friend is found, false else
     */
    bool IsIgnored(std::string player_name);
	bool IsIgnored(std::string player_name, boost::unique_lock<boost::mutex>& lock);

    /**
     * Adds a player to the ignored list.
     *
     * @param player_name Name of the player to ignore.
     */ 
    void IgnorePlayer(std::string player_name);
	void IgnorePlayer(std::string player_name, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Removes a player from the ignored list.
     *
     * @param player_name Name of the player to stop ignoring.
     */ 
    void StopIgnoringPlayer(std::string player_name);
    void StopIgnoringPlayer(std::string player_name, boost::unique_lock<boost::mutex>& lock);

	void SerializeIgnoredPlayers(swganh::messages::BaseSwgMessage* message);
	void SerializeIgnoredPlayers(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    /**
     * Clears the ignored list.
     */
    void ClearIgnored();
	void ClearIgnored(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return the current speaking language of the player.
     */
    uint32_t GetLanguage() ;
	uint32_t GetLanguage(boost::unique_lock<boost::mutex>& lock) ;

    void SetLanguage(uint32_t language_id);
	void SetLanguage(uint32_t language_id, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return the current stomach volume of the player.
     */
    uint32_t GetCurrentStomach() ;
	uint32_t GetCurrentStomach(boost::unique_lock<boost::mutex>& lock) ;
    
    /**
     * Increases the current stomach volume of the player.
     *
     * @param stomach Amount to increase the stomach volume.
     */ 
    void IncreaseCurrentStomach(uint32_t stomach);
	void IncreaseCurrentStomach(uint32_t stomach, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Decreases the current stomach volume of the player.
     *
     * @param stomach Amount to decrease the stomach volume.
     */ 
    void DecreaseCurrentStomach(uint32_t stomach);
	void DecreaseCurrentStomach(uint32_t stomach, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Resets the current stomach volume of the player.
     *
     * @param stomach The new stomach volume.
     */
    void ResetCurrentStomach(uint32_t stomach);
	void ResetCurrentStomach(uint32_t stomach, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return the max stomach volume of the player.
     */
	uint32_t GetMaxStomach() ;
    uint32_t GetMaxStomach(boost::unique_lock<boost::mutex>& lock) ;
    
    /**
     * Resets the max stomach volume of the player.
     *
     * @param stomach The new max stomach volume.
     */
    void ResetMaxStomach(uint32_t stomach);
	void ResetMaxStomach(uint32_t stomach, boost::unique_lock<boost::mutex>& lock);

    
    /**
     * @return the current drink volume of the player.
     */
    uint32_t GetCurrentDrink() ;
	uint32_t GetCurrentDrink(boost::unique_lock<boost::mutex>& lock) ;
    
    /**
     * Increases the current drink volume of the player.
     *
     * @param drink Amount to increase the drink volume.
     */ 
    void IncreaseCurrentDrink(uint32_t drink);
	void IncreaseCurrentDrink(uint32_t drink, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Decreases the current drink volume of the player.
     *
     * @param drink Amount to decrease the drink volume.
     */ 
    void DecreaseCurrentDrink(uint32_t drink);
	void DecreaseCurrentDrink(uint32_t drink, boost::unique_lock<boost::mutex>& lock);

    /**
     * Resets the current drink volume of the player.
     *
     * @param drink The new drink volume.
     */
    void ResetCurrentDrink(uint32_t drink);
	void ResetCurrentDrink(uint32_t drink, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return the max drink volume of the player.
     */
    uint32_t GetMaxDrink() ;
	uint32_t GetMaxDrink(boost::unique_lock<boost::mutex>& lock) ;

    /**
     * Resets the max drink volume of the player.
     *
     * @param drink The new max drink volume.
     */
    void ResetMaxDrink(uint32_t drink);
	void ResetMaxDrink(uint32_t drink, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return the current jedi state of the player.
     */
    uint32_t GetJediState() ;
	uint32_t GetJediState(boost::unique_lock<boost::mutex>& lock) ;

    /**
     * Sets the current jedi state for the player.
     *
     * @param jedi_state The jedi state for the player.
     */
    void SetJediState(uint32_t jedi_state);
	void SetJediState(uint32_t jedi_state, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return the gender of the player.
     */
    Gender GetGender() ;
	Gender GetGender(boost::unique_lock<boost::mutex>& lock) ;

    /**
     * Sets the gender of the player.
     *
     * @param gender The gender of the player.
     */
    void SetGender(Gender gender);
	void SetGender(Gender gender, boost::unique_lock<boost::mutex>& lock);

	/**
	 * Toggles badge.
	 *
	 * @param id The id of the badge to remove. (Use BadgeService to look-up badge id by name.)
	 */
	void RemoveBadge(uint32_t id);
	void RemoveBadge(uint32_t id, boost::unique_lock<boost::mutex>& lock);

	/**
	 * Adds a badge by id. (Use BadgeService to look-up badge id by name.)
	 *
	 * @param id Id of the badge you want to add.
	 */
	void AddBadge(uint32_t id);
	void AddBadge(uint32_t id, boost::unique_lock<boost::mutex>& lock);

	/**
	 * @param id Id of the badge you want to find. (Use BadgeService to look-up badge id by name.)
	 * 
	 * @return Check for badge.
	 */
	bool HasBadge(uint32_t id);
	bool HasBadge(uint32_t id, boost::unique_lock<boost::mutex>& lock);

	std::list<uint32_t> GetBadges();
	std::list<uint32_t> GetBadges(boost::unique_lock<boost::mutex>& lock);

	std::queue<std::pair<uint8_t, uint32_t>> GetBadgesSyncQueue();
	std::queue<std::pair<uint8_t, uint32_t>> GetBadgesSyncQueue(boost::unique_lock<boost::mutex>& lock);

    // baselines
    virtual void CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer);

    typedef swganh::ValueEvent<std::shared_ptr<Player>> PlayerEvent;

private:
    void SetDeltaBitmask_(uint32_t bitmask, uint16_t update_type, swganh::object::Object::ViewType view_type);

    std::array<uint32_t, 4> status_flags_;
    std::array<uint32_t, 4> profile_flags_;
    std::string profession_tag_;
    uint32_t born_date_;
    uint32_t total_playtime_;
    uint8_t admin_tag_;
    uint32_t region_;
    swganh::containers::NetworkMap<std::string, XpData, XpData> experience_;
    swganh::containers::NetworkMap<uint64_t, PlayerWaypointSerializer, PlayerWaypointSerializer> waypoints_;
    int32_t current_force_power_;
    int32_t max_force_power_;
    uint32_t current_force_sensitive_quests_;
    uint32_t completed_force_sensitive_quests_;
    swganh::containers::NetworkMap<uint32_t, QuestJournalData, QuestJournalData> quest_journal_;
    uint32_t experimentation_flag_;
    uint32_t crafting_stage_;
    uint64_t nearest_crafting_station_;
    swganh::containers::NetworkVector<DraftSchematicData> draft_schematics_;
    uint32_t experimentation_points_;
    swganh::containers::NetworkVector<std::string> friends_;
    swganh::containers::NetworkVector<std::string> ignored_players_;
    uint32_t accomplishment_counter_;
    uint32_t language_;
    uint32_t current_stomach_;
    uint32_t max_stomach_;
    uint32_t current_drink_;
    uint32_t max_drink_;
    uint32_t jedi_state_;
    Gender gender_;

	// Badges
	std::list<uint32_t> badges_;
	std::queue<std::pair<uint8_t, uint32_t>> badges_sync_queue_;
};

}}  // namespace swganh::object
