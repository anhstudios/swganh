// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "player_message_builder.h"

#include <algorithm>

#include "swganh/logger.h"
#include "swganh/crc.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/baselines_message.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/object/player/player_events.h"
#include "swganh_core/object/object_events.h"
#include "swganh_core/object/waypoint/waypoint.h"

using namespace swganh;
using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

void PlayerMessageBuilder::RegisterEventHandlers()
{
    
    event_dispatcher->Subscribe("Player::Baselines", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto controller_event = static_pointer_cast<ObserverEvent>(incoming_event);
        SendBaselines(static_pointer_cast<Player>(controller_event->object), controller_event->observer);
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

void PlayerMessageBuilder::BuildStatusBitmaskDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 5);

        auto status_flags = object->GetStatusFlags();

        message.data.write<uint32_t>(status_flags.size());

        for_each(begin(status_flags), end(status_flags), [&message] (uint32_t& flag) {
            message.data.write<uint32_t>(flag);
        });
        
        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildProfileBitmaskDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 6);
        
        auto profile_flags = object->GetProfileFlags();

        message.data.write<uint32_t>(profile_flags.size());

        for_each(begin(profile_flags),end(profile_flags), [&message] (uint32_t& flag) {
            message.data.write<uint32_t>(flag);
        });

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildProfessionTagDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 7);
        message.data.write(object->GetProfessionTag());

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildBornDateDelta(const shared_ptr<Player>& object)
{
     if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 8);
        message.data.write(object->GetBornDate());

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildPlayTimeDelta(const shared_ptr<Player>& object)
{
     if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 9);
        message.data.write(object->GetTotalPlayTime());

        object->AddDeltasUpdate(&message);
    }
}

void PlayerMessageBuilder::BuildAdminTagDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_6, 1);
        message.data.write<uint8_t>(object->GetAdminTag());
        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildXpDelta(const shared_ptr<Player>& object)
{
    DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_8, 0);
    object->SerializeXp(&message);
    object->AddDeltasUpdate(&message);
}
void PlayerMessageBuilder::BuildWaypointDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_8, 1);
        object->SerializeWaypoints(&message);
        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildCurrentForcePowerDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_8, 2);
        message.data.write(object->GetCurrentForcePower());

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildMaxForcePowerDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_8, 3);
        message.data.write(object->GetMaxForcePower());

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildForceSensitiveQuestDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_8, 4);
        message.data.write(object->GetCurrentForceSensitiveQuests());

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildCompletedForceSensitiveQuestDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_8, 5);
        message.data.write(object->GetCompletedForceSensitiveQuests());

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildQuestJournalDelta(const shared_ptr<Player>& object)
{
	if (object->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_8, 6);
		object->SerializeQuests(&message);
		object->AddDeltasUpdate(&message);
	}
}
void PlayerMessageBuilder::BuildAbilityDelta(const shared_ptr<Player>& object)
{
	if (object->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 0);
		//object->SerializeAbilities(&message);

		message.data.write<uint32_t>(0);
		message.data.write<uint32_t>(0);

		object->AddDeltasUpdate(&message);
	}
}

void PlayerMessageBuilder::BuildExperimentationFlagDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 1);
        message.data.write(object->GetExperimentationFlag());            

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildCraftingStageDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 2);
        message.data.write(object->GetCraftingStage());            

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildNearestCraftingStationDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 3);
        message.data.write(object->GetNearestCraftingStation());            

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildDraftSchematicDelta(const shared_ptr<Player>& object)
{
	if (object->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 4);
		object->SerializeDraftSchematics(&message);
		object->AddDeltasUpdate(&message);
	}
}

void PlayerMessageBuilder::BuildExperimentationPointsDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 5);
        message.data.write(object->GetExperimentationPoints());            

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildAccomplishmentCounterDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 6);
        message.data.write(object->GetAccomplishmentCounter());            

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildFriendsDelta(const shared_ptr<Player>& object)
{
	if (object->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 7);
		object->SerializeFriends(&message);
		object->AddDeltasUpdate(&message);
	}
}
void PlayerMessageBuilder::BuildIgnoredDelta(const shared_ptr<Player>& object)
{
	if (object->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 8);
		object->SerializeIgnoredPlayers(&message);
		object->AddDeltasUpdate(&message);
	}
}
void PlayerMessageBuilder::BuildLanguageDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 9);
        message.data.write(object->GetLanguage());            

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildCurrentStomachDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 10);
        message.data.write(object->GetCurrentStomach());            

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildMaxStomachDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 11);
        message.data.write(object->GetMaxStomach());            

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildCurrentDrinkDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 12);
        message.data.write(object->GetCurrentDrink());            

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildMaxDrinkDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 13);
        message.data.write(object->GetMaxDrink());            

        object->AddDeltasUpdate(&message);
    }
}
void PlayerMessageBuilder::BuildJediStateDelta(const shared_ptr<Player>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_9, 17);
        message.data.write(object->GetExperimentationPoints());            

        object->AddDeltasUpdate(&message);
    }
}

