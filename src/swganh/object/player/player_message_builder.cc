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
void PlayerMessageBuilder::BuildStatusBitmaskDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(object->Object::VIEW_3, 5);
        message.data.write(object->GetStatusFlags());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildProfileBitmaskDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(object->Object::VIEW_3, 6);
        message.data.write(object->GetProfileFlags());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildProfessionTagDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(object->Object::VIEW_3, 7);
        message.data.write(object->GetProfessionTag());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildBornDateDelta(Player* object)
{
     if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(object->Object::VIEW_3, 8);
        message.data.write(object->GetBornDate());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildPlayTimeDelta(Player* object)
{
     if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(object->Object::VIEW_3, 9);
        message.data.write(object->GetTotalPlayTime());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildRegionIdDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(object->Object::VIEW_6, 0);
        message.data.write(object->GetRegionId());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildAdminTagDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(object->Object::VIEW_6, 1);
        message.data.write<uint8_t>(object->GetAdminTag());
        object->AddDeltasUpdate(std::move(message));
    }
}
void PlayerMessageBuilder::BuildXpDelta(Player* object, uint8_t sub_type, std::string type, uint32_t amount)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(object->Object::VIEW_7, 0);
        message.data.write(object->experience_.size());
        // list counter
        message.data.write(++object->experience_counter_);
        // subtype
        message.data.write<uint8_t>(sub_type);
        message.data.write(type);
        message.data.write(amount);

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildResetXpDelta(Player* object, std::vector<Player::XpData> experience)
{
    // do the delta update
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_8, 0);
        // @TODO: Check if there is a reset
        message.data.write<uint16_t>(2);
        message.data.write(object->experience_.size());
        message.data.write(object->experience_.size());
        for_each(begin(object->experience_), end(object->experience_),[&message](Player::XpData xp){
            message.data.write(xp.type);
            message.data.write(xp.value);
        });

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildWaypointDelta(Player* object, uint8_t sub_type, shared_ptr<Waypoint> waypoint)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(object->Object::VIEW_7, 1);
        message.data.write(object->waypoints_.size());
        // list counter
        message.data.write(++object->waypoint_counter_);
        // subtype
        message.data.write<uint8_t>(sub_type);
        message.data.write(waypoint->GetObjectId());
        // TODO: Add CELL ID?
        message.data.write(0);
        auto position = waypoint->GetPosition();
        message.data.write(position.x);
        message.data.write(position.z);
        message.data.write(position.y);
        // TODO: network id
        message.data.write<uint64_t>(0);
        message.data.write(anh::memcrc(waypoint->GetPlanet()));
        message.data.write(waypoint->GetName());
        message.data.write(waypoint->GetObjectId());
        message.data.write<uint8_t>(waypoint->GetColorByte());
        message.data.write<uint8_t>(waypoint->GetActiveFlag());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildResetWaypointDelta(Player* object, std::vector<shared_ptr<Waypoint>> waypoints)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(object->Object::VIEW_8, 1);
        message.data.write(object->waypoints_.size());
        message.data.write(object->waypoints_.size());
        message.data.write<uint16_t>(2);
        std::for_each(begin(object->waypoints_), end(object->waypoints_), [&message](shared_ptr<Waypoint> waypoint){
            // waypoint
            message.data.write<uint64_t>(waypoint->GetObjectId());
            // cell id
            message.data.write(0);
            auto position = waypoint->GetPosition();
            message.data.write(position.x);
            message.data.write(position.y);
            message.data.write(position.z);
            message.data.write(0);
            message.data.write(anh::memcrc(waypoint->GetPlanet()));
            message.data.write(waypoint->GetName());
            message.data.write(waypoint->GetObjectId());
            message.data.write<uint8_t>(waypoint->GetColorByte());
            message.data.write<uint8_t>(waypoint->GetActiveFlag());
        });

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildCurrentForcePowerDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_8, 2);
        message.data.write(object->GetCurrentForcePower());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildMaxForcePowerDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_8, 3);
        message.data.write(object->GetMaxForcePower());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildForceSensitiveQuestDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_8, 4);
        message.data.write(object->GetCurrentForceSensitiveQuests());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildCompletedForceSensitiveQuestDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_8, 5);
        message.data.write(object->GetCompletedForceSensitiveQuests());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildQuestJournalDelta(Player* object, uint8_t sub_type, Player::QuestJournalData quest)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_8, 6);
        // subtype
        message.data.write<uint8_t>(sub_type);
        message.data.write(quest.quest_crc);
        message.data.write(quest.owner_id);
        message.data.write(quest.active_step_bitmask);
        message.data.write(quest.completed_step_bitmask);
        message.data.write(quest.completed_flag);
        message.data.write(quest.counter);        

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildResetQuestJournalDelta(Player* object, std::vector<Player::QuestJournalData> quests)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_8, 6);
        // update counter
        message.data.write<uint16_t>(1);
        message.data.write<uint8_t>(3);
        std::for_each(begin(quests), end(quests), [&message] (Player::QuestJournalData quest){
            message.data.write(quest.quest_crc);
            message.data.write(quest.owner_id);
            message.data.write(quest.active_step_bitmask);
            message.data.write(quest.completed_step_bitmask);
            message.data.write(quest.completed_flag);
            message.data.write(quest.counter);   
        });     

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildAbilityDelta(Player* object, uint8_t sub_type, uint16_t index, std::string ability)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 0);
        message.data.write(object->abilities_.size());
        // list counter
        message.data.write(++object->abilities_counter_);
        // subtype
        message.data.write<uint8_t>(sub_type);
        switch (sub_type)
        {
        case 0:
            message.data.write(index);
            break;
        case 1:
        case 2:
            message.data.write(index);
            message.data.write(ability);
            break;
        case 4:
             break;
        }

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildResetAbilityDelta(Player* object, std::vector<std::string> abilities)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 0);
        message.data.write(object->abilities_.size());
        // list counter
        message.data.write(++object->abilities_counter_);
        // subtype
        message.data.write<uint8_t>(3);
        for_each(begin(abilities), end(abilities), [&message] (string ability) {
            message.data.write(ability);
        });
        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildExperimentationFlagDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 1);
        message.data.write(object->GetExperimentationFlag());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildCraftingStageDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 2);
        message.data.write(object->GetCraftingStage());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildNearestCraftingStationDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 3);
        message.data.write(object->GetNearestCraftingStation());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildDraftSchematicDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 4);
        object->draft_schematics_.Serialize(message);
        object->AddDeltasUpdate(move(message));
    }
}

