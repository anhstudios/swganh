#include "mission_message_builder.h"

#include <memory>

#include "swganh/logger.h"
#include "swganh/crc.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/baselines_message.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/object/object_events.h"
#include "swganh_core/object/waypoint/waypoint.h"

using namespace swganh;
using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

void MissionMessageBuilder::RegisterEventHandlers()
{
	event_dispatcher->Subscribe("Mission::Baselines", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto controller_event = static_pointer_cast<ObserverEvent>(incoming_event);
        SendBaselines(static_pointer_cast<Mission>(controller_event->object), controller_event->observer);
    });
	event_dispatcher->Subscribe("Mission::DifficultyLevel", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<MissionEvent>(incoming_event);
        BuildDifficultyLevelDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Mission::StartLocation", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<MissionEvent>(incoming_event);
		BuildStartingLocationDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Mission::CreatorName", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<MissionEvent>(incoming_event);
		BuildCreatorNameDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Mission::CreditReward", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<MissionEvent>(incoming_event);
		BuildCreditRewardDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Mission::DestinationLocation", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<MissionEvent>(incoming_event);
		BuildDestinationLocationDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Mission::TargetIff", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<MissionEvent>(incoming_event);
		BuildTargetIffCrcDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Mission::MissionDescription", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<MissionEvent>(incoming_event);
		BuildMissionDescriptionDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Mission::MissionTitle", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<MissionEvent>(incoming_event);
		BuildMissionTitleDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Mission::RepeatCounter", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<MissionEvent>(incoming_event);
        BuildDifficultyLevelDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Mission::Waypoint", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<MissionEvent>(incoming_event);
		BuildMissionWaypointDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Mission::MissionType", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<MissionEvent>(incoming_event);
		BuildMissionTitleDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Mission::TargetName", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<MissionEvent>(incoming_event);
		BuildTargetNameDelta(value_event->Get());
    });
}

void MissionMessageBuilder::BuildDifficultyLevelDelta(const std::shared_ptr<Mission>& mission)
{
	if (mission->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(mission, Object::VIEW_3, 5);
		message.data.write(mission->GetDifficultyLevel());
		mission->AddDeltasUpdate(&message);
	}
}

void MissionMessageBuilder::BuildStartingLocationDelta(const std::shared_ptr<Mission>& mission)
{
	if (mission->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(mission, Object::VIEW_3, 6);
		auto location = mission->GetStartingPosition();
		message.data.write(location.x);
		message.data.write(location.y);
		message.data.write(location.z);
		message.data.write<uint64_t>(0L);
		message.data.write(mission->GetStartingSceneCrc());
		mission->AddDeltasUpdate(&message);
	}
}

void MissionMessageBuilder::BuildCreatorNameDelta(const std::shared_ptr<Mission>& mission) 
{
	if (mission->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(mission, Object::VIEW_3, 7);
		message.data.write(mission->GetMissionCreator());
		mission->AddDeltasUpdate(&message);
	}
}

void MissionMessageBuilder::BuildCreditRewardDelta(const std::shared_ptr<Mission>& mission)
{
	if (mission->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(mission, Object::VIEW_3, 8);
		message.data.write(mission->GetMissionReward());
		mission->AddDeltasUpdate(&message);
	}
}
	
void MissionMessageBuilder::BuildDestinationLocationDelta(const std::shared_ptr<Mission>& mission)
{
	if (mission->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(mission, Object::VIEW_3, 9);
		auto location = mission->GetDestinationPosition();
		message.data.write(location.x);
		message.data.write(location.y);
		message.data.write(location.z);
		message.data.write<uint64_t>(0L);
		message.data.write(mission->GetDestinationSceneCrc());
		mission->AddDeltasUpdate(&message);
	}
}

void MissionMessageBuilder::BuildTargetIffCrcDelta(const std::shared_ptr<Mission>& mission)
{
	if (mission->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(mission, Object::VIEW_3, 10);
		message.data.write(mission->GetTargetObjectTemplateCrc());
		mission->AddDeltasUpdate(&message);
	}
}

void MissionMessageBuilder::BuildMissionDescriptionDelta(const std::shared_ptr<Mission>& mission)
{
	if (mission->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(mission, Object::VIEW_3, 11);
		message.data.write(mission->GetMissionDescriptionStfFile());
		message.data.write<uint32_t>(0);
		message.data.write(mission->GetMissionDescriptionStfName());
		mission->AddDeltasUpdate(&message);
	}
}

void MissionMessageBuilder::BuildMissionTitleDelta(const std::shared_ptr<Mission>& mission)
{
	if (mission->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(mission, Object::VIEW_3, 12);
		message.data.write(mission->GetMissionTitleStfFile());
		message.data.write<uint32_t>(0);
		message.data.write(mission->GetMissionTitleStfName());
		mission->AddDeltasUpdate(&message);
	}
}

