
#include "tangible.h"

#include "swganh/scene/scene.h"
#include "swganh/scene/messages/deltas_message.h"

using namespace swganh::object;
using namespace swganh::scene::messages;

Tangible::Tangible()
    : customization_(0)
    , component_customization_list_(std::vector<uint32_t>())
    , component_customization_list_counter_(0)
    , options_bitmask_(0)
    , incap_timer_(0)
    , condition_damage_(0)
    , max_condition_(0)
    , is_static_(false)
    , defender_list_(std::set<uint64_t>())
    , defender_list_counter_(0)
{}

void Tangible::AddCustomization(const std::string& customization)
{
    customization_.append(customization);
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(4);
        message.data.write(customization_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}

void Tangible::SetCustomization(const std::string& customization)
{
    customization_ = customization;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(4);
        message.data.write(customization_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}

void Tangible::AddComponentCustomization(uint32_t customization)
{
    // add customization
    component_customization_list_.push_back(customization);
    // update counter
    component_customization_list_counter_++;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(5);
        message.data.write(component_customization_list_.size());
        message.data.write(component_customization_list_counter_);
        message.data.write(component_customization_list_);
        // update type add
        message.data.write<uint8_t>(1);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}

void Tangible::SetComponentCustomization(std::vector<uint32_t> component_customization)
{
    component_customization_list_ = component_customization;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(5);
        message.data.write(component_customization_list_.size());
        message.data.write(component_customization_list_counter_);
        message.data.write(component_customization_list_);
        // update type clearall
        message.data.write<uint8_t>(2);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}
