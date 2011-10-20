#include "creature_message_builder.h"
#include "creature.h"

using namespace swganh::object::creature;
using namespace swganh::messages;

void CreatureMessageBuilder::BuildBankCreditsDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_1, 0);
        message.data.write<uint32_t>(creature->GetBankCredits());   // Bank Credits
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildCashCreditsDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_1, 1);
        message.data.write<uint32_t>(creature->GetCashCredits());   // Cash Credits
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildStatBaseDelta(Creature* creature, uint8_t subtype, uint16_t index)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_1, 2);
        message.data.write<uint32_t>(1);
        message.data.write<uint32_t>(creature->stat_base_list_counter_++);
        message.data.write<uint8_t>(subtype);

        switch(subtype)
        {
        case 0: // Remove
            message.data.write<uint16_t>(index);
            break;

        case 1: // Add
            message.data.write<uint16_t>(index);
            message.data.write<uint32_t>(creature->GetBaseStats()[index]);
            break;

        case 2: // Change
            message.data.write<uint16_t>(index);
            message.data.write<uint32_t>(creature->GetBaseStats()[index]);
            break;

        default:
            throw new std::runtime_error("Invalid subtype building stat base delta.");
            break;
        }
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildSkillDelta(Creature* creature, uint8_t subtype, std::string skill)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_1, 3);
        message.data.write<uint32_t>(1);
        message.data.write<uint32_t>(creature->skill_list_counter_++);
        message.data.write<uint8_t>(subtype);

        switch(subtype)
        {
        case 0: // Remove
            message.data.write<std::string>(skill);
            break;

        case 1: // Add
            message.data.write<std::string>(skill);
            break;

        default:
            throw new std::runtime_error("Invalid subtype building skill delta");
            break;
        };
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildPostureDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 11);
        message.data.write<uint8_t>(creature->GetPosture());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildFactionRankDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 12);
        message.data.write<uint8_t>(creature->GetFactionRank());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildOwnerIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 13);
        message.data.write<uint64_t>(creature->GetOwnerId());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildScaleDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 14);
        message.data.write<float>(creature->GetScale());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildBattleFatigueDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 15);
        message.data.write<uint32_t>(creature->GetBattleFatigue());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildStateBitmaskDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 16);
        message.data.write<uint64_t>(creature->GetStateBitmask());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildStatWoundDelta(Creature* creature, uint8_t subtype, uint16_t index)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_3, 17);
        message.data.write<uint32_t>(1);
        message.data.write<uint32_t>(creature->stat_wound_list_counter_++);
        message.data.write<uint8_t>(subtype);

        switch(subtype)
        {
        case 0:
            message.data.write<uint16_t>(index);
            break;

        case 1:
            message.data.write<uint16_t>(index);
            message.data.write<uint32_t>(creature->GetStatWounds()[index]);
            break;

        case 2:
            message.data.write<uint16_t>(index);
            message.data.write<uint32_t>(creature->GetStatWounds()[index]);
            break;
        };
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildAccelerationMultiplierBaseDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 0);
        message.data.write<float>(creature->GetAccelerationMultiplierBase());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildAccelerationMultiplierModifierDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 1);
        message.data.write<float>(creature->GetAccelerationMultiplierModifier());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildStatEncumberanceDelta(Creature* creature, uint8_t subtype, uint16_t index)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 2);
        message.data.write<uint32_t>(1);
        message.data.write<uint32_t>(creature->stat_encumberance_list_counter_++);
        message.data.write<uint8_t>(subtype);

        switch(subtype)
        {
        case 0:
            message.data.write<uint16_t>(index);
            break;

        case 1:
            message.data.write<uint16_t>(index);
            message.data.write<uint32_t>(creature->GetStatEncumberance((Creature::Stat)index));
            break;

        case 2:
            message.data.write<uint16_t>(index);
            message.data.write<uint32_t>(creature->GetStatEncumberance((Creature::Stat)index));
            break;
        };
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildSkillModDelta(Creature* creature, uint8_t subtype, Creature::SkillMod mod)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 3);
        message.data.write<uint32_t>(1);
        message.data.write<uint32_t>(creature->skill_mod_list_counter_++);
        message.data.write<uint8_t>(subtype);

        switch(subtype)
        {
        case 0:
            message.data.write<std::string>(mod.indenfitier);
            message.data.write<uint32_t>(mod.base);
            message.data.write<uint32_t>(mod.modifier);
            break;

        case 1:
            message.data.write<std::string>(mod.indenfitier);
            message.data.write<uint32_t>(mod.base);
            message.data.write<uint32_t>(mod.modifier);
            break;
        }
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildSpeedMultiplierBaseDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 4);
        message.data.write<float>(creature->GetSpeedMultiplierBase());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildSpeedMultiplierModifierDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 5);
        message.data.write<float>(creature->GetSpeedMultiplierModifier());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildListenToIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 6);
        message.data.write<uint64_t>(creature->GetListenToId());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildRunSpeedDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 7);
        message.data.write<float>(creature->GetRunSpeed());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildSlopeModifierAngleDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 8);
        message.data.write<float>(creature->GetSlopeModifierAngle());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildSlopeModifierPercentDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 9);
        message.data.write<float>(creature->GetSlopeModifierPercent());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildTurnRadiusDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 10);
        message.data.write<float>(creature->GetTurnRadius());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildWalkingSpeedDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 11);
        message.data.write<float>(creature->GetWalkingSpeed());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildWaterModifierPrecentDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 12);
        message.data.write<float>(creature->GetWaterModifierPercent());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildMissionCriticalObjectDelta(Creature* creature, uint8_t subtype, Creature::MissionCriticalObject object)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_4, 13);
        message.data.write<uint32_t>(1);
        message.data.write<uint32_t>(creature->mission_critical_object_list_counter_++);
        message.data.write<uint8_t>(subtype);

        switch(subtype)
        {
        case 0:
            message.data.write<uint64_t>(object.mission_owner_id_);
            message.data.write<uint64_t>(object.critical_object_id_);
            break;

        case 1:
            message.data.write<uint64_t>(object.mission_owner_id_);
            message.data.write<uint64_t>(object.critical_object_id_);
            break;
        };
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildCombatLevelDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 2);
        message.data.write<uint16_t>(creature->GetCombatLevel());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildAnimationDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 3);
        message.data.write<std::string>(creature->GetAnimation());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildMoodAnimationDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 4);
        message.data.write<std::string>(creature->GetMoodAnimation());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildWeaponIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 5);
        message.data.write<uint64_t>(creature->GetWeaponId());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildGroupIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 6);
        message.data.write<uint64_t>(creature->GetGroupId());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildInviteSenderIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 7);
        message.data.write<uint64_t>(creature->invite_counter_++);
        message.data.write<uint64_t>(creature->GetInviteSenderId());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildGuildIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 8);
        message.data.write<uint32_t>(creature->GetGuildId());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildTargetIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 9);
        message.data.write<uint64_t>(creature->GetTargetId());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildMoodIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 10);
        message.data.write<uint8_t>(creature->GetMoodId());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildPerformanceIdDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 12);
        message.data.write<uint32_t>(creature->GetPerformanceId());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildStatCurrentDelta(Creature* creature, uint8_t subtype, uint16_t index)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 13);
        message.data.write<uint32_t>(1);
        message.data.write<uint32_t>(creature->stat_current_list_counter_++);
        message.data.write<uint8_t>(subtype);

        switch(subtype)
        {
        case 0:
            message.data.write<uint16_t>(index);
            break;

        case 1:
            message.data.write<uint16_t>(index);
            message.data.write<uint32_t>(creature->GetStatCurrent((Creature::Stat)index));
            break;

        case 2:
            message.data.write<uint16_t>(index);
            message.data.write<uint32_t>(creature->GetStatCurrent((Creature::Stat)index));
            break;
        }
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildStatMaxDelta(Creature* creature, uint8_t subtype, uint16_t index)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 14);
        message.data.write<uint32_t>(1);
        message.data.write<uint32_t>(creature->stat_max_list_counter_++);
        message.data.write<uint8_t>(subtype);

        switch(subtype)
        {
        case 0:
            message.data.write<uint16_t>(index);
            break;

        case 1:
            message.data.write<uint16_t>(index);
            message.data.write<uint32_t>(creature->GetStatMax((Creature::Stat)index));
            break;

        case 2:
            message.data.write<uint16_t>(index);
            message.data.write<uint32_t>(creature->GetStatMax((Creature::Stat)index));
            break;
        };
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildEquipmentDelta(Creature* creature, uint8_t subtype, Creature::EquipmentItem item)
{

}

