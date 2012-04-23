// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "resource_container.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::resource_container;
using namespace swganh::messages;

ResourceContainer::ResourceContainer() : Tangible()
    , current_quantity_(0)
    , global_resource_id_(0)
    , max_quantity_(0)
    , resource_type_("")
    , variation_name_(L"")
{
}
ResourceContainer::ResourceContainer(const std::string& customization, std::vector<uint32_t> component_customization, uint32_t bitmask_options,
        uint32_t incap_timer, uint32_t condition_damage, uint32_t max_condition, bool is_static, std::vector<uint64_t> defenders,
        uint32_t current_quantity, uint64_t global_resource_id, uint32_t max_quantity, std::string resource_type, std::wstring name)
    : Tangible(customization, component_customization, bitmask_options, incap_timer, condition_damage, max_condition, is_static, defenders)
    , current_quantity_(current_quantity)
    , global_resource_id_(global_resource_id)
    , max_quantity_(max_quantity)
    , resource_type_(resource_type)
    , variation_name_(name)
{
}

void ResourceContainer::SetCurrentQuantity(uint32_t current_quantity)
{
    current_quantity_ = current_quantity;
    /*if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_3, 11);
        message.data.write(current_quantity_);

        AddDeltasUpdate(move(message));
    }*/
}

void ResourceContainer::SetMaxQuantity(uint32_t max_quantity)
{
    max_quantity_ = max_quantity;
    /*if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_6, 2);
        message.data.write(max_quantity_);

        AddDeltasUpdate(move(message));
    }*/
}
void ResourceContainer::SetResourceType(const string& resource_type)
{
    resource_type_ = resource_type;
    /*if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_6, 3);
        message.data.write(resource_type_);

        AddDeltasUpdate(move(message));
    }*/
}
void ResourceContainer::SetResourceName(const wstring& name)
{
    variation_name_ = name;
    /*if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_6, 4);
        message.data.write(max_quantity_);

        AddDeltasUpdate(move(message));
    }*/
}
void ResourceContainer::GetBaseline3()
{
    //auto message = CreateBaselinesMessage(Object::VIEW_3, 13);
    //
    //// base data
    //message.data.append(Tangible::GetBaseline3().get().data);
    //message.data.write(GetCurrentQuantity());
    //message.data.write(GetGlobalResource());

    //return boost::optional<BaselinesMessage>(std::move(message));
}
void ResourceContainer::GetBaseline6()
{
    //auto message = CreateBaselinesMessage(Object::VIEW_6, 5);
    //
    //// base data
    //message.data.append(Tangible::GetBaseline6().get().data);
    //message.data.write(GetMaxQuantity());
    //message.data.write(GetResourceType());
    //message.data.write(GetResourceName());

    //return boost::optional<BaselinesMessage>(std::move(message));
}