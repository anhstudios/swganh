#include "player_message_builder.h"

#include <algorithm>

#include "anh/crc.h"
#include "swganh/messages/deltas_message.h"
#include "swganh/messages/baselines_message.h"
#include "swganh/object/waypoint/waypoint.h"

using namespace std;
using namespace swganh::object;
using namespace player;
using namespace waypoint;
using namespace swganh::messages;

// deltas
void PlayerMessageBuilder::BuildStatusBitmaskDelta(Player* object){}
void PlayerMessageBuilder::BuildProfileBitmaskDelta(Player* object){}
void PlayerMessageBuilder::BuildProfessionTagDelta(Player* object){}
void PlayerMessageBuilder::BuildBornDateDelta(Player* object){}
void PlayerMessageBuilder::BuildPlayTimeDelta(Player* object){}
void PlayerMessageBuilder::BuildRegionIdDelta(Player* object){}
void PlayerMessageBuilder::BuildAdminTagDelta(Player* object){}
void PlayerMessageBuilder::BuildXpDelta(Player* object, uint8_t sub_type, std::string type, uint32_t amount){}
void PlayerMessageBuilder::BuildResetXpDelta(Player* object, std::vector<Player::XpData> experience){}
void PlayerMessageBuilder::BuildWaypointDelta(Player* object, uint8_t sub_type, Waypoint waypoint){}
void PlayerMessageBuilder::BuildResetWaypointDelta(Player* object, std::vector<Waypoint> waypoints){}
void PlayerMessageBuilder::BuildCurrentForcePowerDelta(Player* object){}
void PlayerMessageBuilder::BuildMaxForcePowerDelta(Player* object){}
void PlayerMessageBuilder::BuildForceSensitiveQuestDelta(Player* object){}
void PlayerMessageBuilder::BuildCompletedForceSensitiveQuestDelta(Player* object){}
void PlayerMessageBuilder::BuildQuestJournalDelta(Player* object, uint8_t sub_type, Player::QuestJournalData quest){}
void PlayerMessageBuilder::BuildResetQuestJournalDelta(Player* object, std::vector<Player::QuestJournalData> quests){}
void PlayerMessageBuilder::BuildAbilityDelta(Player* object, uint8_t sub_type, std::string ability){}
void PlayerMessageBuilder::BuildResetAbilityDelta(Player* object, std::vector<std::string> abilities){}
void PlayerMessageBuilder::BuildExperimentationFlagDelta(Player* object){}
void PlayerMessageBuilder::BuildCraftingStageDelta(Player* object){}
void PlayerMessageBuilder::BuildNearestCraftingStationDelta(Player* object){}
void PlayerMessageBuilder::BuildDraftSchematicDelta(Player* object, uint8_t sub_type, Player::DraftSchematicData schematic){}
void PlayerMessageBuilder::BuildResetDraftSchematicDelta(Player* object, std::vector<Player::DraftSchematicData> schematics){}
void PlayerMessageBuilder::BuildExperimentationPointsDelta(Player* object){}
void PlayerMessageBuilder::BuildAccomplishmentCounterDelta(Player* object){}
void PlayerMessageBuilder::BuildFriendsDelta(Player* object, uint8_t sub_type, std::string friend_name){}
void PlayerMessageBuilder::BuildResetFriendsDelta(Player* object, std::vector<std::string> friends){}
void PlayerMessageBuilder::BuildIgnoredDelta(Player* object, uint8_t sub_type, std::string ignored_name){}
void PlayerMessageBuilder::BuildResetIgnoredDelta(Player* object, std::vector<std::string> ignored_players){}
void PlayerMessageBuilder::BuildLanguageDelta(Player* object){}
void PlayerMessageBuilder::BuildCurrentStomachDelta(Player* object){}
void PlayerMessageBuilder::BuildMaxStomachDelta(Player* object){}
void PlayerMessageBuilder::BuildCurrentDrinkDelta(Player* object){}
void PlayerMessageBuilder::BuildMaxDrinkDelta(Player* object){}
void PlayerMessageBuilder::BuildJediStateDelta(Player* object){}
// baselines
boost::optional<swganh::messages::BaselinesMessage> PlayerMessageBuilder::BuildBaseline3(Player* object)
{
    auto message = object->CreateBaselinesMessage(Object::VIEW_3, 10);

    message.data.append(object->Object::GetBaseline3().get().data);
    // not used
    message.data.write(0);
    message.data.write(object->GetStatusFlags());
    message.data.write(object->GetProfileFlags());
    message.data.write(object->GetProfessionTag());
    message.data.write(object->GetBornDate());
    message.data.write(object->GetTotalPlayTime());

    return boost::optional<BaselinesMessage>(move(message));
}
boost::optional<swganh::messages::BaselinesMessage> PlayerMessageBuilder::BuildBaseline6(Player* object)
{
    auto message = object->CreateBaselinesMessage(Object::VIEW_6, 2);

    message.data.write(object->GetRegionId());
    message.data.write<uint8_t>(object->GetAdminTag());

    return boost::optional<BaselinesMessage>(move(message));
}
boost::optional<swganh::messages::BaselinesMessage> PlayerMessageBuilder::BuildBaseline8(Player* object)
{
    auto message = object->CreateBaselinesMessage(Object::VIEW_8, 7);
    
    // experience
    message.data.write(object->experience_.size());
    message.data.write(object->experience_counter_);
    for_each(begin(object->experience_), end(object->experience_), [&message] (Player::XpData xp){
        message.data.write(xp.type);
        message.data.write(xp.value);
    });
    // waypoints
    message.data.write(object->waypoints_.size());
    message.data.write(object->waypoint_counter_);
    for_each(begin(object->waypoints_), end(object->waypoints_), [&message] (Waypoint waypoint){
        message.data.write<uint8_t>(0);
        message.data.write<uint64_t>(waypoint.GetObjectId());
        // cell id
        message.data.write(0);
        auto position = waypoint.GetPosition();
        message.data.write(position.x);
        message.data.write(position.y);
        message.data.write(position.z);
        message.data.write(0);
        message.data.write(anh::memcrc(waypoint.GetPlanet()));
        message.data.write(waypoint.GetName());
        message.data.write(waypoint.GetObjectId());
        message.data.write<uint8_t>(waypoint.GetColorByte());
        message.data.write<uint8_t>(waypoint.GetActiveFlag());
    });
    // The Force
    message.data.write(object->GetCurrentForcePower());
    message.data.write(object->GetMaxForcePower());
    // TODO: Are these really lists or just bitmasks?
    message.data.write(object->GetCurrentForceSensitiveQuests());
    message.data.write(object->GetCompletedForceSensitiveQuests());
    // Quests
    message.data.write(object->quest_journal_.size());
    message.data.write(0);
    for_each(begin(object->quest_journal_), end(object->quest_journal_), [&message](Player::QuestJournalData quest){
        message.data.write<uint8_t>(0);
        message.data.write(quest.quest_crc);
        message.data.write(quest.owner_id);
        message.data.write(quest.active_step_bitmask);
        message.data.write(quest.completed_step_bitmask);
        message.data.write(quest.completed_flag);
        message.data.write(quest.counter);
    });
    return boost::optional<BaselinesMessage>(move(message));
}
boost::optional<swganh::messages::BaselinesMessage> PlayerMessageBuilder::BuildBaseline9(Player* object)
{
    auto message = object->CreateBaselinesMessage(Object::VIEW_9, 17);

    // Ability
    message.data.write(object->abilities_.size());
    message.data.write(0);
    for_each(begin(object->abilities_), end(object->abilities_), [&message](string ability){
        message.data.write(ability);
    });
    // Crafting
    message.data.write(object->GetExperimentationFlag());
    message.data.write(object->GetCraftingStage());
    message.data.write(object->GetNearestCraftingStation());
    // Schematics
    message.data.write(object->draft_schematics_.size());
    message.data.write(0);
    for_each(begin(object->draft_schematics_), end(object->draft_schematics_), [&message](Player::DraftSchematicData schem) {
        message.data.write(schem.schematic_id);
        message.data.write(schem.schematic_crc);
    });

    message.data.write(object->GetExperimentationPoints());
    message.data.write(object->GetAccomplishmentCounter());
    // friend list
    message.data.write(object->friends_.size());
    message.data.write(0);
    for_each(begin(object->friends_), end(object->friends_), [&message](string name){
        message.data.write(name);
    });
    // ignore list
    message.data.write(object->ignored_players_.size());
    message.data.write(0);
    for_each(begin(object->ignored_players_), end(object->ignored_players_), [&message] (string name) {
        message.data.write(name);
    });
    message.data.write(object->GetLanguage());
    message.data.write(object->GetCurrentStomach());
    message.data.write(object->GetMaxStomach());
    message.data.write(object->GetCurrentDrink());
    message.data.write(object->GetMaxDrink());
    // unused
    message.data.write(0);
    message.data.write(0);
    message.data.write(0);
    message.data.write(0);
    message.data.write(object->GetJediState());

    return boost::optional<BaselinesMessage>(move(message));
}