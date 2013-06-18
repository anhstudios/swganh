// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "installation.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::containers;

Installation::Installation()
	: Tangible()
	, resource_pool_ids_(10)
	, resource_names_(10)
	, resource_types_(10)
{
}

uint32_t Installation::GetType() const
{ 
    return Installation::type; 
}

bool Installation::IsActive() const {
    auto lock = AcquireLock();
    return IsActive(lock);
}

bool Installation::IsActive(boost::unique_lock<boost::mutex>& lock) const
{
    return is_active_;
}

void Installation::Activate() {
    auto lock = AcquireLock();
    Activate(lock);
}

void Installation::Activate(boost::unique_lock<boost::mutex>& lock)
{
    if (!IsActive())
    {
        ToggleActive();
    }
}

void Installation::Deactivate() {
    auto lock = AcquireLock();
    Deactivate(lock);
}

void Installation::Deactivate(boost::unique_lock<boost::mutex>& lock)
{
    if (IsActive())
    {
        ToggleActive();
    }
}

void Installation::ToggleActive() {
    auto lock = AcquireLock();
    ToggleActive(lock);
}

void Installation::ToggleActive(boost::unique_lock<boost::mutex>& lock)
{
    is_active_ = !is_active_;
	DISPATCH(Installation, Active);
}
 
float Installation::GetPowerReserve() const {
    auto lock = AcquireLock();
    return GetPowerReserve(lock);
}

float Installation::GetPowerReserve(boost::unique_lock<boost::mutex>& lock) const
{
    return power_reserve_;
}

void Installation::SetPowerReserve(float power_reserve) {
    auto lock = AcquireLock();
    SetPowerReserve(power_reserve, lock);
}

void Installation::SetPowerReserve(float power_reserve, boost::unique_lock<boost::mutex>& lock)
{
	power_reserve_ = power_reserve;
	DISPATCH(Installation, PowerReserve);
}

float Installation::GetPowerCost() const {
    auto lock = AcquireLock();
    return GetPowerCost(lock);
}

float Installation::GetPowerCost(boost::unique_lock<boost::mutex>& lock) const
{
    return power_cost_;
}

void Installation::SetPowerCost(float power_cost) {
    auto lock = AcquireLock();
    SetPowerCost(power_cost, lock);
}

void Installation::SetPowerCost(float power_cost, boost::unique_lock<boost::mutex>& lock)
{
	power_cost_ = power_cost;
	DISPATCH(Installation, PowerCost);
}

std::vector<Resource> Installation::GetAvailableResources() {
    auto lock = AcquireLock();
    return GetAvailableResources(lock);
}

std::vector<Resource> Installation::GetAvailableResources(boost::unique_lock<boost::mutex>& lock)
{
	std::vector<Resource> available_resources;
	uint16_t size = resource_pool_ids_.size();
	for(uint16_t i = 0; i < size; ++i)
	{
		Resource resource;
		resource.global_id = resource_pool_ids_[i];
		resource.resource_name = resource_names_[i];
		resource.resource_type = resource_types_[i];
		available_resources.push_back(resource);
	}

    return available_resources;
}

void Installation::AddAvailableResource(uint64_t global_id, std::string name, std::string type) {
    auto lock = AcquireLock();
    AddAvailableResource(global_id, name, type, lock);
}

void Installation::AddAvailableResource(uint64_t global_id, std::string name, std::string type, boost::unique_lock<boost::mutex>& lock)
{
	bool send_update = true;

	uint16_t size = resource_pool_ids_.size();
	for(uint16_t i = 0; i < size; ++i)
	{
		if(resource_pool_ids_[i] == global_id)
		{
			//We already have it
			send_update = false;
			return;
		}
	}
	
	resource_pool_ids_.add(global_id);
	resource_names_.add(name);
	resource_types_.add(type);

	if(send_update)
	{
		DISPATCH(Installation, AvailableResource);
	}
}

void Installation::RemoveAvailableResourceById(uint64_t global_id) {
    auto lock = AcquireLock();
    RemoveAvailableResourceById(global_id, lock);
}

