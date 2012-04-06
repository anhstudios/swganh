#include "player_message_builder.h"

#include <algorithm>

#include "anh/logger.h"
#include "anh/crc.h"
#include "swganh/messages/deltas_message.h"
#include "swganh/messages/baselines_message.h"
#include "swganh/messages/scene_end_baselines.h"
#include "swganh/object/waypoint/waypoint.h"

using namespace anh;
using namespace std;
using namespace swganh::object;
using namespace player;
using namespace waypoint;
using namespace swganh::messages;

void PlayerMessageBuilder::RegisterEventHandlers()
{
    // TODO: Register event handlers for player
    event_dispatcher->Subscribe("Player::Baselines", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        SendBaselines(value_event->Get());
    });
}

void PlayerMessageBuilder::SendBaselines(shared_ptr<Player> player)
{
    player->AddBaselineToCache(BuildBaseline3(player));
    player->AddBaselineToCache(BuildBaseline6(player));
    player->AddBaselineToCache(BuildBaseline8(player));
    player->AddBaselineToCache(BuildBaseline9(player));
    
    if (player->HasController())
    {
        auto controller = player->GetController();
        for (auto& baseline : player->GetBaselines())
        {
            controller->Notify(baseline);
        }
        
        SendEndBaselines(player);
    }
    else if (player->HasObservers())
    {
        for (auto& baseline : player->GetBaselines())
        {
            player->NotifyObservers(baseline);
        }
        
        SendEndBaselines(player, true);     
    }
}

// deltas
void PlayerMessageBuilder::BuildStatusBitmaskDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 5);

        auto status_flags = object->GetStatusFlags();

        message.data.write<uint32_t>(status_flags.size());

        for_each(begin(status_flags),
            end(status_flags),
            [&message] (vector<FlagBitmask>::value_type& flag)
        {
            flag.Serialize(message);
        });
        
        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildProfileBitmaskDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 6);
        
        auto profile_flags = object->GetProfileFlags();

        message.data.write<uint32_t>(profile_flags.size());

        for_each(begin(profile_flags),
            end(profile_flags),
            [&message] (vector<FlagBitmask>::value_type& flag)
        {
            flag.Serialize(message);
        });

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildProfessionTagDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 7);
        message.data.write(object->GetProfessionTag());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildBornDateDelta(shared_ptr<Player> object)
{
     if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 8);
        message.data.write(object->GetBornDate());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildPlayTimeDelta(shared_ptr<Player> object)
{
     if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 9);
        message.data.write(object->GetTotalPlayTime());

        object->AddDeltasUpdate(move(message));
    }
}

void PlayerMessageBuilder::BuildAdminTagDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_6, 1);
        message.data.write<uint8_t>(object->GetAdminTag());
        object->AddDeltasUpdate(std::move(message));
    }
}
void PlayerMessageBuilder::BuildXpDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_7, 0);
        object->GetXp().Serialize(message);
        object->AddDeltasUpdate(move(message));
    }
    else
        object->GetXp().ClearDeltas();
}
void PlayerMessageBuilder::BuildWaypointDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_7, 1);
        object->GetWaypoints().Serialize(message);
        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildCurrentForcePowerDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_8, 2);
        message.data.write(object->GetCurrentForcePower());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildMaxForcePowerDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_8, 3);
        message.data.write(object->GetMaxForcePower());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildForceSensitiveQuestDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_8, 4);
        message.data.write(object->GetCurrentForceSensitiveQuests());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildCompletedForceSensitiveQuestDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_8, 5);
        message.data.write(object->GetCompletedForceSensitiveQuests());

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildQuestJournalDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_8, 6);
        object->GetQuests().Serialize(message);
        object->AddDeltasUpdate(move(message));
    }
    else
        object->GetQuests().ClearDeltas();
}
void PlayerMessageBuilder::BuildAbilityDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 0);
        object->GetAbilityList().Serialize(message);
        object->AddDeltasUpdate(move(message));
    }
}

void PlayerMessageBuilder::BuildExperimentationFlagDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 1);
        message.data.write(object->GetExperimentationFlag());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildCraftingStageDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 2);
        message.data.write(object->GetCraftingStage());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildNearestCraftingStationDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 3);
        message.data.write(object->GetNearestCraftingStation());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildDraftSchematicDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 4);
        object->GetDraftSchematics().Serialize(message);
        object->AddDeltasUpdate(move(message));
    }
    else
        object->GetDraftSchematics().ClearDeltas();
}

