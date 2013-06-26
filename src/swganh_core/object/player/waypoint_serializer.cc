#include "waypoint_serializer.h"

#include "swganh_core/object/waypoint/waypoint.h"

#include "swganh/crc.h"

using namespace swganh::object;

void PlayerWaypointSerializer::SerializeBaseline(swganh::ByteBuffer& data, const PlayerWaypointSerializer& t)
{
    data.write<uint8_t>(0);
    data.write<uint64_t>(t.waypoint->GetObjectId());
    data.write<uint32_t>(0);
    auto coordinates_ = t.waypoint->GetCoordinates();
    data.write<float>(coordinates_.x);
    data.write<float>(coordinates_.y);
    data.write<float>(coordinates_.z);
    data.write<uint64_t>(0);
    data.write<uint32_t>(swganh::memcrc(t.waypoint->GetPlanet()));
    data.write<std::wstring>(t.waypoint->GetName());
    data.write<uint64_t>(t.waypoint->GetObjectId());
    data.write<uint8_t>(t.waypoint->GetColor());
    data.write<uint8_t>(t.waypoint->Active() ? 1 : 0);
}

void PlayerWaypointSerializer::SerializeDelta(swganh::ByteBuffer& data, const PlayerWaypointSerializer& t)
{
    data.write<uint64_t>(t.waypoint->GetObjectId());
    data.write<uint32_t>(0);
    auto coordinates_ = t.waypoint->GetCoordinates();
    data.write<float>(coordinates_.x);
    data.write<float>(coordinates_.y);
    data.write<float>(coordinates_.z);
    data.write<uint64_t>(0);
	data.write<uint32_t>(swganh::memcrc(t.waypoint->GetPlanet()));
    data.write<std::wstring>(t.waypoint->GetName());
    data.write<uint64_t>(t.waypoint->GetObjectId());
    data.write<uint8_t>(t.waypoint->GetColor());
    data.write<uint8_t>(t.waypoint->Active() ? 1 : 0);
}

bool PlayerWaypointSerializer::operator==(const PlayerWaypointSerializer& other)
{
    return waypoint->GetObjectId() == other.waypoint->GetObjectId();
}