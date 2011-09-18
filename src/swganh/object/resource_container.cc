#include "resource_container.h"

#include "swganh/scene/scene.h"
#include "swganh/scene/messages/deltas_message.h"

using namespace swganh::object;
using namespace swganh::scene::messages;
using namespace std;

ResourceContainer::ResourceContainer() : BaseTangible()
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
    : BaseTangible(customization, component_customization, bitmask_options, incap_timer, condition_damage, max_condition, is_static, defenders)
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
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(11);
        message.data.write(current_quantity_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));
    }
}

void ResourceContainer::SetMaxQuantity(uint32_t max_quantity)
{
    max_quantity_ = max_quantity;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(2);
        message.data.write(max_quantity_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_6, move(message)));
    }
}
void ResourceContainer::SetResourceType(const string& resource_type)
{
    resource_type_ = resource_type;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(3);
        message.data.write(resource_type_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_6, move(message)));
    }
}
void ResourceContainer::SetResourceName(const wstring& name)
{
    variation_name_ = name;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(4);
        message.data.write(max_quantity_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_6, move(message)));
    }
}