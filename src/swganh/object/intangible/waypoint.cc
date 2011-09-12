
#include "waypoint.h"
#include "swganh/scene/messages/deltas_message.h"

using namespace swganh::object::intangible;
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
    activated ? activated_flag_ = 1 : activated_flag_ = 0;
}

void Waypoint::SetUses(uint32_t uses) 
{
    uses_ = uses;
    
    if (scene->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(4);
        message.data.write(0);
        scene->UpdateObservers(GetObjectId(), message);
        delta_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}
void SetCoordinates(float x, float y, float z)
{
    SetCoordinates(glm::vec3(x,y,z));
}
void SetCoordinates(const glm::vec3& coords)
{
    coordinates_ = coords;
    if (scene->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(5);
        message.data.write(coords.x);
        message.data.write(coords.y);
        message.data.write(coords.z);
        scene->UpdateObservers(GetObjectId(), message);
        delta_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}

void Activate()
{
    activated_flag_ = ACTIVATED;

    if (scene->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(6);
        message.data.write<uint8_t>(activated_flag_);
        scene->UpdateObservers(GetObjectId(), message);
        delta_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}
void DeActivate()
{
    activated_flag_ = DEACTIVATED;

    if (scene->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(6);
        message.data.write<uint8_t>(activated_flag_);
        scene->UpdateObservers(GetObjectId(), message);
        delta_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}

void SetPlanet(const std::string& planet_name)
{
    planet_name_ = planet_name;

    if (scene->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(8);
        message.data.write(planet_name_);
        scene->UpdateObservers(GetObjectId(), message);
        delta_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}

void SetName(const std::wstring& name)
{
    name_ = name;

    if (scene->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(9);
        message.data.write(name_);
        scene->UpdateObservers(GetObjectId(), message);
        delta_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}

void SetColor(const std::string& color)
{
    color_ = color;

    if (scene->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(0x0B);
        message.data.write(color_);
        scene->UpdateObservers(GetObjectId(), message);
        delta_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}