
#include "swganh/object/tangible/base_tangible.h"

#include "swganh/messages/deltas_message.h"
#include "swganh/object/tangible/tangible_message_builder.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::tangible;
using namespace swganh::messages;

BaseTangible::BaseTangible()
    : Object()
    , customization_("")
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
    : Object()
    , customization_(customization)
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
std::vector<uint64_t>::iterator BaseTangible::FindDefender_(uint64_t defender)
{
    auto found = find_if(begin(defender_list_), end(defender_list_), [defender](uint64_t defender_check){
        return defender == defender_check;
    });

    return found;
}
bool BaseTangible::IsDefending(uint64_t defender)
{
    return FindDefender_(defender) != end(defender_list_);
}
void BaseTangible::AddDefender(uint64_t defender)
{
    uint16_t next_index = GetNextAvailableSlot(defender_list_, defender_index_free_list_);
    defender_list_[next_index] = defender;
    TangibleMessageBuilder::BuildDefendersDelta(this, 1, next_index, defender);
}
void BaseTangible::RemoveDefender(uint64_t defender)
{
    // bail if not found
    auto found = FindDefender_(defender);
    if (found == end(defender_list_))
        return;
    uint16_t index_position = found - begin(defender_list_);
    defender_list_.erase(found);
    TangibleMessageBuilder::BuildDefendersDelta(this, 0, index_position, defender);
    defender_index_free_list_.push_back(index_position);
}
void BaseTangible::ResetDefenders(std::vector<uint64_t> defenders)
{
    defender_list_ = defenders;
    TangibleMessageBuilder::BuildNewDefendersDelta(this);
    defender_index_free_list_.clear();
}
void BaseTangible::ClearDefenders()
{
    defender_list_.clear();
    TangibleMessageBuilder::BuildDefendersDelta(this, 4, 0, 0);
    defender_index_free_list_.clear();
}

boost::optional<BaselinesMessage> BaseTangible::GetBaseline3()
{
    return move(TangibleMessageBuilder::BuildBaseline3(this));
}
boost::optional<BaselinesMessage> BaseTangible::GetBaseline6()
{
    return move(TangibleMessageBuilder::BuildBaseline6(this));
}
boost::optional<BaselinesMessage> BaseTangible::GetBaseline7()
{
    return move(TangibleMessageBuilder::BuildBaseline7(this));
}