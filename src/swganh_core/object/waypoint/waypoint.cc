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

glm::vec3 Waypoint::GetCoordinates() { return GetCoordinates(AcquireLock()); }
glm::vec3 Waypoint::GetCoordinates(boost::unique_lock<boost::mutex>& lock)
{
	return coordinates_;
}

void Waypoint::SetCoordinates(const glm::vec3& coords) { SetCoordinates(coords, AcquireLock()); }
void Waypoint::SetCoordinates(const glm::vec3& coords, boost::unique_lock<boost::mutex>& lock)
{
	coordinates_ = move(coords);
	DISPATCH(Waypoint, Coordinates);
}

void Waypoint::Activate() { Activate(AcquireLock()); }
void Waypoint::Activate(boost::unique_lock<boost::mutex>& lock)
{
    activated_flag_ = ACTIVATED;
	DISPATCH(Waypoint, Activated);
}

void Waypoint::DeActivate() { Deactivate(AcquireLock()); }
void Waypoint::DeActivate(boost::unique_lock<boost::mutex>& lock)
{
    activated_flag_ = DEACTIVATED;
	DISPATCH(Waypoint, Activated);
}

uint64_t Waypoint::GetLocationNetworkId() const { return GetLocationNetworkId(AcquireLock()); }
uint64_t Waypoint::GetLocationNetworkId(boost::unique_lock<boost::mutex>& lock) const
{
    return location_network_id_;
}

void Waypoint::SetLocationNetworkId(uint64_t location_network_id) { SetLocationNetworkId(location_network_id, AcquireLock()); }
void Waypoint::SetLocationNetworkId(uint64_t location_network_id, boost::unique_lock<boost::mutex>& lock)
{
    location_network_id_ = location_network_id;
}

void Waypoint::SetPlanet(const string& planet_name) { SetPlanet(planet_name, AcquireLock()); }
void Waypoint::SetPlanet(const string& planet_name, boost::unique_lock<boost::mutex>& lock)
{
	planet_name_ = planet_name;
	DISPATCH(Waypoint, Planet);
}

uint8_t Waypoint::GetColorByte() { return GetColorByte(AcquireLock()); }
uint8_t Waypoint::GetColorByte(boost::unique_lock<boost::mutex>& lock)
{

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

void Waypoint::SetColor(const string& color) { SetColor(color, AcquireLock()); }
void Waypoint::SetColor(const string& color, boost::unique_lock<boost::mutex>& lock)
{
	color_ = color;
	DISPATCH(Waypoint, Color);
}

void Waypoint::SetColorByte(uint8_t color_byte) { SetColorByte(color_byte, AcquireLock()); }
void Waypoint::SetColorByte(uint8_t color_byte, boost::unique_lock<boost::mutex>& lock)
{
	switch (color_byte)
    {
    case 1:
        SetColor("blue", lock);
        break;
    case 2:
        SetColor("green", lock);
        break;
    case 3:
        SetColor("orange", lock);
        break;
    case 4:
        SetColor("yellow", lock);
        break;
    case 5:
        SetColor("white", lock);
        break;
    case 6:
        SetColor("space", lock);
        break;
    default:
        SetColor("blue", lock);
        break;
    }
}

void Waypoint::SetName(const std::wstring& name) { SetName(name, AcquireLock()); }
void Waypoint::SetName(const std::wstring& name, boost::unique_lock<boost::mutex>& lock)
{
	name_ = name;
	DISPATCH(Waypoint, Name);
}

bool Waypoint::Active() const { return Active(AcquireLock()); }
bool Waypoint::Active(boost::unique_lock<boost::mutex>& lock) const
{ 
	return activated_flag_ == 1; 
}

uint8_t Waypoint::GetActiveFlag() { return GetActiveFlag(AcquireLock()); }
uint8_t Waypoint::GetActiveFlag(boost::unique_lock<boost::mutex>& lock) 
{ 
	return activated_flag_; 
}

const std::string& Waypoint::GetPlanet() { return GetPlanet(AcquireLock()); }
const std::string& Waypoint::GetPlanet(boost::unique_lock<boost::mutex>& lock) 
{ 
	return planet_name_; 
}

const std::wstring& Waypoint::GetName() { return GetName(AcquireLock()); }
const std::wstring& Waypoint::GetName(boost::unique_lock<boost::mutex>& lock) 
{ 
	return name_; 
}

std::string Waypoint::GetNameStandard() { return GetNameStandard(AcquireLock()); }
std::string Waypoint::GetNameStandard(boost::unique_lock<boost::mutex>& lock) 
{ 
	return std::string(std::begin(name_), std::end(name_)); 
}

const std::string& Waypoint::GetColor() { return GetColor(AcquireLock()); }
const std::string& Waypoint::GetColor(boost::unique_lock<boost::mutex>& lock) 
{ 
	return color_; 
}