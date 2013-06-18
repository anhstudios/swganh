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
    , color_(BLUE)
{
    SetDatabasePersisted(true);
}

Waypoint::Waypoint(glm::vec3 coordinates, bool activated,const string& planet, const wstring& name, WaypointColor color)
    : coordinates_(coordinates)
    , planet_name_(planet)
    , name_(name)
    , color_(color)
{
    activated ? activated_flag_ = ACTIVATED : activated_flag_ = DEACTIVATED;
}

glm::vec3 Waypoint::GetCoordinates() {
    auto lock = AcquireLock();
    return GetCoordinates(lock);
}

glm::vec3 Waypoint::GetCoordinates(boost::unique_lock<boost::mutex>& lock)
{
	return coordinates_;
}

void Waypoint::SetCoordinates(const glm::vec3& coords) {
    auto lock = AcquireLock();
    SetCoordinates(coords, lock);
}

void Waypoint::SetCoordinates(const glm::vec3& coords, boost::unique_lock<boost::mutex>& lock)
{
	coordinates_ = move(coords);
	DISPATCH(Waypoint, Coordinates);
}

void Waypoint::Activate() {
    auto lock = AcquireLock();
    Activate(lock);
}

void Waypoint::Activate(boost::unique_lock<boost::mutex>& lock)
{
    activated_flag_ = ACTIVATED;
	DISPATCH(Waypoint, Activated);
}

void Waypoint::DeActivate() {
    auto lock = AcquireLock();
    DeActivate(lock);
}

void Waypoint::DeActivate(boost::unique_lock<boost::mutex>& lock)
{
    activated_flag_ = DEACTIVATED;
	DISPATCH(Waypoint, Activated);
}

uint64_t Waypoint::GetLocationNetworkId() const {
    auto lock = AcquireLock();
    return GetLocationNetworkId(lock);
}

uint64_t Waypoint::GetLocationNetworkId(boost::unique_lock<boost::mutex>& lock) const
{
    return location_network_id_;
}

void Waypoint::SetLocationNetworkId(uint64_t location_network_id) {
    auto lock = AcquireLock();
    SetLocationNetworkId(location_network_id, lock);
}

void Waypoint::SetLocationNetworkId(uint64_t location_network_id, boost::unique_lock<boost::mutex>& lock)
{
    location_network_id_ = location_network_id;
}

void Waypoint::SetPlanet(const string& planet_name) {
    auto lock = AcquireLock();
    SetPlanet(planet_name, lock);
}

void Waypoint::SetPlanet(const string& planet_name, boost::unique_lock<boost::mutex>& lock)
{
	planet_name_ = planet_name;
	DISPATCH(Waypoint, Planet);
}

Waypoint::WaypointColor Waypoint::GetColor() {
    auto lock = AcquireLock();
    return GetColor(lock);
}

Waypoint::WaypointColor Waypoint::GetColor(boost::unique_lock<boost::mutex>& lock)
{
    return WaypointColor(static_cast<uint8_t>(color_));
}


std::string Waypoint::GetColorString() {
    auto lock = AcquireLock();
    return GetColorString(lock);
}

std::string Waypoint::GetColorString(boost::unique_lock<boost::mutex>& lock)
{
    std::string color;

    switch(char(color_))
    {
    case BLUE:
        color = "blue";
        break;
    case GREEN:
        color = "green";
        break;
    case ORANGE:
        color = "orange";
        break;
    case YELLOW:
        color = "yellow";
        break;
    case PURPLE:
        color = "purple";
        break;
    case WHITE:
        color = "white";
        break;
    case SPACE:
        color = "space";
        break;
    }

    return color;
}

void Waypoint::SetColor(WaypointColor color_byte) {
    auto lock = AcquireLock();
    SetColor(color_byte, lock);
}

void Waypoint::SetColor(WaypointColor color_byte, boost::unique_lock<boost::mutex>& lock)
{
    color_ = color_byte;
}

void Waypoint::SetName(const std::wstring& name) {
    auto lock = AcquireLock();
    SetName(name, lock);
}

void Waypoint::SetName(const std::wstring& name, boost::unique_lock<boost::mutex>& lock)
{
	name_ = name;
	DISPATCH(Waypoint, Name);
}

bool Waypoint::Active() const {
    auto lock = AcquireLock();
    return Active(lock);
}

bool Waypoint::Active(boost::unique_lock<boost::mutex>& lock) const
{ 
	return activated_flag_ == 1; 
}

uint8_t Waypoint::GetActiveFlag() {
    auto lock = AcquireLock();
    return GetActiveFlag(lock);
}

uint8_t Waypoint::GetActiveFlag(boost::unique_lock<boost::mutex>& lock) 
{ 
	return activated_flag_; 
}

const std::string& Waypoint::GetPlanet() {
    auto lock = AcquireLock();
    return GetPlanet(lock);
}

const std::string& Waypoint::GetPlanet(boost::unique_lock<boost::mutex>& lock) 
{ 
	return planet_name_; 
}

const std::wstring& Waypoint::GetName() {
    auto lock = AcquireLock();
    return GetName(lock);
}

const std::wstring& Waypoint::GetName(boost::unique_lock<boost::mutex>& lock) 
{ 
	return name_; 
}

std::string Waypoint::GetNameStandard() {
    auto lock = AcquireLock();
    return GetNameStandard(lock);
}

std::string Waypoint::GetNameStandard(boost::unique_lock<boost::mutex>& lock) 
{ 
	return std::string(std::begin(name_), std::end(name_)); 
}