void Installation::RemoveAvailableResourceById(uint64_t global_id, boost::unique_lock<boost::mutex>& lock)
{
	bool send_update = false;
	uint16_t size = resource_pool_ids_.size();
	uint16_t index;
	for(index = 0; index < size; ++index)
	{
		if(resource_pool_ids_[index] == global_id)
		{
			resource_pool_ids_.remove(index);
			resource_names_.remove(index);
			resource_types_.remove(index);
			send_update = true;
			break;
		}
	}

	if(send_update)
	{
		DISPATCH(Installation, AvailableResource);
	}
}

void Installation::UpdateResource(uint64_t global_id, std::string name, std::string type) {
    auto lock = AcquireLock();
    UpdateResource(global_id, name, type, lock);
}

void Installation::UpdateResource(uint64_t global_id, std::string name, std::string type,boost::unique_lock<boost::mutex>& lock)
{
	bool send_update = false;
	uint16_t size = resource_pool_ids_.size();
	uint16_t index;
	for(index = 0; index < size; ++index)
	{
		if(resource_pool_ids_[index] == global_id)
		{
			resource_pool_ids_.update(index, global_id);
			resource_names_.update(index, name);
			resource_types_.update(index, type);
			send_update = true;
			break;
		}
	}

	if(send_update)
	{
		DISPATCH(Installation, AvailableResource);
	}
}

void Installation::ClearAllAvailableResources() {
    auto lock = AcquireLock();
    ClearAllAvailableResources(lock);
}

void Installation::ClearAllAvailableResources(boost::unique_lock<boost::mutex>& lock)
{
	resource_pool_ids_.clear();
	resource_names_.clear();
	resource_types_.clear();
	DISPATCH(Installation, AvailableResource);
}

void Installation::SetDisplayedMaxExtractionRate(uint32_t extraction_rate) {
    auto lock = AcquireLock();
    SetDisplayedMaxExtractionRate(extraction_rate, lock);
}

void Installation::SetDisplayedMaxExtractionRate(uint32_t extraction_rate,boost::unique_lock<boost::mutex>& lock)
{
	displayed_max_extraction_rate_ = extraction_rate;
	DISPATCH(Installation, DisplayedMaxExtraction);
}
    
uint32_t Installation::GetDisplayedMaxExtractionRate() const {
    auto lock = AcquireLock();
    return GetDisplayedMaxExtractionRate(lock);
}

uint32_t Installation::GetDisplayedMaxExtractionRate(boost::unique_lock<boost::mutex>& lock) const
{
	return displayed_max_extraction_rate_;
}

float Installation::GetMaxExtractionRate() const {
    auto lock = AcquireLock();
    return GetMaxExtractionRate(lock);
}

float Installation::GetMaxExtractionRate(boost::unique_lock<boost::mutex>& lock) const
{
    return max_extraction_rate_;
}

void Installation::SetMaxExtractionRate(float extraction_rate) {
    auto lock = AcquireLock();
    SetMaxExtractionRate(extraction_rate, lock);
}

void Installation::SetMaxExtractionRate(float extraction_rate,boost::unique_lock<boost::mutex>& lock)
{
	max_extraction_rate_ = extraction_rate;
	DISPATCH(Installation, MaxExtraction);
}

float Installation::GetCurrentExtractionRate() const  {
    auto lock = AcquireLock();
    return GetCurrentExtractionRate(lock);
}

float Installation::GetCurrentExtractionRate(boost::unique_lock<boost::mutex>& lock) const
{
    return current_extraction_rate_;
}

void Installation::SetCurrentExtractionRate(float extraction_rate) {
    auto lock = AcquireLock();
    SetCurrentExtractionRate(extraction_rate, lock);
}

void Installation::SetCurrentExtractionRate(float extraction_rate,boost::unique_lock<boost::mutex>& lock)
{
	current_extraction_rate_ = extraction_rate;
	DISPATCH(Installation, CurrentExtraction);
}
 
