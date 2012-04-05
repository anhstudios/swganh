
#ifndef SWGANH_OBJECT_PLAYER_PLAYER_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_PLAYER_PLAYER_MESSAGE_BUILDER_H_

#include "swganh/object/object_message_builder.h"

#include "player.h"

namespace swganh {
namespace object {
namespace waypoint {
    class Waypoint;
}}} // swganh::object::waypoint

namespace swganh {
namespace object {
namespace player {
    class PlayerMessageBuilder : public ObjectMessageBuilder
    {
    public:
        PlayerMessageBuilder(anh::EventDispatcher* dispatcher) :
            ObjectMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void SendBaselines(std::shared_ptr<Player> player);
        virtual void RegisterEventHandlers();
        // deltas
        static void BuildStatusBitmaskDelta(std::shared_ptr<Player> object);
        static void BuildProfileBitmaskDelta(std::shared_ptr<Player> object);
        static void BuildProfessionTagDelta(std::shared_ptr<Player> object);
        static void BuildBornDateDelta(std::shared_ptr<Player> object);
        static void BuildPlayTimeDelta(std::shared_ptr<Player> object);
        static void BuildAdminTagDelta(std::shared_ptr<Player> object);
        static void BuildXpDelta(std::shared_ptr<Player> object);
        static void BuildWaypointDelta(std::shared_ptr<Player> object);
        static void BuildCurrentForcePowerDelta(std::shared_ptr<Player> object);
        static void BuildMaxForcePowerDelta(std::shared_ptr<Player> object);
        static void BuildForceSensitiveQuestDelta(std::shared_ptr<Player> object);
        static void BuildCompletedForceSensitiveQuestDelta(std::shared_ptr<Player> object);
        static void BuildQuestJournalDelta(std::shared_ptr<Player> object);
        static void BuildAbilityDelta(std::shared_ptr<Player> object);
        static void BuildExperimentationFlagDelta(std::shared_ptr<Player> object);
        static void BuildCraftingStageDelta(std::shared_ptr<Player> object);
        static void BuildNearestCraftingStationDelta(std::shared_ptr<Player> object);
        static void BuildDraftSchematicDelta(std::shared_ptr<Player> object);
        static void BuildExperimentationPointsDelta(std::shared_ptr<Player> object);
        static void BuildAccomplishmentCounterDelta(std::shared_ptr<Player> object);
        static void BuildFriendsDelta(std::shared_ptr<Player> object);
        static void BuildIgnoredDelta(std::shared_ptr<Player> object);
        static void BuildLanguageDelta(std::shared_ptr<Player> object);
        static void BuildCurrentStomachDelta(std::shared_ptr<Player> object);
        static void BuildMaxStomachDelta(std::shared_ptr<Player> object);
        static void BuildCurrentDrinkDelta(std::shared_ptr<Player> object);
        static void BuildMaxDrinkDelta(std::shared_ptr<Player> object);
        static void BuildJediStateDelta(std::shared_ptr<Player> object);
        // baselines
        static swganh::messages::BaselinesMessage BuildBaseline3(std::shared_ptr<Player> object);
        static swganh::messages::BaselinesMessage BuildBaseline6(std::shared_ptr<Player> object);
        static swganh::messages::BaselinesMessage BuildBaseline8(std::shared_ptr<Player> object);
        static swganh::messages::BaselinesMessage BuildBaseline9(std::shared_ptr<Player> object);

        typedef anh::ValueEvent<std::shared_ptr<Player>> PlayerEvent;
    };

}}}  // swganh::object::player

#endif  // SWGANH_OBJECT_PLAYER_PLAYER_MESSAGE_BUILDER_H_
