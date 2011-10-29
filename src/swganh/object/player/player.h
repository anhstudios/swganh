#ifndef SWGANH_OBJECT_PLAYER_PLAYER_H_
#define SWGANH_OBJECT_PLAYER_PLAYER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>

#include "anh/crc.h"

#include "swganh/object/object.h"
#include "swganh/object/waypoint/waypoint.h"

#include "swganh/messages/containers/network_sorted_list.h"
#include "swganh/messages/containers/network_array.h"
#include "swganh/messages/containers/network_map.h"
#include "swganh/messages/containers/network_sorted_vector.h"

namespace swganh {
namespace object {
namespace player {

enum Gender
{
    FEMALE = 0,
    MALE
};

enum StatusFlags
{
    CLEAR = 0,
    LFG = 1,
    HELPER = 2,
    ROLEPLAYER = 4,
    AFK = 80,
    LD = 100,
    FACTION_RANK = 200,
    ANONYMOUS = 80000000
};
enum StatusIndex
{
    DEFAULT = 0,
    ANON = 4
};
// TODO: discover these
enum ProfileFlags
{
    LIKES_X = 1,
    DISLIKES_X
};

enum AdminTag
{
    CSR = 1,
    DEVELOPER = 2
};
struct Ability
{
    Ability(void)
        : ability("")
    {}

    Ability(std::string value_)
        : ability(value_)
    {}

    ~Ability()
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<std::string>(ability);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<std::string>(ability);
    }
    bool operator==(const Ability& other)
    {
        return ability == other.ability;
    }

    std::string ability;
};
struct Name
{
    Name(void)
        : name("")
    {}

    Name(const std::string& value_)
        : name(value_)
    {}

    ~Name()
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<std::string>(name);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<std::string>(name);
    }
    bool operator==(const Name& other)
    {
        return name == other.name;
    }

    std::string name;
};
struct XpData
{
    XpData()
        : type("")
        , value(0){}
    XpData(std::string in_type, uint32_t in_value)
    : type(in_type)
    , value(in_value){}

    std::string type;
    uint32_t value;

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint8_t>(0); // byte
        message.data.write(type);
        message.data.write(value);           
    }
    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write(type);
        message.data.write(value);
    }
    bool operator==(const XpData& other)
    {
        return (type == other.type && value == other.value);
    }
};
struct FlagBitmask
{
    FlagBitmask()
        : bitmask(0)
    {}

    FlagBitmask(uint32_t flag_bitmask_)
        : bitmask(flag_bitmask_)
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint32_t>(bitmask);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint32_t>(bitmask);
    }

    bool operator==(const FlagBitmask& other)
    {
        return other.bitmask == bitmask;
    }

    uint32_t bitmask;
};

struct QuestJournalData
{
    QuestJournalData()
        : quest_crc(0)
        , owner_id(0)
        , active_step_bitmask(0)
        , completed_step_bitmask(0)
        , completed_flag(false)
        , counter(0){}

    uint32_t quest_crc;
    uint64_t owner_id;
    uint16_t active_step_bitmask;
    uint16_t completed_step_bitmask;
    bool completed_flag;
    uint32_t counter;

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint8_t>(0); // byte
        message.data.write<uint32_t>(quest_crc);
        message.data.write<uint64_t>(owner_id);
        message.data.write<uint16_t>(active_step_bitmask);
        message.data.write<uint16_t>(completed_step_bitmask);
        if (completed_flag)
            message.data.write<uint8_t>(1);
        else
            message.data.write<uint8_t>(0);
        message.data.write<uint32_t>(counter);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint32_t>(quest_crc);
        message.data.write<uint64_t>(owner_id);
        message.data.write<uint16_t>(active_step_bitmask);
        message.data.write<uint16_t>(completed_step_bitmask);
        if (completed_flag)
            message.data.write<uint8_t>(1);
        else
            message.data.write<uint8_t>(0);
        message.data.write<uint32_t>(counter);
    }

    bool operator==(const QuestJournalData& other)
    {
        return (quest_crc == other.quest_crc);
    }
};

struct DraftSchematicData
{
    DraftSchematicData()
        : schematic_id(0)
        , schematic_crc(0)
    {}

    DraftSchematicData(uint32_t schematic_id_)
        : schematic_id(schematic_id_)
        , schematic_crc(0)
    {}

    DraftSchematicData(uint32_t schematic_id_, uint32_t schematic_crc_)
        : schematic_id(schematic_id_)
        , schematic_crc(schematic_crc_)
    {}

    ~DraftSchematicData()
    {}

