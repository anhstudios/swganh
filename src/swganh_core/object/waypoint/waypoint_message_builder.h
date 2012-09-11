// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/intangible/intangible_message_builder.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {


    class Waypoint;

    class WaypointMessageBuilder : public swganh::object::IntangibleMessageBuilder
    {
    public:
        WaypointMessageBuilder(swganh::EventDispatcher* dispatcher) : 
            IntangibleMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void RegisterEventHandlers();
		static void BuildActivateDelta(const std::shared_ptr<Waypoint>& object);
		static void BuildPlanetDelta(const std::shared_ptr<Waypoint>& object);
		static void BuildCoordinatesDelta(const std::shared_ptr<Waypoint>& object);
		static void BuildColorDelta(const std::shared_ptr<Waypoint>& object);

		virtual void SendBaselines(const std::shared_ptr<Waypoint>& waypoint, const std::shared_ptr<swganh::observer::ObserverInterface>& observer);

        static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<Waypoint>& object);
		static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<Waypoint>& object);
	private:
		typedef swganh::ValueEvent<std::shared_ptr<Waypoint>> WaypointEvent;
    };

}}  // swganh::object
