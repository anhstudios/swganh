// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/waypoint/waypoint.h"

namespace swganh
{
namespace object
{

struct PlayerWaypointSerializer {
    PlayerWaypointSerializer()
        : waypoint(nullptr){}
    PlayerWaypointSerializer(std::shared_ptr<swganh::object::Waypoint> waypoint_)
        : waypoint(waypoint_){}

    static void SerializeBaseline(swganh::ByteBuffer& data, const PlayerWaypointSerializer& t);
    static void SerializeDelta(swganh::ByteBuffer& data, const PlayerWaypointSerializer& t);

    bool operator==(const PlayerWaypointSerializer& other);

    std::shared_ptr<swganh::object::Waypoint> waypoint;
};

}
}