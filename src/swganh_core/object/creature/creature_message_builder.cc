// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "creature_message_builder.h"
#include "creature.h"
#include "swganh_core/messages/update_pvp_status_message.h"
#include "swganh_core/messages/controllers/posture.h"
#include "swganh_core/object/object_events.h"

using namespace std;
using namespace swganh;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::messages;

void CreatureMessageBuilder::RegisterEventHandlers()
{
    event_dispatcher->Subscribe("Creature::Baselines", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto observer_event = static_pointer_cast<ObserverEvent>(incoming_event);
        SendBaselines(static_pointer_cast<Creature>(observer_event->object), observer_event->observer);
    });
    event_dispatcher->Subscribe("Creature::Bank", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildBankCreditsDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::Cash", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildCashCreditsDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::StatBase", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildStatBaseDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::Skill", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildSkillDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::Posture", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildPostureDelta(value_event->Get());
        BuildPostureUpdate(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::FactionRank", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildFactionRankDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::OwnerId", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildOwnerIdDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::Scale", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildScaleDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::BattleFatigue", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildScaleDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::StateBitmask", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildStateBitmaskDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::StateBitmask", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildStateBitmaskDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::StatWound", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildStatWoundDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::AccelerationMultiplierBase", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildAccelerationMultiplierBaseDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::AccelerationMultiplierModifier", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildAccelerationMultiplierModifierDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::StatEncumberance", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildStatEncumberanceDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::SkillMod", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildSkillModDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::SpeedMultiplierBase", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildSpeedMultiplierBaseDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::SpeedMultiplierModifer", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildSpeedMultiplierModifierDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::ListenToId", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildListenToIdDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::RunSpeed", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildRunSpeedDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::SlopeModifierAngle", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildSlopeModifierAngleDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::SlopeModifierPercent", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildSlopeModifierPercentDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::TurnRadius", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildTurnRadiusDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::WalkingSpeed", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildWalkingSpeedDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::WaterModifierPercent", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildWaterModifierPrecentDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::MissionCriticalObject", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildMissionCriticalObjectDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::Animation", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildAnimationDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::MoodAnimation", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildMoodAnimationDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::WeaponId", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildWeaponIdDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::GroupId", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildGroupIdDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::InviteSenderId", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildInviteSenderIdDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::GuildId", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildGuildIdDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::TargetId", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildTargetIdDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::PerformanceId", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildPerformanceIdDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::StatCurrent", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildStatCurrentDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::StatMax", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildStatMaxDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::EquipmentItem", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildEquipmentDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::Disguise", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildDisguiseDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Creature::Stationary", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildStationaryDelta(value_event->Get());
    });
   event_dispatcher->Subscribe("Creature::PvPStatus", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<CreatureEvent>(incoming_event);
        BuildUpdatePvpStatusMessage(value_event->Get());
    });
}
void CreatureMessageBuilder::SendBaselines(const shared_ptr<Creature>& creature, const shared_ptr<swganh::observer::ObserverInterface>& observer)
{
    creature->AddBaselineToCache(&BuildBaseline1(creature));
    creature->AddBaselineToCache(&BuildBaseline3(creature));
    creature->AddBaselineToCache(&BuildBaseline4(creature));
    creature->AddBaselineToCache(&BuildBaseline6(creature));
    for (auto& baseline : creature->GetBaselines())
    {
        observer->Notify(&baseline);
    }
        
    SendEndBaselines(creature, observer);

    BuildUpdatePvpStatusMessage(creature);
}
void CreatureMessageBuilder::BuildBankCreditsDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_1, 0);
        message.data.write<uint32_t>(creature->GetBankCredits());   // Bank Credits
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildCashCreditsDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_1, 1);
        message.data.write<uint32_t>(creature->GetCashCredits());   // Cash Credits
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildStatBaseDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_1, 2);
        creature->GetBaseStats().Serialize(message);
        creature->AddDeltasUpdate(&message);
    }
    else
        creature->GetBaseStats().ClearDeltas();
}

void CreatureMessageBuilder::BuildSkillDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_1, 3);
        creature->GetSkills().Serialize(message);
        creature->AddDeltasUpdate(&message);
    }
    else
        creature->GetSkills().ClearDeltas();
}

void CreatureMessageBuilder::BuildPostureUpdate(const shared_ptr<Creature>& creature)
{
	if (creature->HasObservers())
    {
		// Update the posture message
		controllers::Posture posture;
		posture.posture_id = creature->GetPosture();
		creature->GetController()->Notify(&posture);
	}
}

