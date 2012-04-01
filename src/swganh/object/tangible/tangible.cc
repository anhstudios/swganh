
#include "tangible.h"

#include "swganh/messages/deltas_message.h"
#include "swganh/object/tangible/tangible_message_builder.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::tangible;
using namespace swganh::messages;
using namespace swganh::messages::containers;

Tangible::Tangible()
    : Object()
    , customization_("")
    , component_customization_list_(NetworkList<ComponentCustomization>())
    , options_bitmask_(0)
    , incap_timer_(0)
    , condition_damage_(0)
    , max_condition_(0)
    , is_static_(false)
    , defender_list_(swganh::messages::containers::NetworkSortedVector<Defender>(3))
{}

Tangible::Tangible(const std::string& customization, std::vector<uint32_t> component_customization, uint32_t bitmask_options,
        uint32_t incap_timer, uint32_t condition_damage, uint32_t max_condition, bool is_static, std::vector<uint64_t> defenders)
    : Object()
    , customization_(customization)
    , component_customization_list_(NetworkList<ComponentCustomization>())
    , options_bitmask_(bitmask_options)
    , incap_timer_(incap_timer)
    , condition_damage_(condition_damage)
    , max_condition_(max_condition)
    , is_static_(is_static)
    , defender_list_(swganh::messages::containers::NetworkSortedVector<Defender>(3))
{
    std::for_each(defenders.begin(), defenders.end(), [=](const uint64_t& def) {
        defender_list_.Add(Defender(def));
    });

    std::for_each(component_customization.begin(), component_customization.end(), [=](const uint32_t& crc) {
        component_customization_list_.Add(ComponentCustomization(crc));
    });
}

void Tangible::AddCustomization(const string& customization)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    customization_.append(customization);
    TangibleMessageBuilder::BuildCustomizationDelta(this);
}

std::string Tangible::GetCustomization(void)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return customization_;
}

void Tangible::SetCustomization(const string& customization)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    customization_ = customization;
    TangibleMessageBuilder::BuildCustomizationDelta(this);
}
void Tangible::RemoveComponentCustomization(uint32_t customization)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    auto iter = std::find_if(component_customization_list_.Begin(), component_customization_list_.End(), [=](ComponentCustomization component) {
        return component.component_customization_crc = customization;
    });

    if(iter != component_customization_list_.End())
    {
        component_customization_list_.Remove(iter);
        TangibleMessageBuilder::BuildComponentCustomizationDelta(this);
    }
}
void Tangible::AddComponentCustomization(uint32_t customization)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    component_customization_list_.Add(ComponentCustomization(customization));
    TangibleMessageBuilder::BuildComponentCustomizationDelta(this);
}

NetworkList<ComponentCustomization> Tangible::GetComponentCustomization(void)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return component_customization_list_;
}

void Tangible::ClearComponentCustomization()
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    component_customization_list_.Clear();
    TangibleMessageBuilder::BuildComponentCustomizationDelta(this);
}

void Tangible::SetOptionsMask(uint32_t options_mask)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    options_bitmask_ = options_mask;
    TangibleMessageBuilder::BuildOptionsMaskDelta(this);    
}

void Tangible::ToggleOption(uint32_t option)
{
	std::lock_guard<std::recursive_mutex> lock(mutex_);
	options_bitmask_ ^= option;
	TangibleMessageBuilder::BuildOptionsMaskDelta(this);
}

uint32_t Tangible::GetOptionsMask(void)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return options_bitmask_;
}

void Tangible::SetIncapTimer(uint32_t incap_timer)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    incap_timer_ = incap_timer;
    TangibleMessageBuilder::BuildIncapTimerDelta(this);
}

uint32_t Tangible::GetIncapTimer(void)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return incap_timer_;
}

void Tangible::SetConditionDamage(uint32_t damage)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    condition_damage_ = damage;
    TangibleMessageBuilder::BuildConditionDamageDelta(this);
}

uint32_t Tangible::GetCondition(void)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return condition_damage_;
}

void Tangible::SetMaxCondition(uint32_t max_condition)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    max_condition_ = max_condition;
    TangibleMessageBuilder::BuildMaxConditionDelta(this);
}

uint32_t Tangible::GetMaxCondition(void)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return max_condition_;
}

void Tangible::SetStatic(bool is_static)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    is_static_ = is_static;
    TangibleMessageBuilder::BuildStaticDelta(this);
}

bool Tangible::IsStatic(void)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return is_static_;
}

bool Tangible::IsDefending(uint64_t defender)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    auto iter = std::find_if(defender_list_.Begin(), defender_list_.End(), [=](const Defender& x)->bool {
        return (x.object_id == defender);
    });

    if(iter != defender_list_.End())
        return true;
    else
        return false;
}
void Tangible::AddDefender(uint64_t defender)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    defender_list_.Add(Defender(defender));
    TangibleMessageBuilder::BuildDefendersDelta(this);
}
void Tangible::RemoveDefender(uint64_t defender)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    auto iter = std::find_if(defender_list_.Begin(), defender_list_.End(), [=](const Defender& x)->bool {
        return (x.object_id == defender);
    });

    if(iter != defender_list_.End())
    {
        defender_list_.Remove(iter);
        TangibleMessageBuilder::BuildDefendersDelta(this);
    }
}
void Tangible::ResetDefenders(std::vector<uint64_t> defenders)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    defender_list_.Clear();
    std::for_each(defenders.begin(), defenders.end(), [=](const uint64_t& x) {
        defender_list_.Insert(Defender(x));
    });
    defender_list_.Reinstall();
    TangibleMessageBuilder::BuildDefendersDelta(this);
}

NetworkSortedVector<Defender> Tangible::GetDefenders()
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return defender_list_;
}

void Tangible::ClearDefenders()
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    defender_list_.Clear();
    //@TODO: NOT SURE WHY CLEAR DOESN'T WIPE OUT THE LIST
    /*auto iter = defender_list_.Begin();
    while (iter != defender_list_.End())
    {
        defender_list_.Remove(iter);
        iter++;
    }*/
    TangibleMessageBuilder::BuildDefendersDelta(this);
}

void Tangible::ActivateAutoAttack()
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    auto_attack_ = true;
}
void Tangible::ClearAutoAttack()
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    auto_attack_ = false;
}
bool Tangible::IsAutoAttacking()
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return auto_attack_ == true;
}

boost::optional<BaselinesMessage> Tangible::GetBaseline3()
{
    return std::move(TangibleMessageBuilder::BuildBaseline3(this));
}
boost::optional<BaselinesMessage> Tangible::GetBaseline6()
{
    return std::move(TangibleMessageBuilder::BuildBaseline6(this));
}
boost::optional<BaselinesMessage> Tangible::GetBaseline7()
{
    return std::move(TangibleMessageBuilder::BuildBaseline7(this));
}