// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "tangible.h"

#include "swganh_core/object/object_events.h"
using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::messages;
using namespace swganh::messages::containers;

Tangible::Tangible()
    : Object()
    , customization_("")
    , component_customization_list_(NetworkList<ComponentCustomization>())
    , options_bitmask_(0)
    , counter_(0)
    , condition_damage_(0)
    , max_condition_(1000)
    , is_static_(false)
    , defender_list_(swganh::messages::containers::NetworkSortedVector<Defender>(3))
{}

Tangible::Tangible(const std::string& customization, std::vector<uint32_t> component_customization, uint32_t bitmask_options,
        uint32_t counter, uint32_t condition_damage, uint32_t max_condition, bool is_static, std::vector<uint64_t> defenders)
    : Object()
    , customization_(customization)
    , component_customization_list_(NetworkList<ComponentCustomization>())
    , options_bitmask_(bitmask_options)
    , counter_(counter)
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

void Tangible::AddCustomization(const std::string& customization) { AddCustomization(customization, AcquireLock()); }
void Tangible::AddCustomization(const std::string& customization, boost::unique_lock<boost::mutex>& lock)
{
    customization_.append(customization);
	DISPATCH(Tangible, Customization);
}

std::string Tangible::GetCustomization() { return GetCustomization(AcquireLock()); }
std::string Tangible::GetCustomization(boost::unique_lock<boost::mutex>& lock)
{
    return customization_;
}

void Tangible::SetCustomization(const std::string& customization) { SetCustomization(customization, AcquireLock()); }
void Tangible::SetCustomization(const std::string& customization, boost::unique_lock<boost::mutex>& lock)
{
	customization_ = customization;
    DISPATCH(Tangible, Customization);
}

void Tangible::SetCustomizationFromInts(std::vector<int> customization_ints) { SetCustomizationFromInts(customization_ints, AcquireLock()); }
void Tangible::SetCustomizationFromInts(std::vector<int> customization_ints, boost::unique_lock<boost::mutex>& lock)
{
	{
		auto lock = AcquireLock();
		customization_ = std::string(customization_ints.begin(), customization_ints.end());
	}
	DISPATCH(Tangible, Customization);
}

void Tangible::RemoveComponentCustomization(uint32_t customization) { RemoveComponentCustomization(customization, AcquireLock()); }
void Tangible::RemoveComponentCustomization(uint32_t customization, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(component_customization_list_), end(component_customization_list_), [=](ComponentCustomization component) {
        return component.component_customization_crc = customization;
    });

    if(iter == end(component_customization_list_))
    {
        return;
    }
            
    component_customization_list_.Remove(iter);
    DISPATCH(Tangible, ComponentCustomization);
}

void Tangible::AddComponentCustomization(uint32_t customization) { AddComponentCustomization(customization, AcquireLock()); }
void Tangible::AddComponentCustomization(uint32_t customization, boost::unique_lock<boost::mutex>& lock)
{
	component_customization_list_.Add(ComponentCustomization(customization));
    DISPATCH(Tangible, ComponentCustomization);
}

std::list<ComponentCustomization> Tangible::GetComponentCustomization() { return GetComponentCustomization(AcquireLock()); }
std::list<ComponentCustomization> Tangible::GetComponentCustomization(boost::unique_lock<boost::mutex>& lock)
{
    return std::move(component_customization_list_.Get());
}

void Tangible::ClearComponentCustomization() { ClearComponentCustomization(AcquireLock()); }
void Tangible::ClearComponentCustomization(boost::unique_lock<boost::mutex>& lock)
{
	component_customization_list_.Clear();
    DISPATCH(Tangible, ComponentCustomization);
}

void Tangible::SetOptionsMask(uint32_t options_mask) { SetOptionsMask(options_mask, AcquireLock()); }
void Tangible::SetOptionsMask(uint32_t options_mask, boost::unique_lock<boost::mutex>& lock)
{
    options_bitmask_ = options_mask;
	DISPATCH(Tangible, OptionsMask);
}

void Tangible::ToggleOption(uint32_t option) { ToggleOption(option, AcquireLock()); }
void Tangible::ToggleOption(uint32_t option, boost::unique_lock<boost::mutex>& lock)
{
	options_bitmask_ ^= option;
	DISPATCH(Tangible, OptionsMask);
}

uint32_t Tangible::GetOptionsMask() { return GetOptionsMask(AcquireLock()); }
uint32_t Tangible::GetOptionsMask(boost::unique_lock<boost::mutex>& lock)
{
    return options_bitmask_;
}

void Tangible::SetCounter(uint32_t counter) { SetCounter(counter, AcquireLock()); }
void Tangible::SetCounter(uint32_t counter, boost::unique_lock<boost::mutex>& lock)
{
    counter_ = counter;
    DISPATCH(Tangible, Counter);
}

uint32_t Tangible::GetCounter() { return GetCounter(AcquireLock()); }
uint32_t Tangible::GetCounter(boost::unique_lock<boost::mutex>& lock)
{
    return counter_;
}

