// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "tangible.h"

#include "swganh_core/object/object_events.h"
using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::messages;
using namespace swganh::containers;

Tangible::Tangible()
    : Object()
    , customization_("")
    , options_bitmask_(0)
    , counter_(0)
    , condition_damage_(0)
    , max_condition_(1000)
    , is_static_(false)
{}

Tangible::Tangible(const std::string& customization, std::vector<uint32_t> component_customization, uint32_t bitmask_options,
        uint32_t counter, uint32_t condition_damage, uint32_t max_condition, bool is_static, std::vector<uint64_t> defenders)
    : Object()
    , customization_(customization)
    , options_bitmask_(bitmask_options)
    , counter_(counter)
    , condition_damage_(condition_damage)
    , max_condition_(max_condition)
    , is_static_(is_static)
{
    std::for_each(defenders.begin(), defenders.end(), [=](const uint64_t& def) {
        defender_list_.add(def);
    });

    std::for_each(component_customization.begin(), component_customization.end(), [=](const uint32_t& crc) {
        component_customization_list_.add(crc);
    });
}

void Tangible::AddCustomization(const std::string& customization) {
    auto lock = AcquireLock();
    AddCustomization(customization, lock);
}

void Tangible::AddCustomization(const std::string& customization, boost::unique_lock<boost::mutex>& lock)
{
    customization_.append(customization);
	DISPATCH(Tangible, Customization);
}

std::string Tangible::GetCustomization() {
    auto lock = AcquireLock();
    return GetCustomization(lock);
}

std::string Tangible::GetCustomization(boost::unique_lock<boost::mutex>& lock)
{
    return customization_;
}

void Tangible::SetCustomization(const std::string& customization) {
    auto lock = AcquireLock();
    SetCustomization(customization, lock);
}

void Tangible::SetCustomization(const std::string& customization, boost::unique_lock<boost::mutex>& lock)
{
	customization_ = customization;
    DISPATCH(Tangible, Customization);
}

void Tangible::SetCustomizationFromInts(std::vector<int> customization_ints) {
    auto lock = AcquireLock();
    SetCustomizationFromInts(customization_ints, lock);
}

void Tangible::SetCustomizationFromInts(std::vector<int> customization_ints, boost::unique_lock<boost::mutex>& lock)
{
	customization_ = std::string(customization_ints.begin(), customization_ints.end());
	DISPATCH(Tangible, Customization);
}

void Tangible::RemoveComponentCustomization(uint32_t customization) {
    auto lock = AcquireLock();
    RemoveComponentCustomization(customization, lock);
}

void Tangible::RemoveComponentCustomization(uint32_t customization, boost::unique_lock<boost::mutex>& lock)
{
    component_customization_list_.remove(customization);
    DISPATCH(Tangible, ComponentCustomization);
}

void Tangible::AddComponentCustomization(uint32_t customization) {
    auto lock = AcquireLock();
    AddComponentCustomization(customization, lock);
}

void Tangible::AddComponentCustomization(uint32_t customization, boost::unique_lock<boost::mutex>& lock)
{
	component_customization_list_.add(customization);
    DISPATCH(Tangible, ComponentCustomization);
}

std::set<uint32_t> Tangible::GetComponentCustomization() {
    auto lock = AcquireLock();
    return GetComponentCustomization(lock);
}

std::set<uint32_t> Tangible::GetComponentCustomization(boost::unique_lock<boost::mutex>& lock)
{
    return component_customization_list_.raw();
}

void Tangible::ClearComponentCustomization() {
    auto lock = AcquireLock();
    ClearComponentCustomization(lock);
}

void Tangible::ClearComponentCustomization(boost::unique_lock<boost::mutex>& lock)
{
	component_customization_list_.clear();
    DISPATCH(Tangible, ComponentCustomization);
}

void Tangible::SetOptionsMask(uint32_t options_mask) {
    auto lock = AcquireLock();
    SetOptionsMask(options_mask, lock);
}

void Tangible::SetOptionsMask(uint32_t options_mask, boost::unique_lock<boost::mutex>& lock)
{
    options_bitmask_ = options_mask;
	DISPATCH(Tangible, OptionsMask);
}

void Tangible::ToggleOption(uint32_t option) {
    auto lock = AcquireLock();
    ToggleOption(option, lock);
}

void Tangible::ToggleOption(uint32_t option, boost::unique_lock<boost::mutex>& lock)
{
	options_bitmask_ ^= option;
	DISPATCH(Tangible, OptionsMask);
}

uint32_t Tangible::GetOptionsMask() {
    auto lock = AcquireLock();
    return GetOptionsMask(lock);
}

uint32_t Tangible::GetOptionsMask(boost::unique_lock<boost::mutex>& lock)
{
    return options_bitmask_;
}

void Tangible::SetCounter(uint32_t counter) {
    auto lock = AcquireLock();
    SetCounter(counter, lock);
}

void Tangible::SetCounter(uint32_t counter, boost::unique_lock<boost::mutex>& lock)
{
    counter_ = counter;
    DISPATCH(Tangible, Counter);
}

