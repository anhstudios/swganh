// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "installation.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::messages::containers;

Installation::Installation()
	: Tangible()
	, resource_pool_ids_(10)
	, resource_names_(10)
	, resource_types_(10)
	, hopper_(10)
{
}

uint32_t Installation::GetType() const
{ 
    return Installation::type; 
}

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
	DISPATCH(Installation, Active);
}

float Installation::GetPowerReserve() const
{
	auto lock = AcquireLock();
    return power_reserve_;
}

void Installation::SetPowerReserve(float power_reserve)
{
	{
		auto lock = AcquireLock();
		power_reserve_ = power_reserve;
	}
	DISPATCH(Installation, PowerReserve);
}

float Installation::GetPowerCost() const
{
	auto lock = AcquireLock();
    return power_cost_;
}

void Installation::SetPowerCost(float power_cost)
{
	{
		auto lock = AcquireLock();
		power_cost_ = power_cost;
	}
	DISPATCH(Installation, PowerCost);
}

std::vector<Installation::Resource> Installation::GetAvailableResources()
{
	auto lock = AcquireLock();

	std::vector<Installation::Resource> available_resources;
	uint16_t size = resource_pool_ids_.Size();
	for(uint16_t i = 0; i < size; ++i)
	{
		Installation::Resource resource;
		resource.global_id = resource_pool_ids_[i].id;
		resource.resource_name = resource_names_[i].name;
		resource.resource_type = resource_types_[i].name;
		available_resources.push_back(resource);
	}

    return available_resources;
}

void Installation::AddAvailableResource(uint64_t global_id, std::string name, std::string type)
{
	bool send_update = true;

	{
		auto lock = AcquireLock();
		uint16_t size = resource_pool_ids_.Size();
		for(uint16_t i = 0; i < size; ++i)
		{
			if(resource_pool_ids_[i] == global_id)
			{
				//We already have it
				send_update = false;
				return;
			}
		}
	
		resource_pool_ids_.Add(global_id);
		resource_names_.Add(name);
		resource_types_.Add(type);
	}

	if(send_update)
	{
		DISPATCH(Installation, AvailableResource);
	}
}

void Installation::RemoveAvailableResourceById(uint64_t global_id)
{
	bool send_update = false;

	{
		auto lock = AcquireLock();
		uint16_t size = resource_pool_ids_.Size();
		uint16_t index;
		for(index = 0; index < size; ++index)
		{
			if(resource_pool_ids_[index] == global_id)
			{
				resource_pool_ids_.Remove(index);
				resource_names_.Remove(index);
				resource_types_.Remove(index);
				send_update = true;
				break;
			}
		}
	}

	if(send_update)
	{
		DISPATCH(Installation, AvailableResource);
	}
}

void Installation::UpdateResource(uint64_t global_id, std::string name, std::string type)
{
	bool send_update = false;

	{
		auto lock = AcquireLock();
		uint16_t size = resource_pool_ids_.Size();
		uint16_t index;
		for(index = 0; index < size; ++index)
		{
			if(resource_pool_ids_[index] == global_id)
			{
				resource_pool_ids_.Update(index, ResourceId(global_id));
				resource_names_.Update(index, ResourceString(name));
				resource_types_.Update(index, ResourceString(type));
				send_update = true;
				break;
			}
		}
	}

	if(send_update)
	{
		DISPATCH(Installation, AvailableResource);
	}
}

void Installation::ResetAvailableResources(std::vector<Installation::Resource> available_resource_pool)
{
	{
		auto lock = AcquireLock();
		//Clean out the lists
		resource_pool_ids_.Clear();
		resource_names_.Clear();
		resource_types_.Clear();
    
		//Put the new values in
		for(auto& resource : available_resource_pool)
		{
			resource_pool_ids_.Add(resource.global_id);
			resource_names_.Add(resource.resource_name);
			resource_types_.Add(resource.resource_type);
		}
	
		//Clear Deltas and Reinstall
		resource_pool_ids_.ClearDeltas();
		resource_names_.ClearDeltas();
		resource_types_.ClearDeltas();

		resource_pool_ids_.Reinstall();
		resource_names_.Reinstall();
		resource_types_.Reinstall();
	}
	DISPATCH(Installation, AvailableResource);
}

void Installation::ClearAllAvailableResources()
{
	{
		auto lock = AcquireLock();
		resource_pool_ids_.Clear();
		resource_names_.Clear();
		resource_types_.Clear();
	}
	DISPATCH(Installation, AvailableResource);
}

