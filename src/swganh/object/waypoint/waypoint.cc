
#include "swganh/object/waypoint/waypoint.h"

#include "swganh/object/waypoint/waypoint_message_builder.h"
#include "anh/crc.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object::waypoint;

Waypoint::Waypoint()
    : uses_(0)
    , coordinates_(glm::vec3(0,0,0))
    , activated_flag_(0)
    , location_network_id_(0)
    , planet_name_("")
    , name_(L"")
    , color_("")
{
}
Waypoint::Waypoint(glm::vec3 coordinates, bool activated,const string& planet, const wstring& name, const string& color)
    : uses_(0)
    , coordinates_(coordinates)
    , planet_name_(planet)
    , name_(name)
    , color_(color)
{
    activated ? activated_flag_ = ACTIVATED : activated_flag_ = DEACTIVATED;
}

uint32_t Waypoint::GetUses()
{
    return uses_;
}

void Waypoint::SetUses(uint32_t uses) 
{
    uses_ = uses;
    WaypointMessageBuilder::BuildUsesDelta(this);
}
glm::vec3 Waypoint::GetCoordinates()
{
	std::lock_guard<std::mutex> lock(waypoint_mutex_);
	return coordinates_;
}
void Waypoint::SetCoordinates(const glm::vec3& coords)
{
    {
	    std::lock_guard<std::mutex> lock(waypoint_mutex_);
	    coordinates_ = move(coords);
    }

	WaypointMessageBuilder::BuildCoordinatesDelta(this);
}
void Waypoint::Activate()
{
    activated_flag_ = ACTIVATED;
    WaypointMessageBuilder::BuildActivateDelta(this);
}
void Waypoint::DeActivate()
{
    activated_flag_ = DEACTIVATED;
    WaypointMessageBuilder::BuildActivateDelta(this);
}

uint64_t Waypoint::GetLocationNetworkId() const
{
    return location_network_id_;
}

void Waypoint::SetLocationNetworkId(uint64_t location_network_id)
{
    location_network_id_ = location_network_id;
}

void Waypoint::SetPlanet(const string& planet_name)
{
    {
	    std::lock_guard<std::mutex> lock(waypoint_mutex_);
        planet_name_ = planet_name;
    }

    WaypointMessageBuilder::BuildPlanetDelta(this);
}

uint8_t Waypoint::GetColorByte()
{
	std::lock_guard<std::mutex> lock(waypoint_mutex_);

    if (color_.compare("blue") != 0)
        return 1;
    else if (color_.compare("green") != 0)
        return 2;
    else if (color_.compare("orange") != 0)
        return 3;
    else if (color_.compare("yellow") != 0)
        return 4;
    else if (color_.compare("purple") != 0)
        return 5;
    else if (color_.compare("white") != 0)
        return 6;
    else if (color_.compare("space") != 0)
        return 7;
    // default
    else
        return 1;
}

void Waypoint::SetColor(const string& color)
{
    {
	    std::lock_guard<std::mutex> lock(waypoint_mutex_);
        color_ = color;
    }

	WaypointMessageBuilder::BuildColor(this);
}

void Waypoint::SetColorByte(uint8_t color_byte)
{
	switch (color_byte)
    {
    case 1:
        SetColor("blue");
        break;
    case 2:
        SetColor("green");
        break;
    case 3:
        SetColor("orange");
        break;
    case 4:
        SetColor("yellow");
        break;
    case 5:
        SetColor("white");
        break;
    case 6:
        SetColor("space");
        break;
    default:
        SetColor("blue");
        break;
    }
}
boost::optional<BaselinesMessage> Waypoint::GetBaseline3()
{
    return WaypointMessageBuilder::BuildBaseline3(this);
}