void CreatureMessageBuilder::BuildDisguiseDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 16);
        message.data.write<std::string>(creature->GetDisguise());
        creature->AddDeltasUpdate(std::move(message));
    }
}

void CreatureMessageBuilder::BuildStationaryDelta(Creature* creature)
{
    if (creature->HasObservers())
    {
        DeltasMessage message = creature->CreateDeltasMessage(Object::VIEW_6, 17);
        message.data.write<uint8_t>(creature->IsStationary());
        creature->AddDeltasUpdate(std::move(message));
    }
}


boost::optional<swganh::messages::BaselinesMessage> CreatureMessageBuilder::BuildBaseline1(Creature* creature)
{
    auto message = creature->CreateBaselinesMessage(Object::VIEW_1, 4);
    message.data.write<uint32_t>(creature->GetBankCredits());
    message.data.write<uint32_t>(creature->GetCashCredits());

    auto base_stats = creature->GetBaseStats();
    message.data.write<uint32_t>(9);                                            // Base Stat List Size
    message.data.write<uint32_t>(creature->stat_base_list_counter_++);          // Base Stat List Counter
    message.data.write<uint32_t>(base_stats[Creature::Stat::HEALTH]);           // Health Base
    message.data.write<uint32_t>(base_stats[Creature::Stat::STRENGTH]);         // Strength Base
    message.data.write<uint32_t>(base_stats[Creature::Stat::CONSTITUTION]);     // Constitution Base
    message.data.write<uint32_t>(base_stats[Creature::Stat::ACTION]);           // Action Base
    message.data.write<uint32_t>(base_stats[Creature::Stat::QUICKNESS]);        // Quickness Base
    message.data.write<uint32_t>(base_stats[Creature::Stat::STAMINA]);          // Stamina Base
    message.data.write<uint32_t>(base_stats[Creature::Stat::MIND]);             // Mind Base
    message.data.write<uint32_t>(base_stats[Creature::Stat::FOCUS]);            // Focus Base
    message.data.write<uint32_t>(base_stats[Creature::Stat::WILLPOWER]);        // Willpower Base

    auto skills = creature->GetSkills();
    message.data.write<uint32_t>(skills.size());
    message.data.write<uint32_t>(creature->skill_list_counter_);
    std::for_each(skills.begin(), skills.end(), [&message](std::string skill) {
        message.data.write<std::string>(skill);
    });
    return boost::optional<swganh::messages::BaselinesMessage>(std::move(message));
}