void Installation::SetDisplayedMaxExtractionRate(uint32_t extraction_rate)
{
	displayed_max_extraction_rate_ = extraction_rate;
	DISPATCH(Installation, DisplayedMaxExtraction);
}
    
uint32_t Installation::GetDisplayedMaxExtractionRate() const
{
	return displayed_max_extraction_rate_;
}

float Installation::GetMaxExtractionRate() const
{
	auto lock = AcquireLock();
    return max_extraction_rate_;
}

void Installation::SetMaxExtractionRate(float extraction_rate)
{
	{
		auto lock = AcquireLock();
		max_extraction_rate_ = extraction_rate;
	}
	DISPATCH(Installation, MaxExtraction);
}

float Installation::GetCurrentExtractionRate() const
{
	auto lock = AcquireLock();
    return current_extraction_rate_;
}

void Installation::SetCurrentExtractionRate(float extraction_rate)
{
	{
		auto lock = AcquireLock();
		current_extraction_rate_ = extraction_rate;
	}
	DISPATCH(Installation, CurrentExtraction);
}

float Installation::GetCurrentHopperSize() const
{
	auto lock = AcquireLock();
    return current_hopper_size_;
}

void Installation::SetCurrentHopperSize(float hopper_size)
{
	{
		auto lock = AcquireLock();
		current_hopper_size_ = hopper_size;
	}
	DISPATCH(Installation, CurrentHopperSize);
}

uint32_t Installation::GetMaxHopperSize() const
{
    return max_hopper_size_;
}

void Installation::SetMaxHopperSize(uint32_t hopper_size)
{
	max_hopper_size_ = hopper_size;
	DISPATCH(Installation, MaxHopperSize);
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
	DISPATCH(Installation, IsUpdating);
}

NetworkSortedVector<Installation::HopperItem>& Installation::GetHopperContents()
{
	auto lock = AcquireLock();
    return hopper_;
}

void Installation::AddToHopper(uint64_t global_id, float quantity)
{
	{
		auto lock = AcquireLock();
		HopperItem item;
		item.global_id = global_id;
		item.quantity = quantity;
		hopper_.Add(move(item));
	}
	DISPATCH(Installation, Hopper);
}

void Installation::RemoveHopperItem(uint64_t global_id)
{
	bool send_update = false;
	{
		auto lock = AcquireLock();
		for(uint16_t i=0; i < hopper_.Size(); ++i)
		{
			if(hopper_[i].global_id == global_id)
			{
				hopper_.Remove(i);
				send_update = true;
				break;
			}
		}
	}

	if(send_update)
	{
		DISPATCH(Installation, Hopper);
	}
}

void Installation::UpdateHopperItem(uint64_t global_id, float quantity)
{
    bool send_update = false;
	{
		auto lock = AcquireLock();
		for(uint16_t i=0; i < hopper_.Size(); ++i)
		{
			if(hopper_[i].global_id == global_id)
			{
				HopperItem new_item;
				new_item.global_id = global_id;
				new_item.quantity = quantity;
				hopper_.Update(i, new_item);

				send_update = true;
				break;
			}
		}
	}

	if(send_update)
	{
		DISPATCH(Installation, Hopper);
	}
}

void Installation::ResetContents(std::vector<Installation::HopperItem> hopper)
{
	{
		auto lock = AcquireLock();
		hopper_.Clear();

		for(auto& item : hopper)
		{
			hopper_.Add(item);
		}

		hopper_.ClearDeltas();
		hopper_.Reinstall();
	}
	DISPATCH(Installation, Hopper);
}

void Installation::ClearAllHopperContents()
{
	{
		auto lock = AcquireLock();
		hopper_.Clear();
	}
	DISPATCH(Installation, Hopper);
}

uint8_t Installation::GetConditionPercentage() const
{
    return condition_percent_;
}

void Installation::SetConditionPercentage(uint8_t condition)
{
	condition = (condition > 100) ? 100 : condition;
	condition_percent_ = condition;
	DISPATCH(Installation, ConditionPercent);
}

NetworkSortedVector<ResourceId>& Installation::GetResourceIds_()
{
	auto lock = AcquireLock();
	return resource_pool_ids_;
}

NetworkSortedVector<ResourceString>& Installation::GetResourceNames_()
{
	auto lock = AcquireLock();
	return resource_names_;
}

NetworkSortedVector<ResourceString>& Installation::GetResourceTypes_()
{
	auto lock = AcquireLock();
	return resource_types_;
}

uint64_t Installation::GetSelectedResourceId()
{
	return selected_resource_;
}

void Installation::SetSelectedResourceId(uint64_t new_id)
{
	selected_resource_ = new_id;
	DISPATCH(Installation, SelectedResource);
}