void CreatureMessageBuilder::BuildPostureDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_3, 11);
        message.data.write<uint8_t>(creature->GetPosture());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildFactionRankDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_3, 12);
        message.data.write<uint8_t>(creature->GetFactionRank());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildOwnerIdDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_3, 13);
        message.data.write<uint64_t>(creature->GetOwnerId());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildScaleDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_3, 14);
        message.data.write<float>(creature->GetScale());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildBattleFatigueDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_3, 15);
        message.data.write<uint32_t>(creature->GetBattleFatigue());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildStateBitmaskDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_3, 16);
        message.data.write<uint64_t>(creature->GetStateBitmask());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildStatWoundDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_3, 17);
        creature->GetStatWounds().Serialize(message);
        creature->AddDeltasUpdate(&message);
    }
    else
        creature->GetStatWounds().ClearDeltas();
}

void CreatureMessageBuilder::BuildAccelerationMultiplierBaseDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 0);
        message.data.write<float>(creature->GetAccelerationMultiplierBase());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildAccelerationMultiplierModifierDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 1);
        message.data.write<float>(creature->GetAccelerationMultiplierModifier());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildStatEncumberanceDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 2);
        creature->GetStatEncumberances().Serialize(message);
        creature->AddDeltasUpdate(&message);
    }
    else
        creature->GetStatEncumberances().ClearDeltas();
}

void CreatureMessageBuilder::BuildSkillModDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 3);
        creature->GetSkillMods().Serialize(message);
        creature->AddDeltasUpdate(&message);
    }
    else
        creature->GetSkillMods().ClearDeltas();
}

void CreatureMessageBuilder::BuildSpeedMultiplierBaseDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 4);
        message.data.write<float>(creature->GetSpeedMultiplierBase());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildSpeedMultiplierModifierDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 5);
        message.data.write<float>(creature->GetSpeedMultiplierModifier());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildListenToIdDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 6);
        message.data.write<uint64_t>(creature->GetListenToId());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildRunSpeedDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 7);
        message.data.write<float>(creature->GetRunSpeed());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildSlopeModifierAngleDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 8);
        message.data.write<float>(creature->GetSlopeModifierAngle());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildSlopeModifierPercentDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 9);
        message.data.write<float>(creature->GetSlopeModifierPercent());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildTurnRadiusDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 10);
        message.data.write<float>(creature->GetTurnRadius());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildWalkingSpeedDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 11);
        message.data.write<float>(creature->GetWalkingSpeed());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildWaterModifierPrecentDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 12);
        message.data.write<float>(creature->GetWaterModifierPercent());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildMissionCriticalObjectDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_4, 13);
        creature->GetMissionCriticalObjects().Serialize(message);
        creature->AddDeltasUpdate(&message);
    }
    else
        creature->GetMissionCriticalObjects().ClearDeltas();
}

void CreatureMessageBuilder::BuildCombatLevelDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 2);
        message.data.write<uint16_t>(creature->GetCombatLevel());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildAnimationDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 3);
        message.data.write<std::string>(creature->GetAnimation());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildMoodAnimationDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 4);
        message.data.write<std::string>(creature->GetMoodAnimation());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildWeaponIdDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 5);
        message.data.write<uint64_t>(creature->GetWeaponId());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildGroupIdDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 6);
        message.data.write<uint64_t>(creature->GetGroupId());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildInviteSenderIdDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 7);
        message.data.write<uint64_t>(creature->IncrementInviteCounter());
        message.data.write<uint64_t>(creature->GetInviteSenderId());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildGuildIdDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 8);
        message.data.write<uint32_t>(creature->GetGuildId());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildTargetIdDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 9);
        message.data.write<uint64_t>(creature->GetTargetId());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildMoodIdDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 10);
        message.data.write<uint8_t>(creature->GetMoodId());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildPerformanceIdDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 12);
        message.data.write<uint32_t>(creature->GetPerformanceId());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildStatCurrentDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 13);
        creature->GetCurrentStats().Serialize(message);
        creature->AddDeltasUpdate(&message);
    }
    else
        creature->GetCurrentStats().ClearDeltas();
}

void CreatureMessageBuilder::BuildStatMaxDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 14);
        creature->GetMaxStats().Serialize(message);
        creature->AddDeltasUpdate(&message);
    }
    else
        creature->GetMaxStats().ClearDeltas();
}

void CreatureMessageBuilder::BuildEquipmentDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 15);
        creature->GetEquipment().Serialize(message);
        creature->AddDeltasUpdate(&message);
    }
    else
        creature->GetEquipment().ClearDeltas();
}

void CreatureMessageBuilder::BuildDisguiseDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 16);
        message.data.write<std::string>(creature->GetDisguise());
        creature->AddDeltasUpdate(&message);
    }
}

void CreatureMessageBuilder::BuildStationaryDelta(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(creature, Object::VIEW_6, 17);
        message.data.write<uint8_t>(creature->IsStationary());
        creature->AddDeltasUpdate(&message);
    }
}


void CreatureMessageBuilder::BuildUpdatePvpStatusMessage(const shared_ptr<Creature>& creature)
{
    if (creature->HasObservers())
    {
        UpdatePvpStatusMessage message;
        message.object_id = creature->GetObjectId();
        message.faction = 0;
        message.pvp_status = creature->GetPvpStatus();
        creature->NotifyObservers(&message);
    }
}

