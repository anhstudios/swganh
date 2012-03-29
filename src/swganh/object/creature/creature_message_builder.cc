#include "creature_message_builder.h"
#include "creature.h"
#include "swganh/messages/update_pvp_status_message.h"
#include "swganh/messages/controllers/posture.h"

using namespace swganh::object::creature;
using namespace swganh::messages;

void CreatureMessageBuilder::BuildBankCreditsDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_1, 0);
        message.data.write<uint32_t>(creature->bank_credits_);   // Bank Credits
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildCashCreditsDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_1, 1);
        message.data.write<uint32_t>(creature->cash_credits_);   // Cash Credits
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildStatBaseDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_1, 2);
        creature->stat_base_list_.Serialize(message);
        creature->AddDeltasUpdate(std::move(message));
    }
    else
        creature->stat_base_list_.ClearDeltas();
}

void CreatureMessageBuilder::BuildSkillDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_1, 3);
        creature->skills_.Serialize(message);
        creature->AddDeltasUpdate(std::move(message));
    }
    else
        creature->skills_.ClearDeltas();
}

void CreatureMessageBuilder::BuildPostureUpdate(Creature* creature)
{
	if (creature->HasObservers())
    {
		// Update the posture message
		controllers::Posture posture;
		posture.posture_id = creature->GetPosture();
		creature->GetController()->Notify(ObjControllerMessage(0x1B, posture));
	}
}

void CreatureMessageBuilder::BuildPostureDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 11);
        message.data.write<uint8_t>(creature->posture_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildFactionRankDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 12);
        message.data.write<uint8_t>(creature->faction_rank_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildOwnerIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 13);
        message.data.write<uint64_t>(creature->owner_id_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildScaleDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 14);
        message.data.write<float>(creature->scale_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildBattleFatigueDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 15);
        message.data.write<uint32_t>(creature->battle_fatigue_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildStateBitmaskDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 16);
        message.data.write<uint64_t>(creature->state_bitmask_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildStatWoundDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 17);
        creature->stat_wound_list_.Serialize(message);
        creature->AddDeltasUpdate(std::move(message));
    }
    else
        creature->stat_wound_list_.ClearDeltas();
}

void CreatureMessageBuilder::BuildAccelerationMultiplierBaseDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 0);
        message.data.write<float>(creature->acceleration_multiplier_base_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildAccelerationMultiplierModifierDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 1);
        message.data.write<float>(creature->acceleration_multiplier_modifier_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildStatEncumberanceDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 2);
        creature->stat_encumberance_list_.Serialize(message);
        creature->AddDeltasUpdate(std::move(message));
    }
    else
        creature->stat_encumberance_list_.ClearDeltas();
}

void CreatureMessageBuilder::BuildSkillModDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 3);
        creature->skill_mod_list_.Serialize(message);
        creature->AddDeltasUpdate(std::move(message));
    }
    else
        creature->skill_mod_list_.ClearDeltas();
}

void CreatureMessageBuilder::BuildSpeedMultiplierBaseDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 4);
        message.data.write<float>(creature->speed_multiplier_base_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildSpeedMultiplierModifierDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 5);
        message.data.write<float>(creature->speed_multiplier_modifier_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildListenToIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 6);
        message.data.write<uint64_t>(creature->listen_to_id_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildRunSpeedDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 7);
        message.data.write<float>(creature->run_speed_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildSlopeModifierAngleDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 8);
        message.data.write<float>(creature->slope_modifier_angle_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildSlopeModifierPercentDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 9);
        message.data.write<float>(creature->slope_modifier_percent_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildTurnRadiusDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 10);
        message.data.write<float>(creature->turn_radius_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildWalkingSpeedDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 11);
        message.data.write<float>(creature->walking_speed_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildWaterModifierPrecentDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 12);
        message.data.write<float>(creature->water_modifier_percent_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildMissionCriticalObjectDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 13);
        creature->mission_critical_object_list_.Serialize(message);
        creature->AddDeltasUpdate(std::move(message));
    }
    else
        creature->mission_critical_object_list_.ClearDeltas();
}

void CreatureMessageBuilder::BuildCombatLevelDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 2);
        message.data.write<uint16_t>(creature->combat_level_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildAnimationDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 3);
        message.data.write<std::string>(creature->animation_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildMoodAnimationDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 4);
        message.data.write<std::string>(creature->mood_animation_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildWeaponIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 5);
        message.data.write<uint64_t>(creature->weapon_id_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildGroupIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 6);
        message.data.write<uint64_t>(creature->group_id_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildInviteSenderIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 7);
        message.data.write<uint64_t>(creature->invite_counter_++);
        message.data.write<uint64_t>(creature->invite_sender_id_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildGuildIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 8);
        message.data.write<uint32_t>(creature->guild_id_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildTargetIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 9);
        message.data.write<uint64_t>(creature->target_id_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildMoodIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 10);
        message.data.write<uint8_t>(creature->mood_id_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildPerformanceIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 12);
        message.data.write<uint32_t>(creature->performance_id_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildStatCurrentDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 13);
        creature->stat_current_list_.Serialize(message);
        creature->AddDeltasUpdate(std::move(message));
    }
    else
        creature->stat_current_list_.ClearDeltas();
}

void CreatureMessageBuilder::BuildStatMaxDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 14);
        creature->stat_max_list_.Serialize(message);
        creature->AddDeltasUpdate(std::move(message));
    }
    else
        creature->stat_max_list_.ClearDeltas();
}