void MissionMessageBuilder::BuildRepeatCounterDelta(const std::shared_ptr<Mission>& mission)
{
	if (mission->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(mission, Object::VIEW_3, 13);
		message.data.write(mission->GetRepeatCounter());
		mission->AddDeltasUpdate(&message);
	}
}

void MissionMessageBuilder::BuildMissionTypeCrcDelta(const std::shared_ptr<Mission>& mission)
{
	if (mission->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(mission, Object::VIEW_3, 14);
		message.data.write(mission->GetMissionTypeCrc());
		mission->AddDeltasUpdate(&message);
	}
}

void MissionMessageBuilder::BuildTargetNameDelta(const std::shared_ptr<Mission>& mission)
{
	if (mission->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(mission, Object::VIEW_3, 15);
		message.data.write(mission->GetTargetName());
		mission->AddDeltasUpdate(&message);
	}
}

void MissionMessageBuilder::BuildMissionWaypointDelta(const std::shared_ptr<Mission>& mission)
{
	if (mission->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(mission, Object::VIEW_3, 16);
		auto waypoint = mission->GetMissionWaypoint();
		auto location = waypoint->GetCoordinates();
		
		message.data.write<uint32_t>(0); //Unknown
		message.data.write(location.x);
		message.data.write(location.y);
		message.data.write(location.z);
		message.data.write(0);
		message.data.write(mission->GetStartingSceneCrc());
		message.data.write(waypoint->GetName());
		message.data.write(waypoint->GetObjectId());
		message.data.write(waypoint->GetColorByte());
		message.data.write(waypoint->GetActiveFlag());

		mission->AddDeltasUpdate(&message);
	}
}

void MissionMessageBuilder::SendBaselines(const std::shared_ptr<Mission>& mission, const std::shared_ptr<swganh::observer::ObserverInterface>& observer)
{
	mission->AddBaselineToCache(&BuildBaseline3(mission));
    mission->AddBaselineToCache(&BuildBaseline6(mission));
    
    for (auto& baseline : mission->GetBaselines())
    {
        observer->Notify(&baseline);
    }
        
    SendEndBaselines(mission, observer);
}

BaselinesMessage MissionMessageBuilder::BuildBaseline3(const std::shared_ptr<Mission>& mission)
{
	auto message = CreateBaselinesMessage(mission, Object::VIEW_3, 17);
	auto waypoint = mission->GetMissionWaypoint();
	auto start_location = mission->GetStartingPosition();
	auto end_location = mission->GetDestinationPosition();
	auto waypoint_location = waypoint->GetCoordinates();
	
	message.data.append(IntangibleMessageBuilder::BuildBaseline3(mission).data);
	message.data.write(mission->GetDifficultyLevel());
	message.data.write(start_location.x);
	message.data.write(start_location.y);
	message.data.write(start_location.z);
	message.data.write<uint64_t>(0L);
	message.data.write(mission->GetStartingSceneCrc());
	message.data.write(mission->GetMissionCreator());
	message.data.write(mission->GetMissionReward());
	message.data.write(end_location.x);
	message.data.write(end_location.y);
	message.data.write(end_location.z);
	message.data.write<uint64_t>(0L);
	message.data.write(mission->GetDestinationSceneCrc());
	message.data.write(mission->GetTargetObjectTemplateCrc());
	message.data.write(mission->GetMissionDescriptionStfFile());
	message.data.write<uint32_t>(0);
	message.data.write(mission->GetMissionDescriptionStfName());
	message.data.write(mission->GetMissionTitleStfFile());
	message.data.write<uint32_t>(0);
	message.data.write(mission->GetMissionTitleStfName());
	message.data.write(mission->GetRepeatCounter());
	message.data.write(mission->GetMissionTypeCrc());
	message.data.write(mission->GetTargetName());
	message.data.write<uint32_t>(0); //Unknown
	message.data.write(waypoint_location.x);
	message.data.write(waypoint_location.y);
	message.data.write(waypoint_location.z);
	message.data.write(0);
	message.data.write(mission->GetStartingSceneCrc());
	message.data.write(waypoint->GetName());
	message.data.write(waypoint->GetObjectId());
	message.data.write(waypoint->GetColorByte());
	message.data.write(waypoint->GetActiveFlag());
	return BaselinesMessage(std::move(message));
}

BaselinesMessage MissionMessageBuilder::BuildBaseline6(const std::shared_ptr<Mission>& mission)
{
	auto message = CreateBaselinesMessage(mission, Object::VIEW_6, 1);
    message.data.append(IntangibleMessageBuilder::BuildBaseline6(mission).data);
    return BaselinesMessage(move(message));
}