float Installation::GetCurrentHopperSize() const {
    auto lock = AcquireLock();
    return GetCurrentHopperSize(lock);
}

float Installation::GetCurrentHopperSize(boost::unique_lock<boost::mutex>& lock) const
{
    return current_hopper_size_;
}

void Installation::SetCurrentHopperSize(float hopper_size) {
    auto lock = AcquireLock();
    SetCurrentHopperSize(hopper_size, lock);
}

void Installation::SetCurrentHopperSize(float hopper_size,boost::unique_lock<boost::mutex>& lock)
{
	current_hopper_size_ = hopper_size;
	DISPATCH(Installation, CurrentHopperSize);
}

uint32_t Installation::GetMaxHopperSize() const {
    auto lock = AcquireLock();
    return GetMaxHopperSize(lock);
}

uint32_t Installation::GetMaxHopperSize(boost::unique_lock<boost::mutex>& lock) const
{
    return max_hopper_size_;
}

void Installation::SetMaxHopperSize(uint32_t hopper_size) {
    auto lock = AcquireLock();
    SetMaxHopperSize(hopper_size, lock);
}

void Installation::SetMaxHopperSize(uint32_t hopper_size,boost::unique_lock<boost::mutex>& lock)
{
	max_hopper_size_ = hopper_size;
	DISPATCH(Installation, MaxHopperSize);
}

bool Installation::IsUpdating() const {
    auto lock = AcquireLock();
    return IsUpdating(lock);
}

bool Installation::IsUpdating(boost::unique_lock<boost::mutex>& lock) const
{
    return is_updating_;
}

void Installation::StartUpdating() {
    auto lock = AcquireLock();
    StartUpdating(lock);
}

void Installation::StartUpdating(boost::unique_lock<boost::mutex>& lock)
{
    if (!IsUpdating())
    {
        ToggleUpdating();
    }
}

void Installation::StopUpdating() {
    auto lock = AcquireLock();
    StopUpdating(lock);
}

void Installation::StopUpdating(boost::unique_lock<boost::mutex>& lock)
{
    if (IsUpdating())
    {
        ToggleUpdating();
    }
}

void Installation::ToggleUpdating() {
    auto lock = AcquireLock();
    ToggleUpdating(lock);
}

void Installation::ToggleUpdating(boost::unique_lock<boost::mutex>& lock)
{
    is_updating_ = !is_updating_;
	DISPATCH(Installation, IsUpdating);
}

std::vector<HopperItem> Installation::GetHopperContents() {
    auto lock = AcquireLock();
    return GetHopperContents(lock);
}

std::vector<HopperItem> Installation::GetHopperContents(boost::unique_lock<boost::mutex>& lock)
{
    return hopper_.raw();
}

void Installation::AddToHopper(uint64_t global_id, float quantity) {
    auto lock = AcquireLock();
    AddToHopper(global_id, quantity, lock);
}

void Installation::AddToHopper(uint64_t global_id, float quantity, boost::unique_lock<boost::mutex>& lock)
{
	HopperItem item;
	item.global_id = global_id;
	item.quantity = quantity;
	hopper_.add(item);
	DISPATCH(Installation, Hopper);
}

void Installation::RemoveHopperItem(uint64_t global_id) {
    auto lock = AcquireLock();
    RemoveHopperItem(global_id, lock);
}

void Installation::RemoveHopperItem(uint64_t global_id, boost::unique_lock<boost::mutex>& lock)
{
	bool send_update = false;
	for(uint16_t i=0; i < hopper_.size(); ++i)
	{
		if(hopper_[i].global_id == global_id)
		{
			hopper_.remove(i);
			send_update = true;
			break;
		}
	}

	if(send_update)
	{
		DISPATCH(Installation, Hopper);
	}
}

void Installation::UpdateHopperItem(uint64_t global_id, float quantity) {
    auto lock = AcquireLock();
    UpdateHopperItem(global_id, quantity, lock);
}

