// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

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
        virtual void SendBaselines(const std::shared_ptr<Player>& player, const std::shared_ptr<anh::observer::ObserverInterface>& controller);
        virtual void RegisterEventHandlers();
        // deltas
        static void BuildStatusBitmaskDelta(const std::shared_ptr<Player>& object);
        static void BuildProfileBitmaskDelta(const std::shared_ptr<Player>& object);
        static void BuildProfessionTagDelta(const std::shared_ptr<Player>& object);
        static void BuildBornDateDelta(const std::shared_ptr<Player>& object);
        static void BuildPlayTimeDelta(const std::shared_ptr<Player>& object);
        static void BuildAdminTagDelta(const std::shared_ptr<Player>& object);
        static void BuildXpDelta(const std::shared_ptr<Player>& object);
        static void BuildWaypointDelta(const std::shared_ptr<Player>& object);
        static void BuildCurrentForcePowerDelta(const std::shared_ptr<Player>& object);
        static void BuildMaxForcePowerDelta(const std::shared_ptr<Player>& object);
        static void BuildForceSensitiveQuestDelta(const std::shared_ptr<Player>& object);
        static void BuildCompletedForceSensitiveQuestDelta(const std::shared_ptr<Player>& object);
        static void BuildQuestJournalDelta(const std::shared_ptr<Player>& object);
        static void BuildAbilityDelta(const std::shared_ptr<Player>& object);
        static void BuildExperimentationFlagDelta(const std::shared_ptr<Player>& object);
        static void BuildCraftingStageDelta(const std::shared_ptr<Player>& object);
        static void BuildNearestCraftingStationDelta(const std::shared_ptr<Player>& object);
        static void BuildDraftSchematicDelta(const std::shared_ptr<Player>& object);
        static void BuildExperimentationPointsDelta(const std::shared_ptr<Player>& object);
        static void BuildAccomplishmentCounterDelta(const std::shared_ptr<Player>& object);
        static void BuildFriendsDelta(const std::shared_ptr<Player>& object);
        static void BuildIgnoredDelta(const std::shared_ptr<Player>& object);
        static void BuildLanguageDelta(const std::shared_ptr<Player>& object);
        static void BuildCurrentStomachDelta(const std::shared_ptr<Player>& object);
        static void BuildMaxStomachDelta(const std::shared_ptr<Player>& object);
        static void BuildCurrentDrinkDelta(const std::shared_ptr<Player>& object);
        static void BuildMaxDrinkDelta(const std::shared_ptr<Player>& object);
        static void BuildJediStateDelta(const std::shared_ptr<Player>& object);
        // baselines
        static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<Player>& object);
        static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<Player>& object);
        static swganh::messages::BaselinesMessage BuildBaseline8(const std::shared_ptr<Player>& object);
        static swganh::messages::BaselinesMessage BuildBaseline9(const std::shared_ptr<Player>& object);

        typedef anh::ValueEvent<std::shared_ptr<Player>> PlayerEvent;
    };

}}}  // swganh::object::player

#endif  // SWGANH_OBJECT_PLAYER_PLAYER_MESSAGE_BUILDER_H_