void Tangible::SetConditionDamage(uint32_t damage) { SetConditionDamage(damage, AcquireLock()); }
void Tangible::SetConditionDamage(uint32_t damage, boost::unique_lock<boost::mutex>& lock)
{
    condition_damage_ = damage;
	DISPATCH(Tangible, ConditionDamage);
	DISPATCH(Tangible, UpdateAttribute);
}

uint32_t Tangible::GetCondition() { return GetCondition(AcquireLock()); }
uint32_t Tangible::GetCondition(boost::unique_lock<boost::mutex>& lock)
{
    return condition_damage_;
}

void Tangible::SetMaxCondition(uint32_t max_condition) { SetMaxCondition(max_condition, AcquireLock()); }
void Tangible::SetMaxCondition(uint32_t max_condition, boost::unique_lock<boost::mutex>& lock)
{
    max_condition_ = max_condition;
    DISPATCH(Tangible, MaxCondition);
	DISPATCH(Tangible, UpdateAttribute);
}

uint32_t Tangible::GetMaxCondition() { return GetMaxCondition(AcquireLock()); }
uint32_t Tangible::GetMaxCondition(boost::unique_lock<boost::mutex>& lock)
{
    return max_condition_;
}

void Tangible::SetStatic(bool is_static) { SetStatic(is_static, AcquireLock()); }
void Tangible::SetStatic(bool is_static, boost::unique_lock<boost::mutex>& lock)
{
    is_static_ = is_static;
	DISPATCH(Tangible, Static);
}

bool Tangible::IsStatic() { return IsStatic(AcquireLock()); }
bool Tangible::IsStatic(boost::unique_lock<boost::mutex>& lock)
{
    return is_static_;
}

bool Tangible::IsDefending(uint64_t defender) { return IsDefending(defender, AcquireLock()); }
bool Tangible::IsDefending(uint64_t defender, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(defender_list_), end(defender_list_), [=](const Defender& x)->bool {
        return (x.object_id == defender);
    });

    if(iter != end(defender_list_))
	{
        return true;
	}
    else
	{
        return false;
	}
}

void Tangible::AddDefender(uint64_t defender) { AddDefender(defender, AcquireLock()); }
void Tangible::AddDefender(uint64_t defender, boost::unique_lock<boost::mutex>& lock)
{
	defender_list_.Add(Defender(defender));
	DISPATCH(Tangible, Defenders);
}

void Tangible::RemoveDefender(uint64_t defender) { RemoveDefender(defender, AcquireLock()); }
void Tangible::RemoveDefender(uint64_t defender, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(defender_list_), end(defender_list_), [=](const Defender& x)->bool {
        return (x.object_id == defender);
    });

    if(iter == end(defender_list_))
    {
        return;
    }
            
    defender_list_.Remove(iter);
    DISPATCH(Tangible, Defenders);
}

void Tangible::ResetDefenders(std::vector<uint64_t> defenders) { ResetDefenders(defenders, AcquireLock()); }
void Tangible::ResetDefenders(std::vector<uint64_t> defenders, boost::unique_lock<boost::mutex>& lock)
{
    defender_list_.Clear();
    std::for_each(defenders.begin(), defenders.end(), [=](const uint64_t& x) {
        defender_list_.Insert(Defender(x));
    });
    defender_list_.Reinstall();
    DISPATCH(Tangible, Defenders);
}

std::vector<Defender> Tangible::GetDefenders() { return GetDefenders(AcquireLock()); }
std::vector<Defender> Tangible::GetDefenders(boost::unique_lock<boost::mutex>& lock)
{
    return std::move(defender_list_.Get());
}

void Tangible::ClearDefenders() { ClearDefenders(AcquireLock()); }
void Tangible::ClearDefenders(boost::unique_lock<boost::mutex>& lock)
{
    defender_list_.Clear();
    DISPATCH(Tangible, Defenders);
}

void Tangible::ActivateAutoAttack() { ActivateAutoAttack(AcquireLock()); }
void Tangible::ActivateAutoAttack(boost::unique_lock<boost::mutex>& lock)
{
    auto_attack_ = true;
}

void Tangible::ClearAutoAttack() { ClearAutoAttack(AcquireLock()); }
void Tangible::ClearAutoAttack(boost::unique_lock<boost::mutex>& lock)
{
    auto_attack_ = false;
}

bool Tangible::IsAutoAttacking() { return IsAutoAttacking(AcquireLock()); }
bool Tangible::IsAutoAttacking(boost::unique_lock<boost::mutex>& lock)
{
    return auto_attack_ == true;
}

void Tangible::CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
    if (auto dispatch = GetEventDispatcher())
	{
		dispatch->Dispatch(make_shared<ObserverEvent>
			("Tangible::Baselines", shared_from_this(), observer));
	}
}

void Tangible::SerializeComponentCustomization(swganh::messages::BaseSwgMessage* message) { SerializeComponentCustomization(message, AcquireLock()); }
void Tangible::SerializeComponentCustomization(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	component_customization_list_.Serialize(message);
}

void Tangible::SerializeDefenders(swganh::messages::BaseSwgMessage* message) { SerializeDefenders(message, AcquireLock()); }
void Tangible::SerializeDefenders(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	defender_list_.Serialize(message);
}