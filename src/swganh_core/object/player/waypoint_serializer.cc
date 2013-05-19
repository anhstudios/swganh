#include "waypoint_serializer.h"

#include "swganh_core/object/waypoint/waypoint.h"

#include "swganh/crc.h"

using namespace swganh::object;

void PlayerWaypointSerializer::Serialize(swganh::messages::BaselinesMessage& message)
{
    message.data.write<uint64_t>(waypoint->GetObjectId());
    message.data.write<uint32_t>(0);
    auto coordinates_ = waypoint->GetCoordinates();
    message.data.write<float>(coordinates_.x);
    message.data.write<float>(coordinates_.y);
    message.data.write<float>(coordinates_.z);
    message.data.write<uint64_t>(0);
    message.data.write<uint32_t>(swganh::memcrc(waypoint->GetPlanet()));
    message.data.write<std::wstring>(waypoint->GetName());
    message.data.write<uint64_t>(waypoint->GetObjectId());
    message.data.write<uint8_t>(waypoint->GetColorByte());
    message.data.write<uint8_t>(waypoint->Active() ? 1 : 0);
}

void PlayerWaypointSerializer::Serialize(swganh::messages::DeltasMessage& message)
{
    message.data.write<uint32_t>(0);
    auto coordinates_ = waypoint->GetCoordinates();
    message.data.write<float>(coordinates_.x);
    message.data.write<float>(coordinates_.y);
    message.data.write<float>(coordinates_.z);
    message.data.write<uint64_t>(0);
    message.data.write<uint32_t>(swganh::memcrc(waypoint->GetPlanet()));
    message.data.write<std::wstring>(waypoint->GetName());
    message.data.write<uint64_t>(waypoint->GetObjectId());
    message.data.write<uint8_t>(waypoint->GetColorByte());
    message.data.write<uint8_t>(waypoint->Active() ? 1 : 0);
}

bool PlayerWaypointSerializer::operator==(const PlayerWaypointSerializer& other)
{
    return waypoint->GetObjectId() == other.waypoint->GetObjectId();
}