
#include "swganh/object/creature/creature.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object::creature;

Creature::Creature()
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
{}

void Creature::SetCashCredits(uint32_t cash_credits)
{}

void Creature::AddSkill(std::string skill)
{}

void Creature::RemoveSkill(std::string skill)
{}