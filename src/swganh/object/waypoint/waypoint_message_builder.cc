
#include "swganh/object/waypoint/waypoint_message_builder.h"

#include <cstdint>

#include "swganh/object/object.h"
#include "swganh/object/waypoint/waypoint.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object::waypoint;

void WaypointMessageBuilder::BuildUsesDelta(Waypoint* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_3, 4);
        message.data.write(0);
    
        object->AddDeltasUpdate(message);    
    }
}
void WaypointMessageBuilder::BuildActivateDelta(Waypoint* object)
{
	if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_3, 6);
        message.data.write<uint8_t>(object->activated_flag_);
    
        object->AddDeltasUpdate(message);    
    }
}
void WaypointMessageBuilder::BuildPlanetDelta(Waypoint* object)
{
	if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_3, 8);
        message.data.write(object->planet_name_);
    
        object->AddDeltasUpdate(message);    
    }
}
void WaypointMessageBuilder::BuildCoordinatesDelta(Waypoint* object)
{
	if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_3, 5);
        message.data.write(object->coordinates_.x);
		message.data.write(object->coordinates_.y);
		message.data.write(object->coordinates_.z);
    
        object->AddDeltasUpdate(message);    
    }
}
void WaypointMessageBuilder::BuildColor(Waypoint* object)
{
	if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_3, 11);
        message.data.write<uint8_t>(object->GetColorByte());
    
        object->AddDeltasUpdate(message);    
    }
}
boost::optional<BaselinesMessage> WaypointMessageBuilder::BuildBaseline3(Waypoint* object)
{
    auto message = object->CreateBaselinesMessage(Object::VIEW_3, 12);
    auto coords = object->coordinates_;
    message.data.write(coords.x);
    message.data.write(coords.z);
    message.data.write(coords.y);
    message.data.write<uint8_t>(object->activated_flag_);
    message.data.write<uint64_t>(object->location_network_id_);
    message.data.write(object->planet_name_);
    message.data.write(object->name_);
    message.data.write<uint8_t>(0);
    message.data.write(object->color_);
    return boost::optional<BaselinesMessage>(std::move(message));
}
