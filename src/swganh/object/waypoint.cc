
#include "waypoint.h"
#include "swganh/scene/messages/deltas_message.h"
#include "swganh/scene/scene.h"

using namespace swganh::object;
using namespace swganh::scene::messages;

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
Waypoint::Waypoint(glm::vec3 coordinates, bool activated,const std::string& planet, const std::wstring& name, const std::string& color)
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
    uses_ = uses;
    
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(4);
        message.data.write(0);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}
void Waypoint::SetCoordinates(float x, float y, float z)
{
    SetCoordinates(glm::vec3(x,y,z));
}
void Waypoint::SetCoordinates(const glm::vec3& coords)
{
    coordinates_ = coords;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(5);
        message.data.write(coords.x);
        message.data.write(coords.y);
        message.data.write(coords.z);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}

void Waypoint::Activate()
{
    activated_flag_ = ACTIVATED;

    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(6);
        message.data.write<uint8_t>(activated_flag_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}
void Waypoint::DeActivate()
{
    activated_flag_ = DEACTIVATED;

    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(6);
        message.data.write<uint8_t>(activated_flag_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}

void Waypoint::SetPlanet(const std::string& planet_name)
{
    planet_name_ = planet_name;

    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(8);
        message.data.write(planet_name_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}

void Waypoint::SetName(const std::wstring& name)
{
    name_ = name;

    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(9);
        message.data.write(name_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}

void Waypoint::SetColor(const std::string& color)
{
    color_ = color;

    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(0x0B);
        message.data.write(color_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}