// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "swganh_core/object/object.h"
#include "swganh_core/object/tangible/tangible.h"

#include "hopper_item.h"
#include "resource.h"

namespace swganh {
namespace object {

class InstallationFactory;
class InstallationMessageBuilder;
class Installation : public swganh::object::Tangible
{
public:
	typedef InstallationFactory FactoryType;
    typedef InstallationMessageBuilder MessageBuilderType;

	Installation();
	virtual ~Installation() {}

    /**
     * @return The type of this object instance.
     */
    uint32_t GetType() const;
    const static uint32_t type = 0x2FB6FB8C;

    /**
     * @return True if the installation is active, false if not.
     */
    bool IsActive() const;
	bool IsActive(boost::unique_lock<boost::mutex>& lock) const;
    
    /**
     * Activates the installation if not active.
     */
    void Activate();
	void Activate(boost::unique_lock<boost::mutex>& lock);

    /**
     * Deactivates the installation if active.
     */
    void Deactivate();
	void Deactivate(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Toggles the active state of the installation.
     */
    void ToggleActive();
	void ToggleActive(boost::unique_lock<boost::mutex>& lock);

    /**
     * @return The amount of power/energy put into the installation.
     */
    float GetPowerReserve() const;
	float GetPowerReserve(boost::unique_lock<boost::mutex>& lock) const;
    
    /**
     * Sets the power reserves for the installation.
     */
    void SetPowerReserve(float power_reserve);
	void SetPowerReserve(float power_reserve, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return This is a decimal value for the amount of power being 
     *  consumed PER HOUR of operation.
     */
    float GetPowerCost() const;
	float GetPowerCost(boost::unique_lock<boost::mutex>& lock) const;

    /**
     * Sets the power consumption rate.
     *
     * @param power_cost The new power consumption rate.
     */
    void SetPowerCost(float power_cost);
	void SetPowerCost(float power_cost, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return A list of available resources for consumption.
     */
    std::vector<Resource> GetAvailableResources();
	std::vector<Resource> GetAvailableResources(boost::unique_lock<boost::mutex>& lock);

    /**
     * Adds an available resource to the installation.
     * 
     * @param global_id The identifier for the resource.
     * @param name The name of the resource.
     * @param type The type/category of the resource.
     */
    void AddAvailableResource(uint64_t global_id, std::string name, std::string type);
	void AddAvailableResource(uint64_t global_id, std::string name, std::string type, boost::unique_lock<boost::mutex>& lock);

    /**
     * Removes available resources by the specied id.
     *
     * @param global_id The resource to remove.
     */
    void RemoveAvailableResourceById(uint64_t global_id);
	void RemoveAvailableResourceById(uint64_t global_id, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Updates an existing resource. Not generally used as resource name/types
     * don't change.
     *
     * @param global_id The identifier for the resource.
     * @param name The name of the resource.
     * @param type The type/category of the resource.
     */
    void UpdateResource(uint64_t global_id, std::string name, std::string type);
	void UpdateResource(uint64_t global_id, std::string name, std::string type, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Clears out the list of available resources.
     */
    void ClearAllAvailableResources();
	void ClearAllAvailableResources(boost::unique_lock<boost::mutex>& lock);

	/**
	* @return the selected resource id
	*/
	uint64_t GetSelectedResourceId();
	uint64_t GetSelectedResourceId(boost::unique_lock<boost::mutex>& lock);

	/**
	* Sets the selected resource id
	*/
	void SetSelectedResourceId(uint64_t new_id);
	void SetSelectedResourceId(uint64_t new_id, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return The max possible extraction rate on this machine.
     */
    float GetMaxExtractionRate() const;
	float GetMaxExtractionRate(boost::unique_lock<boost::mutex>& lock) const;

	/**
     * Sets the maximum possible extraction rate.
     *
     * @param extraction_rate New displayed maximum extraction rate.
     */
    void SetDisplayedMaxExtractionRate(uint32_t extraction_rate);
	void SetDisplayedMaxExtractionRate(uint32_t extraction_rate, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return The current percentage efficiency of extraction that is displayed
     */
    uint32_t GetDisplayedMaxExtractionRate() const;
	uint32_t GetDisplayedMaxExtractionRate(boost::unique_lock<boost::mutex>& lock) const;

    /**
     * Sets the maximum possible extraction rate.
     *
     * @param extraction_rate New maximum extraction rate.
     */
    void SetMaxExtractionRate(float extraction_rate);
	void SetMaxExtractionRate(float extraction_rate, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return The current percentage efficiency of extraction.
     */
    float GetCurrentExtractionRate() const;
	float GetCurrentExtractionRate(boost::unique_lock<boost::mutex>& lock) const;

    /**
     * Sets the current percentage efficiency.
     * 
     * @param extraction_rate The new extraction rate.
     */
    void SetCurrentExtractionRate(float extraction_rate);
	void SetCurrentExtractionRate(float extraction_rate, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return The current amount of resources in the hopper.
     */
    float GetCurrentHopperSize() const;
	float GetCurrentHopperSize(boost::unique_lock<boost::mutex>& lock) const;

    /**
     * Sets the current amount of resources in the hopper.
     * 
     * @param extraction_rate The new extraction rate.
     */
    void SetCurrentHopperSize(float hopper_size);
	void SetCurrentHopperSize(float hopper_size, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return The maximum number of resources that can fit into the hopper.
     */
    uint32_t GetMaxHopperSize() const;
	uint32_t GetMaxHopperSize(boost::unique_lock<boost::mutex>& lock) const;

    /**
     * Sets the maximum number of resources that can fit into the hopper.
     * 
     * @param extraction_rate The new extraction rate.
     */
    void SetMaxHopperSize(uint32_t hopper_size);
	void SetMaxHopperSize(uint32_t hopper_size, boost::unique_lock<boost::mutex>& lock);
        
    /**
     * @return True if the installation is updating, false if not.
     */
    bool IsUpdating() const;
	bool IsUpdating(boost::unique_lock<boost::mutex>& lock) const;
    
    /**
     * Starts updating the installation if not updating.
     */
    void StartUpdating();
	void StartUpdating(boost::unique_lock<boost::mutex>& lock);

    /**
     * Stops pdating the installation if updating.
     */
    void StopUpdating();
	void StopUpdating(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Toggles the updating state of the installation.
     */
    void ToggleUpdating();
	void ToggleUpdating(boost::unique_lock<boost::mutex>& lock);

    /**
     * @return the current hopper contents.
     */
	std::vector<HopperItem> GetHopperContents();
	std::vector<HopperItem> GetHopperContents(boost::unique_lock<boost::mutex>& lock);

    /**
     * Adds a quantity of a resource to the hopper.
     *
     * @param global_id The resoruce to add.
     * @param quantity The quantity of the resource to add.
     */
    void AddToHopper(uint64_t global_id, float quantity);
	void AddToHopper(uint64_t global_id, float quantity, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Removes a resource from the hopper.
     *
     * @param global_id The resource to remove from the hopper.
     */
    void RemoveHopperItem(uint64_t global_id);
	void RemoveHopperItem(uint64_t global_id, boost::unique_lock<boost::mutex>& lock);

    /**
     * Updates the quantity of resources for a type in the hopper.
     *
     * @param global_id The resource type to update.
     * @param quantity The new quantity for the resource type.
     */ 
    void UpdateHopperItem(uint64_t global_id, float quantity);
	void UpdateHopperItem(uint64_t global_id, float quantity, boost::unique_lock<boost::mutex>& lock);

    /**
     * Resets the contents of the hopper.
     *
     * @param hopper The new contents of the hopper.
     */
    void ResetContents(std::vector<HopperItem> hopper);
	void ResetContents(std::vector<HopperItem> hopper, boost::unique_lock<boost::mutex>& lock);

    /**
     * Clears all of the hopper contents.
     */
    void ClearAllHopperContents();
	void ClearAllHopperContents(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return The percentage condition the installation is in.
     */
    uint8_t GetConditionPercentage() const;
	uint8_t GetConditionPercentage(boost::unique_lock<boost::mutex>& lock) const;

    /**
     * Sets the percentage condition the installation is in.
     *
     * @param condition The new condition.
     */
    void SetConditionPercentage(uint8_t condition);
	void SetConditionPercentage(uint8_t condition, boost::unique_lock<boost::mutex>& lock);

	std::vector<uint64_t> GetResourceIds();
	std::vector<uint64_t> GetResourceIds(boost::unique_lock<boost::mutex>& lock);

	std::vector<std::string> GetResourceNames();
	std::vector<std::string> GetResourceNames(boost::unique_lock<boost::mutex>& lock);

	std::vector<std::string> GetResourceTypes();
	std::vector<std::string> GetResourceTypes(boost::unique_lock<boost::mutex>& lock);

	void SerializeResourceIds(swganh::messages::BaseSwgMessage* message);
	void SerializeResourceIds(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

	void SerializeResourceNames(swganh::messages::BaseSwgMessage* message);
	void SerializeResourceNames(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

	void SerializeResourceTypes(swganh::messages::BaseSwgMessage* message);
	void SerializeResourceTypes(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

	void SerializeHopperContents(swganh::messages::BaseSwgMessage* message);
	void SerializeHopperContents(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

private:
	typedef swganh::ValueEvent<std::shared_ptr<Installation>> InstallationEvent;

    bool is_active_;
    float power_reserve_;
    float power_cost_;
    
	swganh::containers::NetworkVector<uint64_t> resource_pool_ids_;
	swganh::containers::NetworkVector<std::string> resource_names_;
	swganh::containers::NetworkVector<std::string> resource_types_;
	
    uint64_t selected_resource_;
    
	uint32_t displayed_max_extraction_rate_;
	float max_extraction_rate_;

    float current_extraction_rate_;
    float current_hopper_size_;
    uint32_t max_hopper_size_;
    
	bool is_updating_;
	swganh::containers::NetworkVector<HopperItem> hopper_;

    uint8_t condition_percent_;
};

}}  // namespace swganh::object
