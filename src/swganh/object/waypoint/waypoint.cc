
#include "swganh/object/waypoint/waypoint.h"

#include "swganh/object/waypoint/waypoint_message_builder.h"

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

void Waypoint::SetUses(uint32_t uses) 
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    uses_ = uses;
    WaypointMessageBuilder::BuildUsesDelta(this);
}
glm::vec3 Waypoint::GetCoordinates()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	return coordinates_;
}
void Waypoint::SetCoordinates(const glm::vec3& coords)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	coordinates_ = move(coords);
	WaypointMessageBuilder::BuildCoordinatesDelta(this);
}
void Waypoint::Activate()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    activated_flag_ = ACTIVATED;
    WaypointMessageBuilder::BuildActivateDelta(this);
}
void Waypoint::DeActivate()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    activated_flag_ = DEACTIVATED;
    WaypointMessageBuilder::BuildActivateDelta(this);
}

void Waypoint::SetPlanet(const string& planet_name)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    planet_name_ = planet_name;
    WaypointMessageBuilder::BuildPlanetDelta(this);
}

uint8_t Waypoint::GetColorByte()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);

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
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    color_ = color;
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