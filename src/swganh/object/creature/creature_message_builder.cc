#include "creature_message_builder.h"
#include "creature.h"

using namespace swganh::object::creature;

void CreatureMessageBuilder::BuildBankCreditsDelta(Creature* creature){}
void CreatureMessageBuilder::BuildCashCreditsDelta(Creature* creature){}
void CreatureMessageBuilder::BuildSkillListDelta(Creature* creature, uint8_t subtype, std::string skill){}
void CreatureMessageBuilder::BuildPostureDelta(Creature* creature){}
void CreatureMessageBuilder::BuildFactionRankDelta(Creature* creature){}
void CreatureMessageBuilder::BuildOwnerIdDelta(Creature* creature){}
void CreatureMessageBuilder::BuildScaleDelta(Creature* creature){}
void CreatureMessageBuilder::BuildBattleFatigueDelta(Creature* creature){}
void CreatureMessageBuilder::BuildStateBitmaskDelta(Creature* creature){}
void CreatureMessageBuilder::BuildAccelerationMultiplierBaseDelta(Creature* creature){}
void CreatureMessageBuilder::BuildAccelerationMultiplierModifierDelta(Creature* creature){}
void CreatureMessageBuilder::BuildSpeedMultiplierBaseDelta(Creature* creature){}
void CreatureMessageBuilder::BuildSpeedMultiplierModifierDelta(Creature* creature){}
void CreatureMessageBuilder::BuildListenToIdDelta(Creature* creature){}
void CreatureMessageBuilder::BuildRunSpeedDelta(Creature* creature){}
void CreatureMessageBuilder::BuildSlopeModifierAngleDelta(Creature* creature){}
void CreatureMessageBuilder::BuildSlopeModifierPercentDelta(Creature* creature){}
void CreatureMessageBuilder::BuildTurnRadiusDelta(Creature* creature){}
void CreatureMessageBuilder::BuildWalkingSpeedDelta(Creature* creature){}
void CreatureMessageBuilder::BuildWaterModifierPrecentDelta(Creature* creature){}
void CreatureMessageBuilder::BuildCombatLevelDelta(Creature* creature){}
void CreatureMessageBuilder::BuildAnimationDelta(Creature* creature){}
void CreatureMessageBuilder::BuildMoodAnimationDelta(Creature* creature){}
void CreatureMessageBuilder::BuildWeaponIdDelta(Creature* creature){}
void CreatureMessageBuilder::BuildGroupIdDelta(Creature* creature){}
void CreatureMessageBuilder::BuildInviteSenderIdDelta(Creature* creature){}
void CreatureMessageBuilder::BuildGuildIdDelta(Creature* creature){}
void CreatureMessageBuilder::BuildTargetIdDelta(Creature* creature){}
void CreatureMessageBuilder::BuildMoodIdDelta(Creature* creature){}
void CreatureMessageBuilder::BuildPerformanceIdDelta(Creature* creature){}
void CreatureMessageBuilder::BuildStationaryDelta(Creature* creature){}

boost::optional<swganh::messages::BaselinesMessage> CreatureMessageBuilder::BuildBaseline1(Creature* creature)
{
    auto message = creature->CreateBaselinesMessage(Object::VIEW_1, 4);
    message.data.write<uint32_t>(creature->GetBankCredits());
    message.data.write<uint32_t>(creature->GetCashCredits());
    message.data.write<uint32_t>(0); // Negative HAM List Size
    message.data.write<uint32_t>(0); // Negative HAM List Counter
    message.data.write<uint32_t>(0); // Skill List Size
    message.data.write<uint32_t>(0); // Skill List Counter
    return boost::optional<swganh::messages::BaselinesMessage>(std::move(message));
}

