
#include "swganh/object/waypoint/waypoint.h"

#include "swganh/messages/deltas_message.h"

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
    uses_ = uses;
    
    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(4);
        message.data.write(0);
    
        AddDeltasUpdate(message);    
    }
}
void Waypoint::SetCoordinates(float x, float y, float z)
{
    SetCoordinates(glm::vec3(x,y,z));
}
void Waypoint::SetCoordinates(const glm::vec3& coords)
{
    coordinates_ = coords;

    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(5);
        message.data.write(coords.x);
        message.data.write(coords.y);
        message.data.write(coords.z);
    
        AddDeltasUpdate(message);    
    }
}

void Waypoint::Activate()
{
    activated_flag_ = ACTIVATED;
    
    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(6);
        message.data.write<uint8_t>(activated_flag_);
    
        AddDeltasUpdate(message);    
    }
}
void Waypoint::DeActivate()
{
    activated_flag_ = DEACTIVATED;
    
    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(7);
        message.data.write<uint8_t>(activated_flag_);
    
        AddDeltasUpdate(message);    
    }
}

void Waypoint::SetPlanet(const string& planet_name)
{
    planet_name_ = planet_name;
    
    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(8);
        message.data.write(planet_name_);
    
        AddDeltasUpdate(message);    
    }
}

void Waypoint::SetName(const wstring& name)
{
    name_ = name;
    
    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(9);
        message.data.write(name_);
    
        AddDeltasUpdate(message);    
    }
}

uint8_t Waypoint::GetColorByte()
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

void Waypoint::SetColor(const string& color)
{
    color_ = color;
    
    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(0x0B);
        message.data.write(color_);
    
        AddDeltasUpdate(message);    
    }
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
    auto message = CreateBaselinesMessage(VIEW_3, 12);
    auto coords = coordinates_;
    message.data.write(coords.x);
    message.data.write(coords.z);
    message.data.write(coords.y);
    message.data.write<uint8_t>(activated_flag_);
    message.data.write<uint64_t>(location_network_id_);
    message.data.write(planet_name_);
    message.data.write(name_);
    message.data.write<uint8_t>(not_used_);
    message.data.write(color_);

    return boost::optional<BaselinesMessage>(move(message));
}