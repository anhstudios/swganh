
#include "swganh/object/creature/creature.h"
#include "swganh/object/creature/creature_message_builder.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object::creature;

Creature::Creature()
: Object()
{}

Creature::Creature(uint32_t bank_credits, uint32_t cash_credits, std::list<std::string> skill_list)
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

boost::optional<BaselinesMessage> Creature::GetBaseline3()
{
    return move(CreatureMessageBuilder::BuildBaseline3(this));
}