
#include "swganh/object/creature/creature.h"
#include "swganh/object/creature/creature_message_builder.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object::creature;

Creature::Creature()
: BaseTangible()
, bank_credits_(0)
, cash_credits_(0)
, skill_list_(std::list<std::string>())
, posture_(0)
, faction_rank_(0)
, owner_id_(0)
, scale_(1.0f)
, battle_fatigue_(0)
, state_bitmask_(0)
, acceleration_multiplier_base_(1.0f)
, acceleration_multiplier_modifier_(1.0f)
, speed_multiplier_base_(1.0f)
, speed_multiplier_modifier_(1.0f)
, listen_to_id_(0)
, run_speed_(5.0f)
, slope_modifier_angle_(1.02f)
, slope_modifier_percent_(0)
, turn_radius_(1.0f)
, walking_speed_(1.0f)
, water_modifier_percent_(0.0125f)
, combat_level_(0)
, animation_("")
, mood_animation_("")
, weapon_id_(0)
, group_id_(0)
, invite_sender_id_(0)
, invite_counter_(0)
, guild_id_(0)
, target_id_(0)
, mood_id_(0)
, performance_counter_(0)
, performance_id_(0)
, stationary_(0)
{}

Creature::~Creature()
{}

uint32_t Creature::GetType() const 
{ 
    return Creature::type;
}

void Creature::SetBankCredits(uint32_t bank_credits)
{
    bank_credits_ = bank_credits;
    CreatureMessageBuilder::BuildBankCreditsDelta(this);
}

void Creature::SetCashCredits(uint32_t cash_credits)
{
    cash_credits = cash_credits;
    CreatureMessageBuilder::BuildCashCreditsDelta(this);
}

void Creature::AddSkill(std::string skill)
{
    if(std::find(skill_list_.begin(), skill_list_.end(), skill) == skill_list_.end())
    {
        skill_list_.push_back(skill);
        CreatureMessageBuilder::BuildSkillListDelta(this, 1, skill);
    }
}

void Creature::RemoveSkill(std::string skill)
{
    if(std::find(skill_list_.begin(), skill_list_.end(), skill) != skill_list_.end())
    {
        skill_list_.remove(skill);
        CreatureMessageBuilder::BuildSkillListDelta(this, 0, skill);
    }
}

void Creature::SetPosture(POSTURE posture)
{
    posture_ = posture;
    CreatureMessageBuilder::BuildPostureDelta(this);
}

void Creature::SetFactionRank(uint8_t faction_rank)
{
    faction_rank_ = faction_rank;
    CreatureMessageBuilder::BuildFactionRankDelta(this);
}

void Creature::SetOwnerId(uint64_t owner_id)
{
    owner_id_ = owner_id;
    CreatureMessageBuilder::BuildOwnerIdDelta(this);
}

void Creature::SetScale(float scale)
{
    scale_ = scale;
    CreatureMessageBuilder::BuildScaleDelta(this);
}

void Creature::SetBattleFatigue(uint32_t battle_fatigue)
{
    battle_fatigue_ = battle_fatigue;
    CreatureMessageBuilder::BuildBattleFatigueDelta(this);
}

void Creature::SetStateBitmask(uint64_t state_bitmask)
{
    state_bitmask_ = state_bitmask;
    CreatureMessageBuilder::BuildStateBitmaskDelta(this);
}

boost::optional<BaselinesMessage> Creature::GetBaseline1()
{
    return move(CreatureMessageBuilder::BuildBaseline1(this));
}

boost::optional<BaselinesMessage> Creature::GetBaseline3()
{
    return move(CreatureMessageBuilder::BuildBaseline3(this));
}

boost::optional<BaselinesMessage> Creature::GetBaseline4()
{
    return move(CreatureMessageBuilder::BuildBaseline4(this));
}

boost::optional<BaselinesMessage> Creature::GetBaseline6()
{
    return move(CreatureMessageBuilder::BuildBaseline6(this));
}