boost::optional<swganh::messages::BaselinesMessage> CreatureMessageBuilder::BuildBaseline3(Creature* creature)
{
    auto message = creature->CreateBaselinesMessage(Object::VIEW_3, 18);
    message.data.append(creature->BaseTangible::GetBaseline3().get().data);
    message.data.write<uint8_t>(creature->GetPosture());                    // Posture
    message.data.write<uint8_t>(creature->GetFactionRank());                // Faction Rank
    message.data.write<uint64_t>(creature->GetOwnerId());                   // Owner Id
    message.data.write<float>(creature->GetScale());                        // Scale
    message.data.write<uint32_t>(creature->GetBattleFatigue());             // Battle Fatigue
    message.data.write<uint64_t>(creature->GetStateBitmask());              // States Bitmask

    auto wounds = creature->GetStatWounds();
    message.data.write<uint32_t>(9);                                        // Wound List Size
    message.data.write<uint32_t>(creature->stat_wound_list_counter_++);     // Wound List Counter
    message.data.write<uint32_t>(wounds[Creature::Stat::HEALTH]);           // Health Wounds
    message.data.write<uint32_t>(wounds[Creature::Stat::STRENGTH]);         // Strength Wounds
    message.data.write<uint32_t>(wounds[Creature::Stat::CONSTITUTION]);     // Constitution Wounds
    message.data.write<uint32_t>(wounds[Creature::Stat::ACTION]);           // Action Wounds
    message.data.write<uint32_t>(wounds[Creature::Stat::QUICKNESS]);        // Quickness Wounds
    message.data.write<uint32_t>(wounds[Creature::Stat::STAMINA]);          // Stamina Wounds
    message.data.write<uint32_t>(wounds[Creature::Stat::MIND]);             // Mind Wounds
    message.data.write<uint32_t>(wounds[Creature::Stat::FOCUS]);            // Focus Wounds
    message.data.write<uint32_t>(wounds[Creature::Stat::WILLPOWER]);        // Willpower Wounds

    return boost::optional<swganh::messages::BaselinesMessage>(std::move(message));
}

