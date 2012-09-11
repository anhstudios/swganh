// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/intangible/intangible_message_builder.h"

#include "player.h"

namespace swganh {
namespace object {

    class Waypoint;
}} // swganh::object

namespace swganh {
namespace object {

    class PlayerMessageBuilder : public swganh::object::IntangibleMessageBuilder
    {
    public:
        PlayerMessageBuilder(swganh::EventDispatcher* dispatcher) :
            IntangibleMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void SendBaselines(const std::shared_ptr<Player>& player, const std::shared_ptr<swganh::observer::ObserverInterface>& controller);
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

        typedef swganh::ValueEvent<std::shared_ptr<Player>> PlayerEvent;
    };

}}  // swganh::object