BaselinesMessage CreatureMessageBuilder::BuildBaseline1(const shared_ptr<Creature>& creature)
{
    auto message = CreateBaselinesMessage(creature, Object::VIEW_1, 4);
    message.data.write<uint32_t>(creature->GetBankCredits());                  // Bank Credits
    message.data.write<uint32_t>(creature->GetCashCredits());                  // Cash Credits
    creature->GetBaseStats().Serialize(message);                           // Stats Negative
    creature->GetSkills().Serialize(message);                                   // Skills
    
    return BaselinesMessage(std::move(message));
}

BaselinesMessage CreatureMessageBuilder::BuildBaseline3(const shared_ptr<Creature>& creature)
{
    auto message = CreateBaselinesMessage(creature, Object::VIEW_3, 18);
    message.data.append(TangibleMessageBuilder::BuildBaseline3(creature).data);
    message.data.write<uint8_t>(creature->GetPosture());                        // Posture
    message.data.write<uint8_t>(creature->GetFactionRank());                   // Faction Rank
    message.data.write<uint64_t>(creature->GetOwnerId());                      // Owner Id
    message.data.write<float>(creature->GetScale());                            // Scale
    message.data.write<uint32_t>(creature->GetBattleFatigue());                // Battle Fatigue
    message.data.write<uint64_t>(creature->GetStateBitmask());                 // States Bitmask
    creature->GetStatWounds().Serialize(message);                          // Stat Wounds
    
    return BaselinesMessage(move(message));
}

BaselinesMessage CreatureMessageBuilder::BuildBaseline4(const shared_ptr<Creature>& creature)
{
    auto message = CreateBaselinesMessage(creature, Object::VIEW_4, 20);
    message.data.write<float>(creature->GetAccelerationMultiplierBase());         // Acceleration Multiplier Base
    message.data.write<float>(creature->GetAccelerationMultiplierModifier());     // Acceleration Multiplier Modifier
    creature->GetStatEncumberances().Serialize(message);                       // Stat Encumberances
    creature->GetSkillMods().Serialize(message);                               // Skill Mods
    message.data.write<float>(creature->GetSpeedMultiplierBase());                // Speed Multiplier Base
    message.data.write<float>(creature->GetSpeedMultiplierModifier());            // Speed Multiplier Modifier
    message.data.write<uint64_t>(creature->GetListenToId());                      // Listen To Id
    message.data.write<float>(creature->GetRunSpeed());                            // Run Speed
    message.data.write<float>(creature->GetSlopeModifierAngle());                 // Slope Modifier Angle
    message.data.write<float>(creature->GetSlopeModifierPercent());               // Slope Modifier Percent
    message.data.write<float>(creature->GetTurnRadius());                          // Turn Radius
    message.data.write<float>(creature->GetWalkingSpeed());                        // Walking Speed
    message.data.write<float>(creature->GetWaterModifierPercent());               // Water Modifier Percent
    creature->GetMissionCriticalObjects().Serialize(message);                 // Mission Critical Object
    
    return BaselinesMessage(move(message));
}

BaselinesMessage CreatureMessageBuilder::BuildBaseline6(const shared_ptr<Creature>& creature)
{
    auto message = CreateBaselinesMessage(creature, Object::VIEW_6, 23);
    message.data.append(TangibleMessageBuilder::BuildBaseline6(creature).data);
    message.data.write<uint16_t>(creature->GetCombatLevel());                      // Combat Level
    message.data.write<std::string>(creature->GetAnimation());                      // Current Animation
    message.data.write<std::string>(creature->GetMoodAnimation());                 // Mood Animation
    message.data.write<uint64_t>(creature->GetWeaponId());                         // Weapon Id
    message.data.write<uint64_t>(creature->GetGroupId());                          // Group Id
    message.data.write<uint64_t>(creature->GetInviteSenderId());                  // Invite Sender Id
    message.data.write<uint64_t>(creature->IncrementInviteCounter());                  // Invite Sender Counter
    message.data.write<uint32_t>(creature->GetGuildId());                          // Guild Id
    message.data.write<uint64_t>(creature->GetTargetId());                         // Target Id
    message.data.write<uint8_t>(creature->GetMoodId());                            // Mood Id
    message.data.write<uint32_t>(creature->IncrementPerformanceCounter());             // Performance Update Counter
    message.data.write<uint32_t>(creature->GetPerformanceId());                    // Performance Id
    creature->GetCurrentStats().Serialize(message);                            // Current Stats
    creature->GetMaxStats().Serialize(message);                                // Max Stats
    creature->GetEquipment().Serialize(message);                               // Equipment
    message.data.write<std::string>(creature->GetDisguise());                       // Disguise Template
    message.data.write<uint8_t>(creature->IsStationary());                         // Stationary
    
    return BaselinesMessage(move(message));
}