// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "waypoint_message_builder.h"

#include <cstdint>

#include "swganh_core/object/waypoint/waypoint.h"
#include "swganh_core/object/object_events.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/baselines_message.h"

using namespace swganh;
using namespace std;
using namespace swganh::messages;
using namespace swganh::object;

void WaypointMessageBuilder::RegisterEventHandlers()
{
    event_dispatcher->Subscribe("Waypoint::Baselines", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto controller_event = static_pointer_cast<ObserverEvent>(incoming_event);
        SendBaselines(static_pointer_cast<Waypoint>(controller_event->object), controller_event->observer);
    });

    event_dispatcher->Subscribe("Waypoint::Activated", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<WaypointEvent>(incoming_event);
		BuildActivateDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Waypoint::Coordinates", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<WaypointEvent>(incoming_event);
		BuildCoordinatesDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Waypoint::Planet", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<WaypointEvent>(incoming_event);
		BuildPlanetDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Waypoint::Color", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<WaypointEvent>(incoming_event);
		BuildColorDelta(value_event->Get());
    });
}

void WaypointMessageBuilder::BuildActivateDelta(const shared_ptr<Waypoint>& object)
{
	if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 6);
        message.data.write<uint8_t>(object->GetActiveFlag());
    
        object->AddDeltasUpdate(&message);    
    }
}
void WaypointMessageBuilder::BuildPlanetDelta(const shared_ptr<Waypoint>& object)
{
	if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 8);
        message.data.write(object->GetPlanet());
    
        object->AddDeltasUpdate(&message);    
    }
}
void WaypointMessageBuilder::BuildCoordinatesDelta(const shared_ptr<Waypoint>& object)
{
	if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 5);
        auto coords = object->GetCoordinates();
        message.data.write(coords.x);
		message.data.write(coords.y);
		message.data.write(coords.z);
    
        object->AddDeltasUpdate(&message);    
    }
}
void WaypointMessageBuilder::BuildColorDelta(const shared_ptr<Waypoint>& object)
{
	if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 11);
        message.data.write<uint8_t>(object->GetColor());
    
        object->AddDeltasUpdate(&message);    
    }
}

boost::optional<BaselinesMessage> WaypointMessageBuilder::BuildBaseline3(const shared_ptr<Waypoint>& object, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(object, lock, Object::VIEW_3, 12);
	message.data.append((*IntangibleMessageBuilder::BuildBaseline3(object, lock)).data);
    auto coords = object->GetCoordinates(lock);
    message.data.write(coords.x);
    message.data.write(coords.z);
    message.data.write(coords.y);
    message.data.write<uint8_t>(object->GetActiveFlag(lock));
    message.data.write<uint64_t>(object->GetLocationNetworkId(lock));
    message.data.write(object->GetPlanet(lock));
    message.data.write(object->GetName(lock));
    message.data.write<uint8_t>(0);
    message.data.write(object->GetColor(lock));
    return BaselinesMessage(std::move(message));
}

boost::optional<BaselinesMessage> WaypointMessageBuilder::BuildBaseline6(const std::shared_ptr<Waypoint>& object, boost::unique_lock<boost::mutex>& lock)
{
	auto message = CreateBaselinesMessage(object, lock, Object::VIEW_6, 5);
	message.data.append((*IntangibleMessageBuilder::BuildBaseline6(object, lock)).data);
	return message;
}