void Installation::UpdateHopperItem(uint64_t global_id, float quantity, boost::unique_lock<boost::mutex>& lock)
{
    bool send_update = false;
	for(uint16_t i=0; i < hopper_.size(); ++i)
	{
		if(hopper_[i].global_id == global_id)
		{
			HopperItem new_item;
			new_item.global_id = global_id;
			new_item.quantity = quantity;
			hopper_.update(i);

			send_update = true;
			break;
		}
	}

	if(send_update)
	{
		DISPATCH(Installation, Hopper);
	}
}

void Installation::ResetContents(std::vector<HopperItem> hopper) {
    auto lock = AcquireLock();
    ResetContents(hopper, lock);
}

void Installation::ResetContents(std::vector<HopperItem> hopper,boost::unique_lock<boost::mutex>& lock)
{
	hopper_.reset(hopper);
	DISPATCH(Installation, Hopper);
}

void Installation::ClearAllHopperContents() {
    auto lock = AcquireLock();
    ClearAllHopperContents(lock);
}

void Installation::ClearAllHopperContents(boost::unique_lock<boost::mutex>& lock)
{
	hopper_.clear();
	DISPATCH(Installation, Hopper);
}

uint8_t Installation::GetConditionPercentage() const {
    auto lock = AcquireLock();
    return GetConditionPercentage(lock);
}

uint8_t Installation::GetConditionPercentage(boost::unique_lock<boost::mutex>& lock) const
{
    return condition_percent_;
}

void Installation::SetConditionPercentage(uint8_t condition) {
    auto lock = AcquireLock();
    SetConditionPercentage(condition, lock);
}

void Installation::SetConditionPercentage(uint8_t condition,boost::unique_lock<boost::mutex>& lock)
{
	condition = (condition > 100) ? 100 : condition;
	condition_percent_ = condition;
	DISPATCH(Installation, ConditionPercent);
}

std::vector<uint64_t> Installation::GetResourceIds() {
    auto lock = AcquireLock();
    return GetResourceIds(lock);
}

std::vector<uint64_t> Installation::GetResourceIds(boost::unique_lock<boost::mutex>& lock)
{
	return resource_pool_ids_.raw();
}

std::vector<std::string> Installation::GetResourceNames() {
    auto lock = AcquireLock();
    return GetResourceNames(lock);
}

std::vector<std::string> Installation::GetResourceNames(boost::unique_lock<boost::mutex>& lock)
{
	return resource_names_.raw();
}

std::vector<std::string> Installation::GetResourceTypes() {
    auto lock = AcquireLock();
    return GetResourceTypes(lock);
}

std::vector<std::string> Installation::GetResourceTypes(boost::unique_lock<boost::mutex>& lock)
{
	return resource_types_.raw();
}

uint64_t Installation::GetSelectedResourceId() {
    auto lock = AcquireLock();
    return GetSelectedResourceId(lock);
}

uint64_t Installation::GetSelectedResourceId(boost::unique_lock<boost::mutex>& lock)
{
	return selected_resource_;
}

void Installation::SetSelectedResourceId(uint64_t new_id) {
    auto lock = AcquireLock();
    SetSelectedResourceId(new_id, lock);
}

void Installation::SetSelectedResourceId(uint64_t new_id,boost::unique_lock<boost::mutex>& lock)
{
	selected_resource_ = new_id;
	DISPATCH(Installation, SelectedResource);
}

void Installation::SerializeResourceIds(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeResourceIds(message, lock);
}

void Installation::SerializeResourceIds(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	resource_pool_ids_.Serialize(message);
}

void Installation::SerializeResourceNames(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeResourceNames(message, lock);
}

void Installation::SerializeResourceNames(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	resource_names_.Serialize(message);
}

void Installation::SerializeResourceTypes(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeResourceTypes(message, lock);
}

void Installation::SerializeResourceTypes(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	resource_types_.Serialize(message);
}

void Installation::SerializeHopperContents(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeHopperContents(message, lock);
}

void Installation::SerializeHopperContents(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	hopper_.Serialize(message);
}
