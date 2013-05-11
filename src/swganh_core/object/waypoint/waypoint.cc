// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh_core/object/waypoint/waypoint.h"

#include "swganh/crc.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object;

Waypoint::Waypoint()
    : coordinates_(glm::vec3(0,0,0))
    , activated_flag_(0)
    , location_network_id_(0)
    , planet_name_("")
    , name_(L"")
    , color_("")
{
}
Waypoint::Waypoint(glm::vec3 coordinates, bool activated,const string& planet, const wstring& name, const string& color)
    : coordinates_(coordinates)
    , planet_name_(planet)
    , name_(name)
    , color_(color)
{
    activated ? activated_flag_ = ACTIVATED : activated_flag_ = DEACTIVATED;
}

glm::vec3 Waypoint::GetCoordinates()
{
	auto lock = AcquireLock();
	return coordinates_;
}
void Waypoint::SetCoordinates(const glm::vec3& coords)
{
    auto lock = AcquireLock();
	coordinates_ = move(coords);
	DISPATCH(Waypoint, Coordinates);
}
void Waypoint::Activate()
{
    activated_flag_ = ACTIVATED;
	DISPATCH(Waypoint, Activated);
}
void Waypoint::DeActivate()
{
    activated_flag_ = DEACTIVATED;
	DISPATCH(Waypoint, Activated);
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
	    auto lock = AcquireLock();
        planet_name_ = planet_name;
    }
	DISPATCH(Waypoint, Planet);
}

uint8_t Waypoint::GetColorByte()
{
	auto lock = AcquireLock();

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
	    auto lock = AcquireLock();
        color_ = color;
    }
	DISPATCH(Waypoint, Color);
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

void Waypoint::SetName(const std::wstring& name)
{
	{
		auto lock = AcquireLock();
		name_ = name;
	}
	DISPATCH(Waypoint, Name);
}

bool Waypoint::Active() const 
{ 
	auto lock = AcquireLock();
	return activated_flag_ == 1; 
}

uint8_t Waypoint::GetActiveFlag() 
{ 
	auto lock = AcquireLock();
	return activated_flag_; 
}

const std::string& Waypoint::GetPlanet() 
{ 
	auto lock = AcquireLock();
	return planet_name_; 
}

const std::wstring& Waypoint::GetName() 
{ 
	auto lock = AcquireLock();
	return name_; 
}

std::string Waypoint::GetNameStandard() 
{ 
	auto lock = AcquireLock();
	return std::string(std::begin(name_), std::end(name_)); 
}

const std::string& Waypoint::GetColor() 
{ 
	auto lock = AcquireLock();
	return color_; 
}