void CreatureMessageBuilder::BuildEquipmentDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 15);
        creature->equipment_list_.Serialize(message);
        creature->AddDeltasUpdate(std::move(message));
    }
    else
        creature->equipment_list_.ClearDeltas();
}

void CreatureMessageBuilder::BuildDisguiseDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 16);
        message.data.write<std::string>(creature->disguise_);
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildStationaryDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 17);
        message.data.write<uint8_t>(creature->stationary_);
        creature->AddDeltasUpdate(std::move(message));
    }
}


void CreatureMessageBuilder::BuildUpdatePvpStatusMessage(Creature* creature)
{
    if (creature->HasObservers())
    {
        UpdatePvpStatusMessage message;
        message.object_id = creature->object_id_;
        message.faction = 0;
        message.pvp_status = creature->pvp_status_;

        creature->NotifyObservers(message);
    }
}

boost::optional<swganh::messages::BaselinesMessage> CreatureMessageBuilder::BuildBaseline1(Creature* creature)
{
    auto message = creature->CreateBaselinesMessage(Object::VIEW_1, 4);
    message.data.write<uint32_t>(creature->bank_credits_);                  // Bank Credits
    message.data.write<uint32_t>(creature->cash_credits_);                  // Cash Credits
    creature->stat_base_list_.Serialize(message);                           // Stats Negative
    creature->skills_.Serialize(message);                                   // Skills
    return boost::optional<swganh::messages::BaselinesMessage>(std::move(message));
}

boost::optional<swganh::messages::BaselinesMessage> CreatureMessageBuilder::BuildBaseline3(Creature* creature)
{
    auto message = creature->CreateBaselinesMessage(Object::VIEW_3, 18);
    message.data.append(creature->Tangible::GetBaseline3().get().data);
    message.data.write<uint8_t>(creature->posture_);                        // Posture
    message.data.write<uint8_t>(creature->faction_rank_);                   // Faction Rank
    message.data.write<uint64_t>(creature->owner_id_);                      // Owner Id
    message.data.write<float>(creature->scale_);                            // Scale
    message.data.write<uint32_t>(creature->battle_fatigue_);                // Battle Fatigue
    message.data.write<uint64_t>(creature->state_bitmask_);                 // States Bitmask
    creature->stat_wound_list_.Serialize(message);                          // Stat Wounds
    return boost::optional<swganh::messages::BaselinesMessage>(std::move(message));
}

boost::optional<swganh::messages::BaselinesMessage> CreatureMessageBuilder::BuildBaseline4(Creature* creature)
{
    auto message = creature->CreateBaselinesMessage(Object::VIEW_4, 20);
    message.data.write<float>(creature->acceleration_multiplier_base_);         // Acceleration Multiplier Base
    message.data.write<float>(creature->acceleration_multiplier_modifier_);     // Acceleration Multiplier Modifier
    creature->stat_encumberance_list_.Serialize(message);                       // Stat Encumberances
    creature->skill_mod_list_.Serialize(message);                               // Skill Mods
    message.data.write<float>(creature->speed_multiplier_base_);                // Speed Multiplier Base
    message.data.write<float>(creature->speed_multiplier_modifier_);            // Speed Multiplier Modifier
    message.data.write<uint64_t>(creature->listen_to_id_);                      // Listen To Id
    message.data.write<float>(creature->run_speed_);                            // Run Speed
    message.data.write<float>(creature->slope_modifier_angle_);                 // Slope Modifier Angle
    message.data.write<float>(creature->slope_modifier_percent_);               // Slope Modifier Percent
    message.data.write<float>(creature->turn_radius_);                          // Turn Radius
    message.data.write<float>(creature->walking_speed_);                        // Walking Speed
    message.data.write<float>(creature->water_modifier_percent_);               // Water Modifier Percent
    creature->mission_critical_object_list_.Serialize(message);                 // Mission Critical Object
    return boost::optional<swganh::messages::BaselinesMessage>(std::move(message));
}

boost::optional<swganh::messages::BaselinesMessage> CreatureMessageBuilder::BuildBaseline6(Creature* creature)
{
    auto message = creature->CreateBaselinesMessage(Object::VIEW_6, 23);
    message.data.append(creature->Tangible::GetBaseline6().get().data);
    message.data.write<uint16_t>(creature->combat_level_);                      // Combat Level
    message.data.write<std::string>(creature->animation_);                      // Current Animation
    message.data.write<std::string>(creature->mood_animation_);                 // Mood Animation
    message.data.write<uint64_t>(creature->weapon_id_);                         // Weapon Id
    message.data.write<uint64_t>(creature->group_id_);                          // Group Id
    message.data.write<uint64_t>(creature->invite_sender_id_);                  // Invite Sender Id
    message.data.write<uint64_t>(creature->invite_counter_++);                  // Invite Sender Counter
    message.data.write<uint32_t>(creature->guild_id_);                          // Guild Id
    message.data.write<uint64_t>(creature->target_id_);                         // Target Id
    message.data.write<uint8_t>(creature->mood_id_);                            // Mood Id
    message.data.write<uint32_t>(creature->performance_counter_++);             // Performance Update Counter
    message.data.write<uint32_t>(creature->performance_id_);                    // Performance Id
    creature->stat_current_list_.Serialize(message);                            // Current Stats
    creature->stat_max_list_.Serialize(message);                                // Max Stats
    creature->equipment_list_.Serialize(message);                               // Equipment
    message.data.write<std::string>(creature->disguise_);                       // Disguise Template
    message.data.write<uint8_t>(creature->stationary_);                         // Stationary
    return boost::optional<swganh::messages::BaselinesMessage>(std::move(message));
}