// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_MISSION_MISSION_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_MISSION_MISSION_MESSAGE_BUILDER_H_

#include "swganh/object/intangible/intangible_message_builder.h"

#include "mission.h"

namespace swganh {
namespace object {
namespace waypoint {
    class Waypoint;
}}} // swganh::object::waypoint

namespace swganh {
namespace object {
namespace mission {
    class MissionMessageBuilder : public swganh::object::intangible::IntangibleMessageBuilder
    {
    public:
        MissionMessageBuilder(anh::EventDispatcher* dispatcher) :
            IntangibleMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void SendBaselines(const std::shared_ptr<Mission>& mission, const std::shared_ptr<anh::observer::ObserverInterface>& observer);
        virtual void RegisterEventHandlers();
		//deltas
		static void BuildDifficultyLevelDelta(const std::shared_ptr<Mission>& mission);
		static void BuildStartingLocationDelta(const std::shared_ptr<Mission>& mission);
		static void BuildCreatorNameDelta(const std::shared_ptr<Mission>& mission);
		static void BuildCreditRewardDelta(const std::shared_ptr<Mission>& mission);
		static void BuildDestinationLocationDelta(const std::shared_ptr<Mission>& mission);
		static void BuildTargetIffCrcDelta(const std::shared_ptr<Mission>& mission);
		static void BuildMissionDescriptionDelta(const std::shared_ptr<Mission>& mission);
		static void BuildMissionTitleDelta(const std::shared_ptr<Mission>& mission);
		static void BuildRepeatCounterDelta(const std::shared_ptr<Mission>& mission);
		static void BuildMissionTypeCrcDelta(const std::shared_ptr<Mission>& mission);
		static void BuildTargetNameDelta(const std::shared_ptr<Mission>& mission);
		static void BuildMissionWaypointDelta(const std::shared_ptr<Mission>& mission);

        // baselines
        static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<Mission>& object);
        static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<Mission>& object);
	private:
		typedef anh::ValueEvent<std::shared_ptr<Mission>> MissionEvent;
    };

}}}  // swganh::object::player

#endif  // SWGANH_OBJECT_PLAYER_PLAYER_MESSAGE_BUILDER_H_
