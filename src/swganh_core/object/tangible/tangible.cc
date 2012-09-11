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
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        customization_.append(customization);
    }
    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::Customization",static_pointer_cast<Tangible>(shared_from_this())));
}

std::string Tangible::GetCustomization(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return customization_;
}

void Tangible::SetCustomization(const string& customization)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        customization_ = customization;
    }
    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::Customization",static_pointer_cast<Tangible>(shared_from_this())));
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
    
    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::ComponentCustomization",static_pointer_cast<Tangible>(shared_from_this())));
}
void Tangible::AddComponentCustomization(uint32_t customization)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        component_customization_list_.Add(ComponentCustomization(customization));
    }
    
    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::ComponentCustomization",static_pointer_cast<Tangible>(shared_from_this())));
}

NetworkList<ComponentCustomization> Tangible::GetComponentCustomization(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return component_customization_list_;
}

void Tangible::ClearComponentCustomization()
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        component_customization_list_.Clear();
    }
    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::ComponentCustomization",static_pointer_cast<Tangible>(shared_from_this())));
}

void Tangible::SetOptionsMask(uint32_t options_mask)
{
    options_bitmask_ = options_mask;

    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::OptionsMask",static_pointer_cast<Tangible>(shared_from_this())));
}

void Tangible::ToggleOption(uint32_t option)
{
	options_bitmask_ ^= option;
	GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::OptionsMask",static_pointer_cast<Tangible>(shared_from_this())));
}

uint32_t Tangible::GetOptionsMask(void)
{
    return options_bitmask_;
}

void Tangible::SetIncapTimer(uint32_t incap_timer)
{
    incap_timer_ = incap_timer;
    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::IncapTimer",static_pointer_cast<Tangible>(shared_from_this())));
}

uint32_t Tangible::GetIncapTimer(void)
{
    return incap_timer_;
}

void Tangible::SetConditionDamage(uint32_t damage)
{
    condition_damage_ = damage;
    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::ConditionDamage",static_pointer_cast<Tangible>(shared_from_this())));
	
	GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Object::UpdateAttribute", static_pointer_cast<Tangible>(shared_from_this())));
}

uint32_t Tangible::GetCondition(void)
{
    return condition_damage_;
}

void Tangible::SetMaxCondition(uint32_t max_condition)
{
    max_condition_ = max_condition;
    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::MaxCondition",static_pointer_cast<Tangible>(shared_from_this())));
	GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Object::UpdateAttribute", static_pointer_cast<Tangible>(shared_from_this())));
}

uint32_t Tangible::GetMaxCondition(void)
{
    return max_condition_;
}

void Tangible::SetStatic(bool is_static)
{
    is_static_ = is_static;
    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::Static",static_pointer_cast<Tangible>(shared_from_this())));
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

    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::Defenders",static_pointer_cast<Tangible>(shared_from_this())));
}
void Tangible::RemoveDefender(uint64_t defender)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        auto iter = std::find_if(begin(defender_list_), end(defender_list_), [=](const Defender& x)->bool {
            return (x.object_id == defender);
        });

        if(iter != end(defender_list_))
        {
            return;
        }
            
        defender_list_.Remove(iter);
    }
    
    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::Defenders",static_pointer_cast<Tangible>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::Defenders",static_pointer_cast<Tangible>(shared_from_this())));
}

NetworkSortedVector<Defender> Tangible::GetDefenders()
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return defender_list_;
}

void Tangible::ClearDefenders()
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        defender_list_.Clear();
    }   
    GetEventDispatcher()->Dispatch(make_shared<TangibleEvent>
        ("Tangible::Defenders",static_pointer_cast<Tangible>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<ObserverEvent>
        ("Tangible::Baselines",shared_from_this(), observer));
}