    uint32_t schematic_id;
    uint32_t schematic_crc;

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint32_t>(schematic_id);
        message.data.write<uint32_t>(schematic_crc);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint32_t>(schematic_id);
        message.data.write<uint32_t>(schematic_crc);
    }

    bool operator==(const DraftSchematicData& other)
    {
        return schematic_id == other.schematic_id;
    }
};
struct WaypointData
{
    WaypointData()
        : object_id_(0)
        , cell_id_(0)
        , coordinates_(glm::vec3(0,0,0))
        , location_network_id_(0)
        , planet_name_("")
        , name_(L"")
        , color_(0)
        , activated_flag_(0) {}

    WaypointData(uint64_t object_id, uint32_t cell_id, glm::vec3 coords, uint64_t network_id, std::string planet_name,
        std::wstring name, uint8_t color, uint8_t activated_flag)
        : object_id_(object_id)
        , cell_id_(cell_id)
        , coordinates_(std::move(coords))
        , location_network_id_(network_id)
        , planet_name_(planet_name)
        , name_(name)
        , color_(color)
        , activated_flag_(activated_flag) {}
    
    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint64_t>(object_id_);
        message.data.write<uint32_t>(cell_id_);
        message.data.write<float>(coordinates_.x);
        message.data.write<float>(coordinates_.y);
        message.data.write<float>(coordinates_.z);
        message.data.write<uint64_t>(location_network_id_);
        message.data.write<uint32_t>(anh::memcrc(planet_name_));
        message.data.write<std::wstring>(name_);
        message.data.write<uint64_t>(object_id_);
        message.data.write<uint8_t>(color_);
        message.data.write<uint8_t>(activated_flag_);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint64_t>(object_id_);
        message.data.write<uint32_t>(cell_id_);
        message.data.write<float>(coordinates_.x);
        message.data.write<float>(coordinates_.y);
        message.data.write<float>(coordinates_.z);
        message.data.write<uint64_t>(location_network_id_);
        message.data.write<uint32_t>(anh::memcrc(planet_name_));
        message.data.write<std::wstring>(name_);
        message.data.write<uint64_t>(object_id_);
        message.data.write<uint8_t>(color_);
        message.data.write<uint8_t>(activated_flag_);
    }

    bool operator==(const WaypointData& other)
    {
        return object_id_ == other.object_id_;
    }

    uint64_t object_id_;
    uint32_t cell_id_;
    glm::vec3 coordinates_;
    uint64_t location_network_id_;
    std::string planet_name_;
    std::wstring name_;
    uint8_t not_used_;
    uint8_t color_;
    uint8_t activated_flag_;
};

class PlayerMessageBuilder;

class Player : public swganh::object::Object
{
public:
    Player();
    
    
    virtual uint32_t GetType() const { return Player::type; }
    const static uint32_t type = 0x504c4159;

    std::vector<FlagBitmask> GetStatusFlags();
    void AddStatusFlag(StatusFlags flag, StatusIndex index = DEFAULT);
    void RemoveStatusFlag(StatusFlags flag, StatusIndex index = DEFAULT);
    void ClearStatusFlags();
    
    std::vector<FlagBitmask> GetProfileFlags() ;
    void AddProfileFlag(ProfileFlags flag, StatusIndex index = DEFAULT);
    void RemoveProfileFlag(ProfileFlags flag, StatusIndex index = DEFAULT);
    void ClearProfileFlags();

    std::string GetProfessionTag() ;
    void SetProfessionTag(std::string profession_tag);

    uint32_t GetBornDate() ;
    void SetBornDate(uint32_t born_date);

    uint32_t GetTotalPlayTime() ;
    void SetTotalPlayTime(uint32_t play_time);
    void IncrementTotalPlayTime(uint32_t increment);

    uint8_t GetAdminTag() ;
    void SetAdminTag(uint8_t tag);

    uint32_t GetRegionId() ;
    void SetRegionId(uint32_t region);

    swganh::messages::containers::NetworkMap<std::string, XpData> GetXp() ;
    void AddExperience(XpData experience);
    void DeductXp(XpData experience);
    void ClearXpType(std::string type);
    void ResetXp(swganh::messages::containers::NetworkMap<std::string, XpData>& experience);
    void ClearAllXp();
    
    swganh::messages::containers::NetworkMap<uint64_t, WaypointData> GetWaypoints() ;
    void AddWaypoint(WaypointData waypoint);
    void RemoveWaypoint(uint64_t waypoint_id);
    void ModifyWaypoint(WaypointData waypoint);
    void ClearAllWaypoints();

    uint32_t GetCurrentForcePower() ;
    void IncreaseForcePower(uint32_t force_power);
    void DecreaseForcePower(uint32_t force_power);
    void SetCurrentForcePower(uint32_t force_power);
    
    uint32_t GetMaxForcePower() ;
    void SetMaxForcePower(uint32_t force_power);

    uint32_t GetCurrentForceSensitiveQuests();
    void AddCurrentForceSensitiveQuest(uint32_t quest_mask);
    void RemoveCurrentForceSensitiveQuest(uint32_t quest_mask);
    void ClearCurrentForceSensitiveQuests();
    