uint32_t Tangible::GetCounter() {
    auto lock = AcquireLock();
    return GetCounter(lock);
}

uint32_t Tangible::GetCounter(boost::unique_lock<boost::mutex>& lock)
{
    return counter_;
}

void Tangible::SetConditionDamage(uint32_t damage) {
    auto lock = AcquireLock();
    SetConditionDamage(damage, lock);
}

void Tangible::SetConditionDamage(uint32_t damage, boost::unique_lock<boost::mutex>& lock)
{
    condition_damage_ = damage;
	DISPATCH(Tangible, ConditionDamage);
	DISPATCH(Tangible, UpdateAttribute);
}

uint32_t Tangible::GetCondition() {
    auto lock = AcquireLock();
    return GetCondition(lock);
}

uint32_t Tangible::GetCondition(boost::unique_lock<boost::mutex>& lock)
{
    return condition_damage_;
}

void Tangible::SetMaxCondition(uint32_t max_condition) {
    auto lock = AcquireLock();
    SetMaxCondition(max_condition, lock);
}

void Tangible::SetMaxCondition(uint32_t max_condition, boost::unique_lock<boost::mutex>& lock)
{
    max_condition_ = max_condition;
    DISPATCH(Tangible, MaxCondition);
	DISPATCH(Tangible, UpdateAttribute);
}

uint32_t Tangible::GetMaxCondition() {
    auto lock = AcquireLock();
    return GetMaxCondition(lock);
}

uint32_t Tangible::GetMaxCondition(boost::unique_lock<boost::mutex>& lock)
{
    return max_condition_;
}

void Tangible::SetStatic(bool is_static) {
    auto lock = AcquireLock();
    SetStatic(is_static, lock);
}

void Tangible::SetStatic(bool is_static, boost::unique_lock<boost::mutex>& lock)
{
    is_static_ = is_static;
	DISPATCH(Tangible, Static);
}

bool Tangible::IsStatic() {
    auto lock = AcquireLock();
    return IsStatic(lock);
}

bool Tangible::IsStatic(boost::unique_lock<boost::mutex>& lock)
{
    return is_static_;
}

bool Tangible::IsDefending(uint64_t defender) {
    auto lock = AcquireLock();
    return IsDefending(defender, lock);
}

bool Tangible::IsDefending(uint64_t defender, boost::unique_lock<boost::mutex>& lock)
{
    for(uint32_t i=0; i < defender_list_.size(); ++i)
	{
		if(defender_list_[i] == defender)
		{
			return true;
		}
	}
	return false;
}

void Tangible::AddDefender(uint64_t defender) {
    auto lock = AcquireLock();
    AddDefender(defender, lock);
}

void Tangible::AddDefender(uint64_t defender, boost::unique_lock<boost::mutex>& lock)
{
	defender_list_.add(defender);
	DISPATCH(Tangible, Defenders);
}

void Tangible::RemoveDefender(uint64_t defender) {
    auto lock = AcquireLock();
    RemoveDefender(defender, lock);
}

void Tangible::RemoveDefender(uint64_t defender, boost::unique_lock<boost::mutex>& lock)
{
    defender_list_.erase(defender);
    DISPATCH(Tangible, Defenders);
}

void Tangible::ResetDefenders(std::vector<uint64_t> defenders) {
    auto lock = AcquireLock();
    ResetDefenders(defenders, lock);
}

void Tangible::ResetDefenders(std::vector<uint64_t> defenders, boost::unique_lock<boost::mutex>& lock)
{
    defender_list_.reset(defenders);
    DISPATCH(Tangible, Defenders);
}

std::vector<uint64_t> Tangible::GetDefenders() {
    auto lock = AcquireLock();
    return GetDefenders(lock);
}

std::vector<uint64_t> Tangible::GetDefenders(boost::unique_lock<boost::mutex>& lock)
{
    return defender_list_.raw();
}

void Tangible::ClearDefenders() {
    auto lock = AcquireLock();
    ClearDefenders(lock);
}

void Tangible::ClearDefenders(boost::unique_lock<boost::mutex>& lock)
{
    defender_list_.clear();
    DISPATCH(Tangible, Defenders);
}

void Tangible::ActivateAutoAttack() {
    auto lock = AcquireLock();
    ActivateAutoAttack(lock);
}

void Tangible::ActivateAutoAttack(boost::unique_lock<boost::mutex>& lock)
{
    auto_attack_ = true;
}

void Tangible::ClearAutoAttack() {
    auto lock = AcquireLock();
    ClearAutoAttack(lock);
}

void Tangible::ClearAutoAttack(boost::unique_lock<boost::mutex>& lock)
{
    auto_attack_ = false;
}

bool Tangible::IsAutoAttacking() {
    auto lock = AcquireLock();
    return IsAutoAttacking(lock);
}

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

void Tangible::SerializeComponentCustomization(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeComponentCustomization(message, lock);
}

void Tangible::SerializeComponentCustomization(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	component_customization_list_.Serialize(message);
}

void Tangible::SerializeDefenders(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeDefenders(message, lock);
}

void Tangible::SerializeDefenders(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	defender_list_.Serialize(message);
}
