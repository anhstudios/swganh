
#include "swganh/object/base_tangible.h"

#include "swganh/scene/scene.h"
#include "swganh/scene/messages/deltas_message.h"

using namespace swganh::object;
using namespace swganh::scene;
using namespace messages;
using namespace std;

BaseTangible::BaseTangible()
    : customization_(0)
    , component_customization_list_(std::vector<uint32_t>())
    , component_customization_list_counter_(0)
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
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(4);
        message.data.write(customization_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));
    }
}

void BaseTangible::SetCustomization(const string& customization)
{
    customization_ = customization;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(4);
        message.data.write(customization_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));
    }
}

void BaseTangible::AddComponentCustomization(uint32_t customization)
{
    // add customization
    component_customization_list_.push_back(customization);
    // update counter
    component_customization_list_counter_++;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(5);
        message.data.write(component_customization_list_.size());
        message.data.write(component_customization_list_counter_);
        message.data.write(component_customization_list_);
        // update type add
        message.data.write<uint8_t>(1);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));
    }
}

void BaseTangible::SetComponentCustomization(vector<uint32_t> component_customization)
{
    component_customization_list_ = component_customization;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(5);
        message.data.write(component_customization_list_.size());
        message.data.write(component_customization_list_counter_);
        message.data.write(component_customization_list_);
        // update type clearall
        message.data.write<uint8_t>(2);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));
    }
}

void BaseTangible::SetOptionsMask(uint32_t options_mask)
{
    options_bitmask_ = options_mask;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(6);
        message.data.write(options_bitmask_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));
    }
}
void BaseTangible::SetIncapTimer(uint32_t incap_timer)
{
    incap_timer_ = incap_timer;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(7);
        message.data.write(incap_timer_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));
    }
}
void BaseTangible::SetConditionDamage(uint32_t damage)
{
    condition_damage_ = damage;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(8);
        message.data.write(condition_damage_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));
    }
}
void BaseTangible::SetMaxCondition(uint32_t max_condition)
{
    max_condition_ = max_condition;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(9);
        message.data.write(max_condition_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));
    }
}
void BaseTangible::SetStatic(bool is_static)
{
    is_static_ = is_static;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        uint8_t val = MOVEABLE;
        if (is_static_)
            val = STATIC;
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(10);
        message.data.write(val);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));
    }
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
    defender_list_counter_++;

    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(1);
        message.data.write(defender_list_.size());
        message.data.write(defender_list_counter_);
        // end defender index
        message.data.write(defender_list_.back());
        // defender we are adding
        message.data.write(defender);
        // update subtype (add)
        message.data.write<uint8_t>(1);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_6, move(message)));
    }
}
void BaseTangible::RemoveDefender(uint64_t defender)
{
    // bail if not found
    auto found = FindDefender(defender);
    if (found == end(defender_list_))
        return;

    defender_list_.erase(found);

    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(1);
        message.data.write<uint16_t>(defender_list_.size());
        message.data.write<uint16_t>(defender_list_counter_);
        message.data.write<uint16_t>(found - defender_list_.begin());
        // update subtype (remove)
        message.data.write<uint8_t>(0);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_6, move(message)));
    }
    // update counter
    defender_list_counter_--;
}
void BaseTangible::ResetDefenders(std::vector<uint64_t> defenders)
{
    defender_list_.clear();
    defender_list_ = defenders;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
        // update count (not sure if this is the new size or just 1)?

        message.data.write<uint16_t>(defender_list_.size());
        // update type
        message.data.write<uint16_t>(1);
        message.data.write<uint16_t>(defender_list_.size());
        message.data.write<uint8_t>(3);
        for_each(begin(defender_list_), end(defender_list_), [&message](uint64_t defender) {
            message.data.write(defender);
        });
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_6, move(message)));
    }
    // update counter
    defender_list_counter_ = 0;
}
void BaseTangible::ClearDefenders()
{
    defender_list_.clear();

    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(1);
        // update sub type (reset all)
        message.data.write<uint8_t>(4);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_6, move(message)));
    }
    // update counter
    defender_list_counter_ = 0;
}