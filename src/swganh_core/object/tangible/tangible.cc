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

void Tangible::AddCustomization(const std::string& customization)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        customization_.append(customization);
    }
	DISPATCH(Tangible, Customization);
}

std::string Tangible::GetCustomization(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return customization_;
}

void Tangible::SetCustomization(const std::string& customization)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        customization_ = customization;
    }
    DISPATCH(Tangible, Customization);
}
void Tangible::SetCustomizationFromInts(std::vector<int> customization_ints)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		customization_ = std::string(customization_ints.begin(), customization_ints.end());
	}
	DISPATCH(Tangible, Customization);
}
void Tangible::RemoveComponentCustomization(uint32_t customization)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        auto iter = std::find_if(begin(component_customization_list_), end(component_customization_list_), [=](ComponentCustomization component) {
            return component.component_customization_crc = customization;
        });

        if(iter == end(component_customization_list_))
        {
            return;
        }
            
        component_customization_list_.Remove(iter);
    }
    DISPATCH(Tangible, ComponentCustomization);
}
void Tangible::AddComponentCustomization(uint32_t customization)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        component_customization_list_.Add(ComponentCustomization(customization));
    }
    DISPATCH(Tangible, ComponentCustomization);
}

std::list<ComponentCustomization> Tangible::GetComponentCustomization(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return std::move(component_customization_list_.Get());
}

void Tangible::ClearComponentCustomization()
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        component_customization_list_.Clear();
    }
    DISPATCH(Tangible, ComponentCustomization);
}

void Tangible::SetOptionsMask(uint32_t options_mask)
{
    options_bitmask_ = options_mask;
	DISPATCH(Tangible, OptionsMask);
}

void Tangible::ToggleOption(uint32_t option)
{
	options_bitmask_ ^= option;
	DISPATCH(Tangible, OptionsMask);
}

uint32_t Tangible::GetOptionsMask(void)
{
    return options_bitmask_;
}

void Tangible::SetCounter(uint32_t counter)
{
    counter_ = counter;
    DISPATCH(Tangible, Counter);
}

uint32_t Tangible::GetCounter(void)
{
    return counter_;
}

void Tangible::SetConditionDamage(uint32_t damage)
{
    condition_damage_ = damage;
	DISPATCH(Tangible, ConditionDamage);
	DISPATCH(Tangible, UpdateAttribute);
}

uint32_t Tangible::GetCondition(void)
{
    return condition_damage_;
}

void Tangible::SetMaxCondition(uint32_t max_condition)
{
    max_condition_ = max_condition;
    DISPATCH(Tangible, MaxCondition);
	DISPATCH(Tangible, UpdateAttribute);
}

uint32_t Tangible::GetMaxCondition(void)
{
    return max_condition_;
}

void Tangible::SetStatic(bool is_static)
{
    is_static_ = is_static;
	DISPATCH(Tangible, Static);
}

bool Tangible::IsStatic(void)
{
    return is_static_;
}

bool Tangible::IsDefending(uint64_t defender)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto iter = std::find_if(begin(defender_list_), end(defender_list_), [=](const Defender& x)->bool {
        return (x.object_id == defender);
    });

    if(iter != end(defender_list_))
        return true;
    else
        return false;
}
void Tangible::AddDefender(uint64_t defender)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        defender_list_.Add(Defender(defender));
    }
	DISPATCH(Tangible, Defenders);
}
void Tangible::RemoveDefender(uint64_t defender)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        auto iter = std::find_if(begin(defender_list_), end(defender_list_), [=](const Defender& x)->bool {
            return (x.object_id == defender);
        });

        if(iter == end(defender_list_))
        {
            return;
        }
            
        defender_list_.Remove(iter);
    }
    DISPATCH(Tangible, Defenders);
}
void Tangible::ResetDefenders(std::vector<uint64_t> defenders)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        defender_list_.Clear();
        std::for_each(defenders.begin(), defenders.end(), [=](const uint64_t& x) {
            defender_list_.Insert(Defender(x));
        });
        defender_list_.Reinstall();
    }
    DISPATCH(Tangible, Defenders);
}

std::vector<Defender> Tangible::GetDefenders()
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return std::move(defender_list_.Get());
}

void Tangible::ClearDefenders()
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        defender_list_.Clear();
    }   
    DISPATCH(Tangible, Defenders);
}

void Tangible::ActivateAutoAttack()
{
    auto_attack_ = true;
}
void Tangible::ClearAutoAttack()
{
    auto_attack_ = false;
}
bool Tangible::IsAutoAttacking()
{
    return auto_attack_ == true;
}
void Tangible::CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
    if (event_dispatcher_)
	{
		GetEventDispatcher()->Dispatch(make_shared<ObserverEvent>
			("Tangible::Baselines", shared_from_this(), observer));
	}
}

std::shared_ptr<Object> Tangible::Clone()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	auto other = make_shared<Tangible>();
	Clone(other);
	return other;
}

void Tangible::Clone(std::shared_ptr<Tangible> other)
{
	other->customization_ =  customization_;
	other->component_customization_list_ = component_customization_list_;
	other->options_bitmask_.store(options_bitmask_);
    other->counter_.store(counter_);
	other->condition_damage_.store(condition_damage_);
    other->max_condition_.store(max_condition_);
    other->is_static_.store(is_static_);

	//Call the method in the super class
	Object::Clone(other);
}

void Tangible::SerializeComponentCustomization(swganh::messages::BaseSwgMessage* message)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	component_customization_list_.Serialize(message);
}

void Tangible::SerializeDefenders(swganh::messages::BaseSwgMessage* message)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	defender_list_.Serialize(message);
}