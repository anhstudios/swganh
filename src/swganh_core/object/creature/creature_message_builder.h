// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include <boost/optional.hpp>

#include "swganh_core/object/tangible/tangible_message_builder.h"

#include "creature.h"

namespace swganh {
namespace object {


class CreatureMessageBuilder : public swganh::object::TangibleMessageBuilder
{
public:
    CreatureMessageBuilder(swganh::EventDispatcher* event_dispatcher_)
        : TangibleMessageBuilder(event_dispatcher_) 
    {
        RegisterEventHandlers();
    }
    virtual void RegisterEventHandlers();
    virtual void SendBaselines(const std::shared_ptr<Creature>& creature, const std::shared_ptr<swganh::observer::ObserverInterface>& observer);
private:
    // deltas
    static void BuildBankCreditsDelta(const std::shared_ptr<Creature>& creature);
    static void BuildCashCreditsDelta(const std::shared_ptr<Creature>& creature);
    static void BuildStatBaseDelta(const std::shared_ptr<Creature>& creature);
    static void BuildSkillDelta(const std::shared_ptr<Creature>& creature);
    static void BuildPostureDelta(const std::shared_ptr<Creature>& creature);
	static void BuildPostureUpdate(const std::shared_ptr<Creature>& creature);
    static void BuildFactionRankDelta(const std::shared_ptr<Creature>& creature);
    static void BuildOwnerIdDelta(const std::shared_ptr<Creature>& creature);
    static void BuildScaleDelta(const std::shared_ptr<Creature>& creature);
    static void BuildBattleFatigueDelta(const std::shared_ptr<Creature>& creature);
    static void BuildStateBitmaskDelta(const std::shared_ptr<Creature>& creature);
    static void BuildStatWoundDelta(const std::shared_ptr<Creature>& creature);
    static void BuildAccelerationMultiplierBaseDelta(const std::shared_ptr<Creature>& creature);
    static void BuildAccelerationMultiplierModifierDelta(const std::shared_ptr<Creature>& creature);
    static void BuildStatEncumberanceDelta(const std::shared_ptr<Creature>& creature);
    static void BuildSkillModDelta(const std::shared_ptr<Creature>& creature);
    static void BuildSpeedMultiplierBaseDelta(const std::shared_ptr<Creature>& creature);
    static void BuildSpeedMultiplierModifierDelta(const std::shared_ptr<Creature>& creature);
    static void BuildListenToIdDelta(const std::shared_ptr<Creature>& creature);
    static void BuildRunSpeedDelta(const std::shared_ptr<Creature>& creature);
    static void BuildSlopeModifierAngleDelta(const std::shared_ptr<Creature>& creature);
    static void BuildSlopeModifierPercentDelta(const std::shared_ptr<Creature>& creature);
    static void BuildTurnRadiusDelta(const std::shared_ptr<Creature>& creature);
    static void BuildWalkingSpeedDelta(const std::shared_ptr<Creature>& creature);
    static void BuildWaterModifierPrecentDelta(const std::shared_ptr<Creature>& creature);
    static void BuildMissionCriticalObjectDelta(const std::shared_ptr<Creature>& creature);
    static void BuildCombatLevelDelta(const std::shared_ptr<Creature>& creature);
    static void BuildAnimationDelta(const std::shared_ptr<Creature>& creature);
    static void BuildMoodAnimationDelta(const std::shared_ptr<Creature>& creature);
    static void BuildWeaponIdDelta(const std::shared_ptr<Creature>& creature);
    static void BuildGroupIdDelta(const std::shared_ptr<Creature>& creature);
    static void BuildInviteSenderIdDelta(const std::shared_ptr<Creature>& creature);
    static void BuildGuildIdDelta(const std::shared_ptr<Creature>& creature);
    static void BuildTargetIdDelta(const std::shared_ptr<Creature>& creature);
    static void BuildMoodIdDelta(const std::shared_ptr<Creature>& creature);
    static void BuildPerformanceIdDelta(const std::shared_ptr<Creature>& creature);
    static void BuildStatCurrentDelta(const std::shared_ptr<Creature>& creature);
    static void BuildStatMaxDelta(const std::shared_ptr<Creature>& creature);
    static void BuildEquipmentDelta(const std::shared_ptr<Creature>& creature);
    static void BuildDisguiseDelta(const std::shared_ptr<Creature>& creature);
    static void BuildStationaryDelta(const std::shared_ptr<Creature>& creature);
    static void BuildUpdatePvpStatusMessage(const std::shared_ptr<Creature>& object);

    // baselines
    static swganh::messages::BaselinesMessage BuildBaseline1(const std::shared_ptr<Creature>& creature);
    static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<Creature>& creature);
    static swganh::messages::BaselinesMessage BuildBaseline4(const std::shared_ptr<Creature>& creature);
    static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<Creature>& creature);

    typedef swganh::ValueEvent<std::shared_ptr<Creature>> CreatureEvent;
};

}} // swganh::object