    uint32_t GetCompletedForceSensitiveQuests();
    void AddCompletedForceSensitiveQuest(uint32_t quest_mask);
    void RemoveCompletedForceSensitiveQuest(uint32_t quest_mask);
    void ClearCompletedForceSensitiveQuests();

    swganh::messages::containers::NetworkMap<uint32_t, QuestJournalData> GetQuests() ;
    void AddQuest(QuestJournalData quest);
    void RemoveQuest(QuestJournalData quest);
    void UpdateQuest(QuestJournalData quest);
    void ClearAllQuests();
    
    swganh::messages::containers::NetworkSortedList<Ability> GetAbilityList() ;
    void AddAbility(std::string ability);
    void RemoveAbility(std::string ability);
    void ClearAllAbilities();
    
    uint32_t GetExperimentationFlag() ;
    void SetExperimentationFlag(uint32_t experimentation_flag);
    
    uint32_t GetCraftingStage() ;
    void SetCraftingStage(uint32_t crafting_stage);

    uint64_t GetNearestCraftingStation() ;
    void SetNearestCraftingStation( uint64_t crafting_station_id);
    
    swganh::messages::containers::NetworkSortedList<DraftSchematicData> GetDraftSchematics() ;
    void AddDraftSchematic(DraftSchematicData schematic);
    void RemoveDraftSchematic(uint32_t schematic_id);
    void ClearDraftSchematics();

    uint32_t GetExperimentationPoints() ;
    void AddExperimentationPoints(uint32_t points);
    void RemoveExperimentationPoints(uint32_t points);
    void ResetExperimentationPoints(uint32_t points);
    
    uint32_t GetAccomplishmentCounter() ;
    void ResetAccomplishmentCounter(uint32_t counter);
    void IncrementAccomplishmentCounter();

    swganh::messages::containers::NetworkSortedVector<Name> GetFriends();
    void AddFriend(std::string friend_name);
    void RemoveFriend(std::string friend_name);
    void ClearFriends();
    
    swganh::messages::containers::NetworkSortedVector<Name> GetIgnoredPlayers();
    void IgnorePlayer(std::string player_name);
    void StopIgnoringPlayer(std::string player_name);
    void ClearIgnored();
    
    uint32_t GetLanguage() ;
    void SetLanguage(uint32_t language_id);
    
    uint32_t GetCurrentStomach() ;
    void IncreaseCurrentStomach(uint32_t stomach);
    void DecreaseCurrentStomach(uint32_t stomach);
    void ResetCurrentStomach(uint32_t stomach);

    uint32_t GetMaxStomach() ;
    void ResetMaxStomach(uint32_t stomach);
    
    uint32_t GetCurrentDrink() ;
    void IncreaseCurrentDrink(uint32_t drink);
    void DecreaseCurrentDrink(uint32_t drink);
    void ResetCurrentDrink(uint32_t drink);

    uint32_t GetMaxDrink() ;
    void ResetMaxDrink(uint32_t drink);
    
    uint32_t GetJediState() ;
    void SetJediState(uint32_t jedi_state);

    // Gender Set during creation
    Gender GetGender() ;
    void SetGender(Gender value);

    // baselines
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline3();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline6();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline8();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline9();

private:
    friend PlayerMessageBuilder;

    void SetDeltaBitmask_(uint32_t bitmask, uint16_t update_type, swganh::object::Object::ViewType view_type);

    std::vector<FlagBitmask> status_flags_;
    std::vector<FlagBitmask> profile_flags_;
    std::string profession_tag_;
    uint32_t born_date_;
    uint32_t total_playtime_;
    uint8_t admin_tag_;
    uint32_t region_;
    swganh::messages::containers::NetworkMap<std::string, XpData> experience_;
    uint32_t experience_counter_;
    swganh::messages::containers::NetworkMap<uint64_t, WaypointData> waypoints_;
    uint32_t waypoint_counter_;
    uint32_t current_force_power_;
    uint32_t max_force_power_;
    uint32_t current_force_sensitive_quests_;
    uint32_t completed_force_sensitive_quests_;
    swganh::messages::containers::NetworkMap<uint32_t, QuestJournalData> quest_journal_;
    swganh::messages::containers::NetworkSortedList<Ability> abilities_;
    uint32_t experimentation_flag_;
    uint32_t crafting_stage_;
    uint64_t nearest_crafting_station_;
    swganh::messages::containers::NetworkSortedList<DraftSchematicData> draft_schematics_;
    uint32_t experimentation_points_;
    uint32_t accomplishment_counter_;
    swganh::messages::containers::NetworkSortedVector<Name> friends_;
    swganh::messages::containers::NetworkSortedVector<Name> ignored_players_;
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
