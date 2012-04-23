// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/object/waypoint/waypoint_message_builder.h"

#include <cstdint>

#include "swganh/object/object.h"
#include "swganh/object/waypoint/waypoint.h"

using namespace anh;
using namespace std;
using namespace swganh::messages;
using namespace swganh::object::waypoint;

void WaypointMessageBuilder::RegisterEventHandlers()
{
    // TODO: Register Event Handlers Here For Waypoints
}

void WaypointMessageBuilder::BuildUsesDelta(const shared_ptr<Waypoint>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 4);
        message.data.write(object->GetUses());
    
        object->AddDeltasUpdate(message);    
    }
}
void WaypointMessageBuilder::BuildActivateDelta(const shared_ptr<Waypoint>& object)
{
	if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 6);
        message.data.write<uint8_t>(object->GetActiveFlag());
    
        object->AddDeltasUpdate(message);    
    }
}
void WaypointMessageBuilder::BuildPlanetDelta(const shared_ptr<Waypoint>& object)
{
	if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 8);
        message.data.write(object->GetPlanet());
    
        object->AddDeltasUpdate(message);    
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
    
        object->AddDeltasUpdate(message);    
    }
}
void WaypointMessageBuilder::BuildColor(const shared_ptr<Waypoint>& object)
{
	if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 11);
        message.data.write<uint8_t>(object->GetColorByte());
    
        object->AddDeltasUpdate(message);    
    }
}
BaselinesMessage WaypointMessageBuilder::BuildBaseline3(const shared_ptr<Waypoint>& object)
{
    auto message = CreateBaselinesMessage(object, Object::VIEW_3, 12);
    auto coords = object->GetCoordinates();
    message.data.write(coords.x);
    message.data.write(coords.z);
    message.data.write(coords.y);
    message.data.write<uint8_t>(object->GetActiveFlag());
    message.data.write<uint64_t>(object->GetLocationNetworkId());
    message.data.write(object->GetPlanet());
    message.data.write(object->GetName());
    message.data.write<uint8_t>(0);
    message.data.write(object->GetColor());
    return BaselinesMessage(std::move(message));
}