boost::optional<swganh::messages::BaselinesMessage> CreatureMessageBuilder::BuildBaseline3(Creature* creature)
{
    auto message = creature->CreateBaselinesMessage(Object::VIEW_3, 18);
    message.data.append(creature->BaseTangible::GetBaseline3().get().data);
    message.data.write<uint8_t>(creature->GetPosture()); // Posture
    message.data.write<uint8_t>(creature->GetFactionRank()); // Faction Rank
    message.data.write<uint64_t>(creature->GetOwnerId()); // Owner Id
    message.data.write<float>(creature->GetScale()); // Scale
    message.data.write<uint32_t>(creature->GetBattleFatigue()); // Battle Fatigue
    message.data.write<uint64_t>(creature->GetStateBitmask()); // States Bitmask
    message.data.write<uint32_t>(0); // Wound List Size
    message.data.write<uint32_t>(0); // Wound List Counter
    return boost::optional<swganh::messages::BaselinesMessage>(std::move(message));
}

boost::optional<swganh::messages::BaselinesMessage> CreatureMessageBuilder::BuildBaseline4(Creature* creature)
{
    auto message = creature->CreateBaselinesMessage(Object::VIEW_4, 20);
    message.data.write<float>(creature->GetAccelerationMultiplierBase());
    message.data.write<float>(creature->GetAccelerationMultiplierModifier());
    message.data.write<uint32_t>(0); // Encumberance HAM List Size
    message.data.write<uint32_t>(0); // Encumberance HAM List Counter
    message.data.write<uint32_t>(0); // Skill Mod List Size
    message.data.write<uint32_t>(0); // Skill Mod List Counter
    message.data.write<float>(creature->GetSpeedMultiplierBase());
    message.data.write<float>(creature->GetSpeedMultiplierModifier());
    message.data.write<uint64_t>(creature->GetListenToId());
    message.data.write<float>(creature->GetRunSpeed());
    message.data.write<float>(creature->GetSlopeModifierAngle());
    message.data.write<float>(creature->GetSlopeModifierPercent());
    message.data.write<float>(creature->GetTurnRadius());
    message.data.write<float>(creature->GetWalkingSpeed());
    message.data.write<float>(creature->GetWaterModifierPercent());
    message.data.write<uint32_t>(0); // Group Mission List Size
    message.data.write<uint32_t>(0); // Group Mission List Counter
    return boost::optional<swganh::messages::BaselinesMessage>(std::move(message));
}

boost::optional<swganh::messages::BaselinesMessage> CreatureMessageBuilder::BuildBaseline6(Creature* creature)
{
    auto message = creature->CreateBaselinesMessage(Object::VIEW_6, 23);
    message.data.append(creature->BaseTangible::GetBaseline6().get().data);
    message.data.write<uint16_t>(creature->GetCombatLevel()); // Combat Level
    message.data.write<std::string>(creature->GetAnimation()); // Current Animation
    message.data.write<std::string>(creature->GetMoodAnimation()); // Mood Animation
    message.data.write<uint64_t>(creature->GetWeaponId()); // Weapon Id
    message.data.write<uint64_t>(creature->GetGroupId()); // Group Id
    message.data.write<uint64_t>(creature->GetInviteSenderId()); // Invite Sender Id
    message.data.write<uint64_t>(creature->invite_counter_++); // Invite Sender Counter
    message.data.write<uint32_t>(creature->GetGuildId()); // Guild Id
    message.data.write<uint64_t>(creature->GetTargetId()); // Target Id
    message.data.write<uint8_t>(creature->GetMoodId()); // Mood Id
    message.data.write<uint32_t>(creature->performance_counter_++); // Performance Update Counter
    message.data.write<uint32_t>(creature->GetPerformanceId()); // Performance Id
    message.data.write<uint32_t>(0); // Current HAM List Size
    message.data.write<uint32_t>(0); // Current HAM List Counter
    message.data.write<uint32_t>(0); // Max HAM List Size
    message.data.write<uint32_t>(0); // Max HAM List Counter
    message.data.write<uint32_t>(0); // Equipment List Size
    message.data.write<uint32_t>(0); // Equipment List Counter
    message.data.write<std::string>(creature->GetTemplate()); // Template
    message.data.write<uint8_t>(creature->GetStationary());
    return boost::optional<swganh::messages::BaselinesMessage>(std::move(message));
}