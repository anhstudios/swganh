// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_WAYPOINT_WAYPOINT_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_WAYPOINT_WAYPOINT_MESSAGE_BUILDER_H_

#include "swganh/object/object_message_builder.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {
namespace waypoint {

    class Waypoint;

    class WaypointMessageBuilder : public ObjectMessageBuilder
    {
    public:
        WaypointMessageBuilder(anh::EventDispatcher* dispatcher) : 
            ObjectMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void RegisterEventHandlers();
		static void BuildUsesDelta(const std::shared_ptr<Waypoint>& object);
		static void BuildActivateDelta(const std::shared_ptr<Waypoint>& object);
		static void BuildPlanetDelta(const std::shared_ptr<Waypoint>& object);
		static void BuildCoordinatesDelta(const std::shared_ptr<Waypoint>& object);
		static void BuildColor(const std::shared_ptr<Waypoint>& object);
        static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<Waypoint>& object);
    };

}}}  // swganh::object::waypoint

#endif  // SWGANH_OBJECT_WAYPOINT_WAYPOINT_MESSAGE_BUILDER_H_
