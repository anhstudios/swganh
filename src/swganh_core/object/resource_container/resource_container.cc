// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "resource_container.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;
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

uint32_t ResourceContainer::GetCurrentQuantity() {
    auto lock = AcquireLock();
    return GetCurrentQuantity(lock);
}

uint32_t ResourceContainer::GetCurrentQuantity(boost::unique_lock<boost::mutex>& lock)
{
	return current_quantity_;
}

uint64_t ResourceContainer::GetGlobalResource() {
    auto lock = AcquireLock();
    return GetGlobalResource(lock);
}

uint64_t ResourceContainer::GetGlobalResource(boost::unique_lock<boost::mutex>& lock)
{
	return global_resource_id_;
}

uint32_t ResourceContainer::GetMaxQuantity() {
    auto lock = AcquireLock();
    return GetMaxQuantity(lock);
}

uint32_t ResourceContainer::GetMaxQuantity(boost::unique_lock<boost::mutex>& lock)
{
	return max_quantity_;
}

std::string ResourceContainer::GetResourceType() {
    auto lock = AcquireLock();
    return GetResourceType(lock);
}

std::string ResourceContainer::GetResourceType(boost::unique_lock<boost::mutex>& lock)
{
	return resource_type_;
}

std::wstring ResourceContainer::GetResourceName() {
    auto lock = AcquireLock();
    return GetResourceName(lock);
}

std::wstring ResourceContainer::GetResourceName(boost::unique_lock<boost::mutex>& lock)
{
	return variation_name_;
}

void ResourceContainer::SetCurrentQuantity(uint32_t current_quantity) {
    auto lock = AcquireLock();
    SetCurrentQuantity(current_quantity, lock);
}

void ResourceContainer::SetCurrentQuantity(uint32_t current_quantity, boost::unique_lock<boost::mutex>& lock)
{
	current_quantity_ = current_quantity;
	DISPATCH(ResourceContainer, CurrentQuantity);
}

void ResourceContainer::SetGlobalResource(uint64_t global_resource) {
    auto lock = AcquireLock();
    SetGlobalResource(global_resource, lock);
}

void ResourceContainer::SetGlobalResource(uint64_t global_resource, boost::unique_lock<boost::mutex>& lock)
{
	global_resource_id_ = global_resource;
	DISPATCH(ResourceContainer, GlobalResourceId);
}

void ResourceContainer::SetMaxQuantity(uint32_t max_quantity) {
    auto lock = AcquireLock();
    SetMaxQuantity(max_quantity, lock);
}

void ResourceContainer::SetMaxQuantity(uint32_t max_quantity, boost::unique_lock<boost::mutex>& lock)
{	
	max_quantity_ = max_quantity;
	DISPATCH(ResourceContainer, MaxQuantity);
}

void ResourceContainer::SetResourceType(const string& resource_type) {
    auto lock = AcquireLock();
    SetResourceType(resource_type, lock);
}

void ResourceContainer::SetResourceType(const string& resource_type, boost::unique_lock<boost::mutex>& lock)
{
	resource_type_ = resource_type;
	DISPATCH(ResourceContainer, ResourceType);
}

void ResourceContainer::SetResourceName(const wstring& name) {
    auto lock = AcquireLock();
    SetResourceName(name, lock);
}

void ResourceContainer::SetResourceName(const wstring& name, boost::unique_lock<boost::mutex>& lock)
{
	variation_name_ = name;
	DISPATCH(ResourceContainer, ResourceName);
}
