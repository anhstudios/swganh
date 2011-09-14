
#include "swganh/object/installation.h"

using namespace std;
using namespace swganh::object;

bool Installation::IsActive() const
{
    return is_active_;
}

void Installation::Activate()
{
    if (!IsActive())
    {
        ToggleActive();
    }
}

void Installation::Deactivate()
{
    if (IsActive())
    {
        ToggleActive();
    }
}

void Installation::ToggleActive()
{
    is_active_ = !is_active_;
}

float Installation::GetPowerReserve() const
{
    return power_reserve_;
}

void Installation::ResetPowerReserve(float power_reserve)
{
    power_reserve_ = power_reserve;
}

float Installation::GetPowerCost() const
{
    return power_cost_;
}

void Installation::ResetPowerCost(float power_cost)
{
    power_cost_ = power_cost;
}

std::vector<Installation::Resource> Installation::GetAvailableResources() const
{
    return available_resource_pool_;
}

void Installation::AddAvailableResource(uint64_t global_id, std::string name, std::string type)
{
    auto find_iter = find_if(
        available_resource_pool_.begin(),
        available_resource_pool_.end(),
        [global_id] (const Resource& resource)
    {
        return global_id == resource.global_id;
    });

    if (find_iter != available_resource_pool_.end())
    {
        // Already in the list.
        return;
    }

    Resource resource;
    resource.global_id = global_id;
    resource.name = move(name);
    resource.type = move(type);

    available_resource_pool_.push_back(move(resource));
}

void Installation::RemoveAvailableResourceById(uint64_t global_id)
{
    auto find_iter = find_if(
        available_resource_pool_.begin(),
        available_resource_pool_.end(),
        [global_id] (const Resource& resource)
    {
        return global_id == resource.global_id;
    });

    if (find_iter == available_resource_pool_.end())
    {
        // Not in the list.
        return;
    }

    available_resource_pool_.erase(find_iter);
}

void Installation::UpdateResource(uint64_t global_id, std::string name, std::string type)
{

    auto find_iter = find_if(
        available_resource_pool_.begin(),
        available_resource_pool_.end(),
        [global_id] (const Resource& resource)
    {
        return global_id == resource.global_id;
    });

    if (find_iter == available_resource_pool_.end())
    {
        // Not in the list.
        return;
    }

    find_iter->name = move(name);
    find_iter->type = move(type);
}

void Installation::ResetAvailableResources(std::vector<Installation::Resource> available_resource_pool)
{
    available_resource_pool_ = move(available_resource_pool);
}

void Installation::ClearAllAvailableResources()
{
    available_resource_pool_.clear();
}

float Installation::GetMaxExtractionRate() const
{
    return max_extraction_rate_;
}

void Installation::SetMaxExtractionRate(float extraction_rate)
{
    max_extraction_rate_ = extraction_rate;
}

float Installation::GetCurrentExtractionRate() const
{
    return current_extraction_rate_;
}

void Installation::SetCurrentExtractionRate(float extraction_rate)
{
    current_extraction_rate_ = extraction_rate;
}

float Installation::GetCurrentHopperSize() const
{
    return current_hopper_size_;
}

void Installation::SetCurrentHopperSize(float hopper_size)
{
    current_hopper_size_ = hopper_size;
}

float Installation::GetMaxHopperSize() const
{
    return max_hopper_size_;
}

void Installation::SetMaxHopperSize(float hopper_size)
{
    max_hopper_size_ = hopper_size;
}

bool Installation::IsUpdating() const
{
    return is_updating_;
}

void Installation::StartUpdating()
{
    if (!IsUpdating())
    {
        ToggleUpdating();
    }
}

void Installation::StopUpdating()
{
    if (IsUpdating())
    {
        ToggleUpdating();
    }
}

void Installation::ToggleUpdating()
{
    is_updating_ = !is_updating_;
}

std::vector<Installation::HopperItem> Installation::GetHopperContents() const
{
    return hopper_;
}

void Installation::AddToHopper(uint64_t global_id, float quantity)
{
    auto find_iter = find_if(
        hopper_.begin(),
        hopper_.end(),
        [global_id] (const HopperItem& hopper_item)
    {
        return global_id == hopper_item.global_id;
    });

    if (find_iter != hopper_.end())
    {
        // Already in the list.
        return;
    }

    HopperItem item;
    item.global_id = global_id;
    item.quantity = quantity;

    hopper_.push_back(move(item));
}

void Installation::RemoveHopperItem(uint64_t global_id)
{
    auto find_iter = find_if(
        hopper_.begin(),
        hopper_.end(),
        [global_id] (const HopperItem& hopper_item)
    {
        return global_id == hopper_item.global_id;
    });

    if (find_iter == hopper_.end())
    {
        // Not in the list.
        return;
    }

    hopper_.erase(find_iter);
}

void Installation::UpdateHopperItem(uint64_t global_id, float quantity)
{
    auto find_iter = find_if(
        hopper_.begin(),
        hopper_.end(),
        [global_id] (const HopperItem& hopper_item)
    {
        return global_id == hopper_item.global_id;
    });

    if (find_iter == hopper_.end())
    {
        // Not in the list.
        return;
    }

    find_iter->quantity = quantity;
}

void Installation::ResetContents(std::vector<Installation::HopperItem> hopper)
{
    hopper_ = move(hopper);
}

void Installation::ClearAllHopperContents()
{
    hopper_.clear();
}

uint8_t Installation::GetConditionPercentage() const
{
    return condition_percent_;
}

void Installation::SetConditionPercentage(uint8_t condition)
{
    condition = (condition > 100) ? 100 : condition;

    condition_percent_ = condition;
}