void PlayerMessageBuilder::BuildExperimentationPointsDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 5);
        message.data.write(object->GetExperimentationPoints());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildAccomplishmentCounterDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 6);
        message.data.write(object->GetAccomplishmentCounter());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildFriendsDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 7);
        object->GetFriends().Serialize(message);
        object->AddDeltasUpdate(move(message));
    }
    else
        object->GetFriends().ClearDeltas();
}
void PlayerMessageBuilder::BuildIgnoredDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 8);
        object->GetIgnoredPlayers().Serialize(message);
        object->AddDeltasUpdate(move(message));
    }
    else
        object->GetIgnoredPlayers().ClearDeltas();
}
void PlayerMessageBuilder::BuildLanguageDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 9);
        message.data.write(object->GetLanguage());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildCurrentStomachDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 10);
        message.data.write(object->GetCurrentStomach());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildMaxStomachDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 11);
        message.data.write(object->GetMaxStomach());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildCurrentDrinkDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 12);
        message.data.write(object->GetCurrentDrink());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildMaxDrinkDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 13);
        message.data.write(object->GetMaxDrink());            

        object->AddDeltasUpdate(move(message));
    }
}
void PlayerMessageBuilder::BuildJediStateDelta(shared_ptr<Player> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 17);
        message.data.write(object->GetExperimentationPoints());            

        object->AddDeltasUpdate(move(message));
    }
}
// baselines
BaselinesMessage PlayerMessageBuilder::BuildBaseline3(shared_ptr<Player> object)
{
    auto message = CreateBaselinesMessage(object, Object::VIEW_3, 10);
    message.data.append(ObjectMessageBuilder::BuildBaseline3(object).data);
    message.data.write<uint32_t>(0);                                    // Not Used
    
    auto status_flags = object->GetStatusFlags();

    message.data.write<uint32_t>(status_flags.size());

    for_each(begin(status_flags),
        end(status_flags),
        [&message] (vector<FlagBitmask>::value_type& flag)
    {
        flag.Serialize(message);
    });

    auto profile_flags = object->GetProfileFlags();

    message.data.write<uint32_t>(profile_flags.size());

    for_each(begin(profile_flags),
        end(profile_flags),
        [&message] (vector<FlagBitmask>::value_type& flag)
    {
        flag.Serialize(message);
    });

    message.data.write<std::string>(object->GetProfessionTag());        // Profession Tag
    message.data.write<uint32_t>(object->GetBornDate());                // Born Date
    message.data.write<uint32_t>(object->GetTotalPlayTime());           // Total Play Time
    return BaselinesMessage(move(message));
}

BaselinesMessage PlayerMessageBuilder::BuildBaseline6(shared_ptr<Player> object)
{
    auto message = CreateBaselinesMessage(object, Object::VIEW_6, 2);
    message.data.write<uint32_t>(object->GetSceneId());    // Region Id
    message.data.write<uint8_t>(object->GetAdminTag());     // Admin Tag
    return BaselinesMessage(move(message));
}

BaselinesMessage PlayerMessageBuilder::BuildBaseline8(shared_ptr<Player> object)
{
    auto message = CreateBaselinesMessage(object, Object::VIEW_8, 7);
    object->GetXp().Serialize(message);
    object->GetWaypoints().Serialize(message);
    
    message.data.write<uint32_t>(object->GetCurrentForcePower());
    message.data.write<uint32_t>(object->GetMaxForcePower());
    
    message.data.write<uint32_t>(0); // Force Sensetive Quest List Soze
    message.data.write<uint32_t>(0); // Force Sensetive Quest List Counter
    
    message.data.write<uint32_t>(0); // Completed Force Sensetive Quest List Size
    message.data.write<uint32_t>(0); // Completed Force Sensetive Quest List Counter
    
    object->GetQuests().Serialize(message);
    
    return BaselinesMessage(move(message));
}

BaselinesMessage PlayerMessageBuilder::BuildBaseline9(shared_ptr<Player> object)
{
    auto message = CreateBaselinesMessage(object, Object::VIEW_9, 17);
    
    object->GetAbilityList().Serialize(message);
    message.data.write<uint32_t>(object->GetExperimentationFlag());
    message.data.write<uint32_t>(object->GetCraftingStage());
    message.data.write<uint64_t>(object->GetNearestCraftingStation());
    
    object->GetDraftSchematics().Serialize(message);
    
    message.data.write<uint32_t>(object->GetExperimentationPoints());
    message.data.write<uint32_t>(object->GetAccomplishmentCounter());
    
    object->GetFriends().Serialize(message);
    
    object->GetIgnoredPlayers().Serialize(message);
    
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
    
    return BaselinesMessage(message);
}