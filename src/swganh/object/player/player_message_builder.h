
#ifndef SWGANH_OBJECT_PLAYER_PLAYER_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_PLAYER_PLAYER_MESSAGE_BUILDER_H_

#include <cstdint>
#include <memory>
#include <boost/optional.hpp>

#include "player.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {
namespace waypoint {
    class Waypoint;
}}} // swganh::object::waypoint

namespace swganh {
namespace object {
namespace player {
    class PlayerMessageBuilder
    {
    public:
        // deltas
        static void BuildStatusBitmaskDelta(Player* object);
        static void BuildProfileBitmaskDelta(Player* object);
        static void BuildProfessionTagDelta(Player* object);
        static void BuildBornDateDelta(Player* object);
        static void BuildPlayTimeDelta(Player* object);
        static void BuildRegionIdDelta(Player* object);
        static void BuildAdminTagDelta(Player* object);
        static void BuildXpDelta(Player* object);
        static void BuildWaypointDelta(Player* object);
        static void BuildCurrentForcePowerDelta(Player* object);
        static void BuildMaxForcePowerDelta(Player* object);
        static void BuildForceSensitiveQuestDelta(Player* object);
        static void BuildCompletedForceSensitiveQuestDelta(Player* object);
        static void BuildQuestJournalDelta(Player* object);
        static void BuildAbilityDelta(Player* object);
        static void BuildExperimentationFlagDelta(Player* object);
        static void BuildCraftingStageDelta(Player* object);
        static void BuildNearestCraftingStationDelta(Player* object);
        static void BuildDraftSchematicDelta(Player* object);
        static void BuildExperimentationPointsDelta(Player* object);
        static void BuildAccomplishmentCounterDelta(Player* object);
        static void BuildFriendsDelta(Player* object);
        static void BuildIgnoredDelta(Player* object);
        static void BuildLanguageDelta(Player* object);
        static void BuildCurrentStomachDelta(Player* object);
        static void BuildMaxStomachDelta(Player* object);
        static void BuildCurrentDrinkDelta(Player* object);
        static void BuildMaxDrinkDelta(Player* object);
        static void BuildJediStateDelta(Player* object);
        // baselines
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline3(Player* object);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(Player* object);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline8(Player* object);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline9(Player* object);
    };

}}}  // swganh::object::player

#endif  // SWGANH_OBJECT_PLAYER_PLAYER_MESSAGE_BUILDER_H_