boost::optional<swganh::messages::BaselinesMessage> CreatureMessageBuilder::BuildBaseline4(Creature* creature)
{
    auto message = creature->CreateBaselinesMessage(Object::VIEW_4, 20);
    message.data.write<float>(creature->GetAccelerationMultiplierBase());
    message.data.write<float>(creature->GetAccelerationMultiplierModifier());

    auto encumberance = creature->GetStatEncumberances();
    message.data.write<uint32_t>(6);                                                // Encumberance HAM List Size
    message.data.write<uint32_t>(creature->stat_encumberance_list_counter_++);      // Encumberance HAM List Counter
    message.data.write<uint32_t>(encumberance[Creature::Stat::STRENGTH]);           // Strength Encumberance
    message.data.write<uint32_t>(encumberance[Creature::Stat::CONSTITUTION]);       // Constitution Encumberance
    message.data.write<uint32_t>(encumberance[Creature::Stat::QUICKNESS]);          // Quickness Encumberance
    message.data.write<uint32_t>(encumberance[Creature::Stat::STAMINA]);            // Stamina Encumberance
    message.data.write<uint32_t>(encumberance[Creature::Stat::FOCUS]);              // Focus Encumberance
    message.data.write<uint32_t>(encumberance[Creature::Stat::WILLPOWER]);          // Willpower Encumberance

    auto skill_mods = creature->GetSkillMods();
    message.data.write<uint32_t>(skill_mods.size());                                // Skill Mod List Size
    message.data.write<uint32_t>(creature->skill_mod_list_counter_++);              // Skill Mod List Counter
    std::for_each(skill_mods.begin(), skill_mods.end(), [=, &message](Creature::SkillMod mod) {
        message.data.write<uint8_t>(0);                                             // Skill Mod Item: Unused (0)
        message.data.write<std::string>(mod.indenfitier);                           // Skill Mod Item: Identifier
        message.data.write<uint32_t>(mod.base);                                     // Skill Mod Item: Base
        message.data.write<uint32_t>(mod.modifier);                                 // Skill Mod Item: Modifier
    });

    message.data.write<float>(creature->GetSpeedMultiplierBase());
    message.data.write<float>(creature->GetSpeedMultiplierModifier());
    message.data.write<uint64_t>(creature->GetListenToId());
    message.data.write<float>(creature->GetRunSpeed());
    message.data.write<float>(creature->GetSlopeModifierAngle());
    message.data.write<float>(creature->GetSlopeModifierPercent());
    message.data.write<float>(creature->GetTurnRadius());
    message.data.write<float>(creature->GetWalkingSpeed());
    message.data.write<float>(creature->GetWaterModifierPercent());

    auto mission_critical_objects = creature->GetMissionCriticalObjects();
    message.data.write<uint32_t>(mission_critical_objects.size());                              // Critical Group Mission List Size
    message.data.write<uint32_t>(creature->mission_critical_object_list_counter_++);            // Critical Group Mission List Counter
    std::for_each(mission_critical_objects.begin(), mission_critical_objects.end(), [=, &message](Creature::MissionCriticalObject object) {
        message.data.write<uint64_t>(object.mission_owner_id_);                                 // Critical Group Mission Item: Mission Owner Id
        message.data.write<uint64_t>(object.critical_object_id_);                               // Critical Group Mission Item: Critical Object Id
    });

    return boost::optional<swganh::messages::BaselinesMessage>(std::move(message));
}