void PlayerMessageBuilder::BuildExperimentationPointsDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 5);
        message.data.write(object->GetExperimentationPoints());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildAccomplishmentCounterDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 6);
        message.data.write(object->GetAccomplishmentCounter());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildFriendsDelta(Player* object, uint8_t sub_type, uint16_t index, std::string friend_name){}
void PlayerMessageBuilder::BuildResetFriendsDelta(Player* object, std::vector<std::string> friends){}
void PlayerMessageBuilder::BuildIgnoredDelta(Player* object, uint8_t sub_type, uint16_t index, std::string ignored_name){}
void PlayerMessageBuilder::BuildResetIgnoredDelta(Player* object, std::vector<std::string> ignored_players){}
void PlayerMessageBuilder::BuildLanguageDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 9);
        message.data.write(object->GetLanguage());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildCurrentStomachDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 10);
        message.data.write(object->GetCurrentStomach());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildMaxStomachDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 11);
        message.data.write(object->GetMaxStomach());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildCurrentDrinkDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 12);
        message.data.write(object->GetCurrentDrink());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildMaxDrinkDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 13);
        message.data.write(object->GetMaxDrink());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildJediStateDelta(Player* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_9, 17);
        message.data.write(object->GetExperimentationPoints());            

        object->AddDeltasUpdate(move(message));
    }
}
// baselines
boost::optional<swganh::messages::BaselinesMessage> PlayerMessageBuilder::BuildBaseline3(Player* object)
{
    auto message = object->CreateBaselinesMessage(Object::VIEW_3, 10);
    message.data.append(object->Object::GetBaseline3().get().data);
    message.data.write<uint32_t>(0);                                    // Not Used
    message.data.write<uint32_t>(object->GetStatusFlags());             // Status Flag
    message.data.write<uint32_t>(object->GetProfileFlags());            // Profile Flags
    message.data.write<std::string>(object->GetProfessionTag());        // Profession Tag
    message.data.write<uint32_t>(object->GetBornDate());                // Born Date
    message.data.write<uint32_t>(object->GetTotalPlayTime());           // Total Play Time
    return boost::optional<BaselinesMessage>(move(message));
}
boost::optional<swganh::messages::BaselinesMessage> PlayerMessageBuilder::BuildBaseline6(Player* object)
{
    auto message = object->CreateBaselinesMessage(Object::VIEW_6, 2);
    message.data.write<uint32_t>(object->GetRegionId());    // Region Id
    message.data.write<uint8_t>(object->GetAdminTag());     // Admin Tag
    return boost::optional<BaselinesMessage>(move(message));
}
boost::optional<swganh::messages::BaselinesMessage> PlayerMessageBuilder::BuildBaseline8(Player* object)
{

    auto message = object->CreateBaselinesMessage(Object::VIEW_8, 7);
    message.data.write<uint32_t>(1); // Experience List Size
    message.data.write<uint32_t>(0); // Experience List Counter
    message.data.write<uint8_t>(0);
    message.data.write<std::string>("combat_general");
    message.data.write<uint32_t>(100);
    message.data.write<uint32_t>(0); // Waypoint List Size
    message.data.write<uint32_t>(0); // Waypoint List Counter

    message.data.write<uint32_t>(object->GetCurrentForcePower());
    message.data.write<uint32_t>(object->GetMaxForcePower());
    
    message.data.write<uint32_t>(0); // Force Sensetive Quest List Soze
    message.data.write<uint32_t>(0); // Force Sensetive Quest List Counter

    message.data.write<uint32_t>(0); // Completed Force Sensetive Quest List Size
    message.data.write<uint32_t>(0); // Completed Force Sensetive Quest List Counter

    message.data.write<uint32_t>(0); // Quest List Size
    message.data.write<uint32_t>(0); // Quest List Counter

    return boost::optional<BaselinesMessage>(std::move(message));
}
boost::optional<swganh::messages::BaselinesMessage> PlayerMessageBuilder::BuildBaseline9(Player* object)
{
    auto message = object->CreateBaselinesMessage(Object::VIEW_9, 17);

    message.data.write<uint32_t>(0); // Ability List Size
    message.data.write<uint32_t>(0); // Ability List Counter
    message.data.write<uint32_t>(object->GetExperimentationFlag());
    message.data.write<uint32_t>(object->GetCraftingStage());
    message.data.write<uint64_t>(object->GetNearestCraftingStation());

    object->draft_schematics_.Serialize(message);


    message.data.write<uint32_t>(object->GetExperimentationPoints());
    message.data.write<uint32_t>(object->GetAccomplishmentCounter());
 
    message.data.write<uint32_t>(0); // Friends List Size
    message.data.write<uint32_t>(0); // Firends List Counter

    message.data.write<uint32_t>(0); // Ignore List Size
    message.data.write<uint32_t>(0); // Ignore List Counter

    message.data.write<uint32_t>(object->GetLanguage());
    message.data.write<uint32_t>(object->GetCurrentStomach());
    message.data.write<uint32_t>(object->GetMaxStomach());
    message.data.write<uint32_t>(object->GetCurrentDrink());
    message.data.write<uint32_t>(object->GetMaxDrink());
    message.data.write<uint32_t>(0); // Unused
    message.data.write<uint32_t>(0); // Unused
    message.data.write<uint32_t>(0); // Unused
    message.data.write<uint32_t>(0); // Unused
    message.data.write<uint32_t>(object->GetJediState()); // Jedi State

    return boost::optional<BaselinesMessage>(std::move(message));
}