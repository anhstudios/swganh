
#ifndef SWGANH_OBJECT_WAYPOINT_WAYPOINT_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_WAYPOINT_WAYPOINT_MESSAGE_BUILDER_H_

#include <memory>
#include <boost/optional.hpp>

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {
namespace waypoint {

    class Waypoint;

    class WaypointMessageBuilder
    {
    public:
		static void BuildUsesDelta(Waypoint* object);
		static void BuildActivateDelta(Waypoint* object);
		static void BuildPlanetDelta(Waypoint* object);
		static void BuildCoordinatesDelta(Waypoint* object);
		static void BuildColor(Waypoint* object);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline3(Waypoint* object);
    };

}}}  // swganh::object::waypoint

#endif  // SWGANH_OBJECT_WAYPOINT_WAYPOINT_MESSAGE_BUILDER_H_
