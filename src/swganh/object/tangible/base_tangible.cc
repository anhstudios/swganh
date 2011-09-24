
#include "swganh/object/tangible/base_tangible.h"

#include "swganh/messages/deltas_message.h"
#include "swganh/object/tangible/tangible_message_builder.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::tangible;
using namespace swganh::messages;

BaseTangible::BaseTangible()
    : customization_(0)
    , component_customization_list_(std::vector<uint32_t>())
    , component_customization_counter_(0)
    , options_bitmask_(0)
    , incap_timer_(0)
    , condition_damage_(0)
    , max_condition_(0)
    , is_static_(false)
    , defender_list_(std::vector<uint64_t>())
    , defender_list_counter_(0)
{}
BaseTangible::BaseTangible(const std::string& customization, std::vector<uint32_t> component_customization, uint32_t bitmask_options,
        uint32_t incap_timer, uint32_t condition_damage, uint32_t max_condition, bool is_static, std::vector<uint64_t> defenders)
    : customization_(customization)
    , component_customization_list_(component_customization)
    , options_bitmask_(bitmask_options)
    , incap_timer_(incap_timer)
    , condition_damage_(condition_damage)
    , max_condition_(max_condition)
    , is_static_(is_static)
    , defender_list_(defenders)
{
}
void BaseTangible::AddCustomization(const string& customization)
{
    customization_.append(customization);
    TangibleMessageBuilder::BuildCustomizationDelta(this);
}

void BaseTangible::SetCustomization(const string& customization)
{
    customization_ = customization;
    TangibleMessageBuilder::BuildCustomizationDelta(this);
}
void BaseTangible::RemoveComponentCustomization(uint32_t customization)
{
    auto found = find_if(
        begin(component_customization_list_), end(component_customization_list_),
        [&customization](uint32_t crc) 
    {
        return customization == crc;
    });
    // not found
    if (found == end(component_customization_list_))
        return;

    component_customization_list_.erase(found);
    TangibleMessageBuilder::BuildComponentCustomizationDelta(this, 0, customization);
}
void BaseTangible::AddComponentCustomization(uint32_t customization)
{
    // add customization
    component_customization_list_.push_back(customization);
    TangibleMessageBuilder::BuildComponentCustomizationDelta(this, 1, customization);
}

void BaseTangible::ClearComponentCustomization()
{
    component_customization_list_.clear();
    TangibleMessageBuilder::BuildComponentCustomizationDelta(this, 2, 0);
}

void BaseTangible::SetOptionsMask(uint32_t options_mask)
{
    options_bitmask_ = options_mask;
    TangibleMessageBuilder::BuildOptionsMaskDelta(this);    
}
void BaseTangible::SetIncapTimer(uint32_t incap_timer)
{
    incap_timer_ = incap_timer;
    TangibleMessageBuilder::BuildIncapTimerDelta(this);
}
void BaseTangible::SetConditionDamage(uint32_t damage)
{
    condition_damage_ = damage;
    TangibleMessageBuilder::BuildConditionDamageDelta(this);
}
void BaseTangible::SetMaxCondition(uint32_t max_condition)
{
    max_condition_ = max_condition;
    TangibleMessageBuilder::BuildMaxConditionDelta(this);
}
void BaseTangible::SetStatic(bool is_static)
{
    is_static_ = is_static;
    TangibleMessageBuilder::BuildStaticDelta(this);
}
std::vector<uint64_t>::iterator BaseTangible::FindDefender(uint64_t defender)
{
    auto found = find_if(begin(defender_list_), end(defender_list_), [defender](uint64_t defender_check){
        return defender == defender_check;
    });

    return found;
}
void BaseTangible::AddDefender(uint64_t defender)
{
    
    // bail if found
    if (FindDefender(defender) != end(defender_list_))
        return;

    // make sure there's not the same defender already
    defender_list_.push_back(defender);
    // update counter
    IncrementDefendersCounter();
    TangibleMessageBuilder::BuildDefendersDelta(this, 1, defender);
}
void BaseTangible::RemoveDefender(uint64_t defender)
{
    // bail if not found
    auto found = FindDefender(defender);
    if (found == end(defender_list_))
        return;

    defender_list_.erase(found);
    IncrementDefendersCounter();
    TangibleMessageBuilder::BuildDefendersDelta(this, 0, defender);
}
void BaseTangible::ResetDefenders(std::vector<uint64_t> defenders)
{
    defender_list_ = defenders;
    // update counter
    ClearDefendersCounter();
    TangibleMessageBuilder::BuildNewDefendersDelta(this);
}
void BaseTangible::ClearDefenders()
{
    defender_list_.clear();
    ClearDefendersCounter();
    TangibleMessageBuilder::BuildDefendersDelta(this, 4, 0);
}