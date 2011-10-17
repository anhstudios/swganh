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
void CreatureMessageBuilder::BuildWaterModifierPrecent(Creature* creature){}

boost::optional<swganh::messages::BaselinesMessage> CreatureMessageBuilder::BuildBaseline3(Creature* creature)
{
    auto message = creature->CreateBaselinesMessage(Object::VIEW_3, 0);
    message.data.write<uint16_t>(0); // Customization String
    message.data.write<uint32_t>(0); // Customization Component List Size
    message.data.write<uint32_t>(0); // Customization Component List Counter
    message.data.write<uint32_t>(0); // Options Bitmask
    message.data.write<uint32_t>(0); // Incap Timer
    message.data.write<uint32_t>(0); // Critical Damage
    message.data.write<uint32_t>(0); // Max Condition
    message.data.write<uint8_t>(0); // ???
    message.data.write<uint8_t>(0); // Posture
    message.data.write<uint8_t>(0); // Faction Rank
    message.data.write<uint64_t>(0); // Owner Id
    message.data.write<float>(1.0f); // Scale
    message.data.write<uint32_t>(0); // Battle Fatigue
    message.data.write<uint32_t>(512); // States Bitmask
    message.data.write<uint32_t>(0); // Wound List Size
    message.data.write<uint32_t>(0); // Wond List Counter
    return boost::optional<swganh::messages::BaselinesMessage>(std::move(message));
}