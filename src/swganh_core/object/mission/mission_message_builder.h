// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_MISSION_MISSION_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_MISSION_MISSION_MESSAGE_BUILDER_H_

#include "swganh_core/object/intangible/intangible_message_builder.h"

#include "mission.h"

namespace swganh {
namespace object {

    class Waypoint;
}} // swganh::object

namespace swganh {
namespace object {

    class MissionMessageBuilder : public swganh::object::IntangibleMessageBuilder
    {
    public:
        MissionMessageBuilder(swganh::EventDispatcher* dispatcher) :
            IntangibleMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }

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
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline3(const std::shared_ptr<Mission>& object, boost::unique_lock<boost::mutex>& lock);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(const std::shared_ptr<Mission>& object, boost::unique_lock<boost::mutex>& lock);

	private:
		typedef swganh::ValueEvent<std::shared_ptr<Mission>> MissionEvent;
        
        void RegisterEventHandlers();
    };

}}  // swganh::object

#endif  // SWGANH_OBJECT_PLAYER_PLAYER_MESSAGE_BUILDER_H_
