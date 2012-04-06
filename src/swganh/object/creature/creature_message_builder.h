/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_OBJECT_CREATURE_CREATURE_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_CREATURE_CREATURE_MESSAGE_BUILDER_H_

#include <cstdint>
#include <string>
#include <boost/optional.hpp>

#include "swganh/object/tangible/tangible_message_builder.h"

#include "creature.h"

namespace swganh {
namespace object {
namespace creature {

class CreatureMessageBuilder : public swganh::object::tangible::TangibleMessageBuilder
{
public:
    CreatureMessageBuilder(anh::EventDispatcher* event_dispatcher_)
        : TangibleMessageBuilder(event_dispatcher_) 
    {
        RegisterEventHandlers();
    }
    virtual void RegisterEventHandlers();
    virtual void SendBaselines(std::shared_ptr<Creature> creature, std::shared_ptr<ObjectController> controller);
private:
    // deltas
    static void BuildBankCreditsDelta(std::shared_ptr<Creature> creature);
    static void BuildCashCreditsDelta(std::shared_ptr<Creature> creature);
    static void BuildStatBaseDelta(std::shared_ptr<Creature> creature);
    static void BuildSkillDelta(std::shared_ptr<Creature> creature);
    static void BuildPostureDelta(std::shared_ptr<Creature> creature);
	static void BuildPostureUpdate(std::shared_ptr<Creature> creature);
    static void BuildFactionRankDelta(std::shared_ptr<Creature> creature);
    static void BuildOwnerIdDelta(std::shared_ptr<Creature> creature);
    static void BuildScaleDelta(std::shared_ptr<Creature> creature);
    static void BuildBattleFatigueDelta(std::shared_ptr<Creature> creature);
    static void BuildStateBitmaskDelta(std::shared_ptr<Creature> creature);
    static void BuildStatWoundDelta(std::shared_ptr<Creature> creature);
    static void BuildAccelerationMultiplierBaseDelta(std::shared_ptr<Creature> creature);
    static void BuildAccelerationMultiplierModifierDelta(std::shared_ptr<Creature> creature);
    static void BuildStatEncumberanceDelta(std::shared_ptr<Creature> creature);
    static void BuildSkillModDelta(std::shared_ptr<Creature> creature);
    static void BuildSpeedMultiplierBaseDelta(std::shared_ptr<Creature> creature);
    static void BuildSpeedMultiplierModifierDelta(std::shared_ptr<Creature> creature);
    static void BuildListenToIdDelta(std::shared_ptr<Creature> creature);
    static void BuildRunSpeedDelta(std::shared_ptr<Creature> creature);
    static void BuildSlopeModifierAngleDelta(std::shared_ptr<Creature> creature);
    static void BuildSlopeModifierPercentDelta(std::shared_ptr<Creature> creature);
    static void BuildTurnRadiusDelta(std::shared_ptr<Creature> creature);
    static void BuildWalkingSpeedDelta(std::shared_ptr<Creature> creature);
    static void BuildWaterModifierPrecentDelta(std::shared_ptr<Creature> creature);
    static void BuildMissionCriticalObjectDelta(std::shared_ptr<Creature> creature);
    static void BuildCombatLevelDelta(std::shared_ptr<Creature> creature);
    static void BuildAnimationDelta(std::shared_ptr<Creature> creature);
    static void BuildMoodAnimationDelta(std::shared_ptr<Creature> creature);
    static void BuildWeaponIdDelta(std::shared_ptr<Creature> creature);
    static void BuildGroupIdDelta(std::shared_ptr<Creature> creature);
    static void BuildInviteSenderIdDelta(std::shared_ptr<Creature> creature);
    static void BuildGuildIdDelta(std::shared_ptr<Creature> creature);
    static void BuildTargetIdDelta(std::shared_ptr<Creature> creature);
    static void BuildMoodIdDelta(std::shared_ptr<Creature> creature);
    static void BuildPerformanceIdDelta(std::shared_ptr<Creature> creature);
    static void BuildStatCurrentDelta(std::shared_ptr<Creature> creature);
    static void BuildStatMaxDelta(std::shared_ptr<Creature> creature);
    static void BuildEquipmentDelta(std::shared_ptr<Creature> creature);
    static void BuildDisguiseDelta(std::shared_ptr<Creature> creature);
    static void BuildStationaryDelta(std::shared_ptr<Creature> creature);
    static void BuildUpdatePvpStatusMessage(std::shared_ptr<Creature> object);

    // baselines
    static swganh::messages::BaselinesMessage BuildBaseline1(std::shared_ptr<Creature> creature);
    static swganh::messages::BaselinesMessage BuildBaseline3(std::shared_ptr<Creature> creature);
    static swganh::messages::BaselinesMessage BuildBaseline4(std::shared_ptr<Creature> creature);
    static swganh::messages::BaselinesMessage BuildBaseline6(std::shared_ptr<Creature> creature);

    typedef anh::ValueEvent<std::shared_ptr<Creature>> CreatureEvent;
};

}}} // swganh::object::creature

#endif // SWGANH_OBJECT_CREATURE_CREATURE_MESSAGE_BUILDER_H_