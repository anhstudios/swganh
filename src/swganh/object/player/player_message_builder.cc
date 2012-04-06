#include "player_message_builder.h"

#include <algorithm>

#include "anh/logger.h"
#include "anh/crc.h"
#include "swganh/messages/deltas_message.h"
#include "swganh/messages/baselines_message.h"
#include "swganh/messages/scene_end_baselines.h"
#include "swganh/object/player/player_events.h"
#include "swganh/object/object_events.h"
#include "swganh/object/waypoint/waypoint.h"

using namespace anh;
using namespace std;
using namespace swganh::object;
using namespace player;
using namespace waypoint;
using namespace swganh::messages;

void PlayerMessageBuilder::RegisterEventHandlers()
{
    event_dispatcher->Subscribe("Player::Baselines", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto controller_event = static_pointer_cast<ControllerEvent>(incoming_event);
        SendBaselines(static_pointer_cast<Player>(controller_event->object), controller_event->controller);
    });
    event_dispatcher->Subscribe("Player::StatusBitmask", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildStatusBitmaskDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::ProfileFlag", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildProfileBitmaskDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::ProfessionTag", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildProfessionTagDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::BornDate", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildBornDateDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::TotalPlayTime", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildPlayTimeDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::AdminTag", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildAdminTagDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::Experience", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildXpDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::Waypoint", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildWaypointDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::ForcePower", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildCurrentForcePowerDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::MaxForcePower", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildMaxForcePowerDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::ForceSensitiveQuests", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildForceSensitiveQuestDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::CompletedForceSensitiveQuests", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildCompletedForceSensitiveQuestDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::QuestJournal", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildQuestJournalDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::ExperimentationFlag", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildExperimentationFlagDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::CraftingStage", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildCraftingStageDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::NearestCraftingStation", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildNearestCraftingStationDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::DraftSchematic", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildDraftSchematicDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::ExperimentationPoints", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildExperimentationPointsDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::AccomplishmentCounter", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildAccomplishmentCounterDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::RemoveFriend", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto name_event = static_pointer_cast<NameEvent>(incoming_event);
        BuildFriendsDelta(name_event->player);
    });
    event_dispatcher->Subscribe("Player::RemoveIgnoredPlayer", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto name_event = static_pointer_cast<NameEvent>(incoming_event);
        BuildIgnoredDelta(name_event->player);
    });
    event_dispatcher->Subscribe("Player::Friend", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildFriendsDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::IgnorePlayer", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildIgnoredDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::Language", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildLanguageDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::CurrentStomach", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildCurrentStomachDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::MaxStomach", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildMaxStomachDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::CurrentDrink", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildCurrentDrinkDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::MaxDrink", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildMaxDrinkDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Player::JediState", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<PlayerEvent>(incoming_event);
        BuildJediStateDelta(value_event->Get());
    });
}

void PlayerMessageBuilder::SendBaselines(shared_ptr<Player> player, shared_ptr<ObjectController> controller)
{
    player->AddBaselineToCache(BuildBaseline3(player));
    player->AddBaselineToCache(BuildBaseline6(player));
    player->AddBaselineToCache(BuildBaseline8(player));
    player->AddBaselineToCache(BuildBaseline9(player));
    
   
    for (auto& baseline : player->GetBaselines())
    {
        controller->Notify(baseline);
    }
        
    SendEndBaselines(player, controller);
}
void PlayerMessageBuilder::SendEndBaselines(shared_ptr<Player> player, shared_ptr<ObjectController> controller)
{
    swganh::messages::SceneEndBaselines scene_end_baselines;
    scene_end_baselines.object_id = player->GetObjectId();
    controller->Notify(scene_end_baselines);
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