boost::optional<swganh::messages::BaselinesMessage> CreatureMessageBuilder::BuildBaseline6(Creature* creature)
{
    auto message = creature->CreateBaselinesMessage(Object::VIEW_6, 23);
    message.data.append(creature->BaseTangible::GetBaseline6().get().data);
    message.data.write<uint16_t>(creature->GetCombatLevel());                   // Combat Level
    message.data.write<std::string>(creature->GetAnimation());                  // Current Animation
    message.data.write<std::string>(creature->GetMoodAnimation());              // Mood Animation
    message.data.write<uint64_t>(creature->GetWeaponId());                      // Weapon Id
    message.data.write<uint64_t>(creature->GetGroupId());                       // Group Id
    message.data.write<uint64_t>(creature->GetInviteSenderId());                // Invite Sender Id
    message.data.write<uint64_t>(creature->invite_counter_);                    // Invite Sender Counter
    message.data.write<uint32_t>(creature->GetGuildId());                       // Guild Id
    message.data.write<uint64_t>(creature->GetTargetId());                      // Target Id
    message.data.write<uint8_t>(creature->GetMoodId());                         // Mood Id
    message.data.write<uint32_t>(creature->performance_counter_++);             // Performance Update Counter
    message.data.write<uint32_t>(creature->GetPerformanceId());                 // Performance Id

    auto current_stats = creature->GetCurrentStats();
    message.data.write<uint32_t>(9);                                                // Current HAM List Size
    message.data.write<uint32_t>(creature->stat_current_list_counter_++);           // Current HAM List Counter
    message.data.write<uint32_t>(current_stats[Creature::Stat::HEALTH]);            // Health Current
    message.data.write<uint32_t>(current_stats[Creature::Stat::STRENGTH]);          // Strength Current
    message.data.write<uint32_t>(current_stats[Creature::Stat::CONSTITUTION]);      // Constitution Current
    message.data.write<uint32_t>(current_stats[Creature::Stat::ACTION]);            // Action Current
    message.data.write<uint32_t>(current_stats[Creature::Stat::QUICKNESS]);         // Quickness Current
    message.data.write<uint32_t>(current_stats[Creature::Stat::STAMINA]);           // Stamina Current
    message.data.write<uint32_t>(current_stats[Creature::Stat::MIND]);              // Mind Current
    message.data.write<uint32_t>(current_stats[Creature::Stat::FOCUS]);             // Focus Current
    message.data.write<uint32_t>(current_stats[Creature::Stat::WILLPOWER]);         // Willpower Current    

    auto max_stats = creature->GetMaxStats();
    message.data.write<uint32_t>(9);                                                // Max HAM List Size
    message.data.write<uint32_t>(creature->stat_max_list_counter_++);               // Max HAM List Counter
    message.data.write<uint32_t>(max_stats[Creature::Stat::HEALTH]);                // Health Max
    message.data.write<uint32_t>(max_stats[Creature::Stat::STRENGTH]);              // Strength Max
    message.data.write<uint32_t>(max_stats[Creature::Stat::CONSTITUTION]);          // Constitution Max
    message.data.write<uint32_t>(max_stats[Creature::Stat::ACTION]);                // Action Max
    message.data.write<uint32_t>(max_stats[Creature::Stat::QUICKNESS]);             // Quickness Max
    message.data.write<uint32_t>(max_stats[Creature::Stat::STAMINA]);               // Stamina Max
    message.data.write<uint32_t>(max_stats[Creature::Stat::MIND]);                  // Mind Max
    message.data.write<uint32_t>(max_stats[Creature::Stat::FOCUS]);                 // Focus Max
    message.data.write<uint32_t>(max_stats[Creature::Stat::WILLPOWER]);             // Willpower Max    

    auto equipment = creature->GetEquipment();
    message.data.write<uint32_t>(equipment.size());                             // Equipment List Size
    message.data.write<uint32_t>(creature->equipment_list_counter_++);          // Equipment List Counter
    std::for_each(equipment.begin(), equipment.end(), [=, &message](Creature::EquipmentItem item){
        message.data.write<std::string>(item.customization);                    // Equipment Item: Customization String
        message.data.write<uint32_t>(item.containment_type);                    // Equipment Item: Containment Type
        message.data.write<uint64_t>(item.object_id);                           // Equipment Item: Object Id
        message.data.write<uint32_t>(item.template_crc);                        // Equipment Item: Template CRC
    });
    message.data.write<std::string>(creature->GetDisguise());                   // Disguise Template
    message.data.write<uint8_t>(creature->IsStationary());                     // Stationary
    return boost::optional<swganh::messages::BaselinesMessage>(std::move(message));
}