// baselines
boost::optional<BaselinesMessage> PlayerMessageBuilder::BuildBaseline3(const shared_ptr<Player>& object, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(object, lock, Object::VIEW_3, 10);
    message.data.append((*IntangibleMessageBuilder::BuildBaseline3(object, lock)).data);
    
    auto status_flags = object->GetStatusFlags(lock);

    message.data.write<uint32_t>(status_flags.size());

    for_each(begin(status_flags), end(status_flags), [&message] (uint32_t& flag) {
        message.data.write<uint32_t>(flag);
    });

    auto profile_flags = object->GetProfileFlags(lock);

    message.data.write<uint32_t>(profile_flags.size());

    for_each(begin(profile_flags), end(profile_flags), [&message] (uint32_t& flag) {
        message.data.write<uint32_t>(flag);
    });

    message.data.write<std::string>(object->GetProfessionTag(lock));        // Profession Tag
    message.data.write<uint32_t>(object->GetBornDate(lock));                // Born Date
    message.data.write<uint32_t>(object->GetTotalPlayTime(lock));           // Total Play Time
    return BaselinesMessage(move(message));
}

boost::optional<BaselinesMessage> PlayerMessageBuilder::BuildBaseline6(const shared_ptr<Player>& object, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(object, lock, Object::VIEW_6, 2);
    message.data.append((*IntangibleMessageBuilder::BuildBaseline6(object, lock)).data);
    message.data.write<uint8_t>(object->GetAdminTag(lock));     // Admin Tag
    return BaselinesMessage(move(message));
}

boost::optional<BaselinesMessage> PlayerMessageBuilder::BuildBaseline8(const shared_ptr<Player>& object, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(object, lock, Object::VIEW_8, 7);
    object->SerializeXp(&message, lock);
    object->SerializeWaypoints(&message, lock);
    
    message.data.write<uint32_t>(object->GetCurrentForcePower(lock));
    message.data.write<uint32_t>(object->GetMaxForcePower(lock));
    
    message.data.write<uint32_t>(0); // Force Sensetive Quest List Soze
    message.data.write<uint32_t>(0); // Force Sensetive Quest List Counter
    
    message.data.write<uint32_t>(0); // Completed Force Sensetive Quest List Size
    message.data.write<uint32_t>(0); // Completed Force Sensetive Quest List Counter
    
    object->SerializeQuests(&message, lock);
    
    return BaselinesMessage(move(message));
}

boost::optional<BaselinesMessage> PlayerMessageBuilder::BuildBaseline9(const shared_ptr<Player>& object, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(object, lock, Object::VIEW_9, 17);
    
    //object->SerializeAbilities(&message, lock);
    message.data.write<uint32_t>(0);
	message.data.write<uint32_t>(0);
	
	message.data.write<uint32_t>(object->GetExperimentationFlag(lock));
    message.data.write<uint32_t>(object->GetCraftingStage(lock));
    message.data.write<uint64_t>(object->GetNearestCraftingStation(lock));
    
    object->SerializeDraftSchematics(&message, lock);
    
    message.data.write<uint32_t>(object->GetExperimentationPoints(lock));
    message.data.write<uint32_t>(object->GetAccomplishmentCounter(lock));
    
    object->SerializeFriends(&message, lock);
    
    object->SerializeIgnoredPlayers(&message, lock);
    
    message.data.write<uint32_t>(object->GetLanguage(lock));
    message.data.write<uint32_t>(object->GetCurrentStomach(lock));
    message.data.write<uint32_t>(object->GetMaxStomach(lock));
    message.data.write<uint32_t>(object->GetCurrentDrink(lock));
    message.data.write<uint32_t>(object->GetMaxDrink(lock));
    message.data.write<uint32_t>(0); // Unused
    message.data.write<uint32_t>(0); // Unused
    message.data.write<uint32_t>(0); // Unused
    message.data.write<uint32_t>(0); // Unused
    message.data.write<uint32_t>(object->GetJediState(lock)); // Jedi State
    
    return BaselinesMessage(message);
}
