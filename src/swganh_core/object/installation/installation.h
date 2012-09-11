// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "swganh_core/object/object.h"
#include "swganh_core/object/tangible/tangible.h"

namespace swganh {
namespace object {


struct ResourceId
{
    ResourceId(void)
        : id(0)
    {}

    ResourceId(const uint64_t& value_)
        : id(value_)
    {}

    ~ResourceId()
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint64_t>(id);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint64_t>(id);
    }
    bool operator==(const ResourceId& other)
    {
        return id == other.id;
    }

    uint64_t id;
};

struct ResourceString
{
    ResourceString(void)
        : name("")
    {}

    ResourceString(const std::string& value_)
        : name(value_)
    {}

    ~ResourceString()
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<std::string>(name);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<std::string>(name);
    }
    bool Contains(const std::string& str) const
    {
        return name.find(str) != std::string::npos;
    }
    bool operator==(const ResourceString& other)
    {
        return name == other.name;
    }

    std::string name;
};


class InstallationFactory;
class InstallationMessageBuilder;
class Installation : public swganh::object::Tangible
{
public:
	typedef InstallationFactory FactoryType;
    typedef InstallationMessageBuilder MessageBuilderType;

	struct Resource
	{
		uint64_t global_id;
		std::string resource_name;
		std::string resource_type;
	};

    struct HopperItem
    {
        uint64_t global_id;
        float quantity;

		void Serialize(swganh::messages::BaselinesMessage& message)
		{
			message.data.write(global_id);
			message.data.write(quantity);
		}

		void Serialize(swganh::messages::DeltasMessage& message)
		{
			message.data.write(global_id);
			message.data.write(quantity);
		}

		bool operator==(const HopperItem& other)
		{
			return other.global_id == global_id;
		}
    };

	Installation();

    /**
     * @return The type of this object instance.
     */
    uint32_t GetType() const;
    const static uint32_t type = 0x2FB6FB8C;

    /**
     * @return True if the installation is active, false if not.
     */
    bool IsActive() const;
    
    /**
     * Activates the installation if not active.
     */
    void Activate();

    /**
     * Deactivates the installation if active.
     */
    void Deactivate();
    
    /**
     * Toggles the active state of the installation.
     */
    void ToggleActive();

    /**
     * @return The amount of power/energy put into the installation.
     */
    float GetPowerReserve() const;
    
    /**
     * Sets the power reserves for the installation.
     */
    void SetPowerReserve(float power_reserve);

    /**
     * @return This is a decimal value for the amount of power being 
     *  consumed PER HOUR of operation.
     */
    float GetPowerCost() const;

    /**
     * Sets the power consumption rate.
     *
     * @param power_cost The new power consumption rate.
     */
    void SetPowerCost(float power_cost);

    /**
     * @return A list of available resources for consumption.
     */
    std::vector<Resource> GetAvailableResources();

    /**
     * Adds an available resource to the installation.
     * 
     * @param global_id The identifier for the resource.
     * @param name The name of the resource.
     * @param type The type/category of the resource.
     */
    void AddAvailableResource(uint64_t global_id, std::string name, std::string type);

    /**
     * Removes available resources by the specied id.
     *
     * @param global_id The resource to remove.
     */
    void RemoveAvailableResourceById(uint64_t global_id);
    
    /**
     * Updates an existing resource. Not generally used as resource name/types
     * don't change.
     *
     * @param global_id The identifier for the resource.
     * @param name The name of the resource.
     * @param type The type/category of the resource.
     */
    void UpdateResource(uint64_t global_id, std::string name, std::string type);
    
    /**
     * Resets the list of available resources.
     *
     * @param available_resource_pool The new resource pool.
     */
    void ResetAvailableResources(std::vector<Resource> available_resource_pool);
    
    /**
     * Clears out the list of available resources.
     */
    void ClearAllAvailableResources();

	/**
	* @return the selected resource id
	*/
	uint64_t GetSelectedResourceId();

	/**
	* Sets the selected resource id
	*/
	void SetSelectedResourceId(uint64_t new_id);

    /**
     * @return The max possible extraction rate on this machine.
     */
    float GetMaxExtractionRate() const;

	/**
     * Sets the maximum possible extraction rate.
     *
     * @param extraction_rate New displayed maximum extraction rate.
     */
    void SetDisplayedMaxExtractionRate(uint32_t extraction_rate);
    
    /**
     * @return The current percentage efficiency of extraction that is displayed
     */
    uint32_t GetDisplayedMaxExtractionRate() const;

    /**
     * Sets the maximum possible extraction rate.
     *
     * @param extraction_rate New maximum extraction rate.
     */
    void SetMaxExtractionRate(float extraction_rate);
    
    /**
     * @return The current percentage efficiency of extraction.
     */
    float GetCurrentExtractionRate() const;

    /**
     * Sets the current percentage efficiency.
     * 
     * @param extraction_rate The new extraction rate.
     */
    void SetCurrentExtractionRate(float extraction_rate);
    
    /**
     * @return The current amount of resources in the hopper.
     */
    float GetCurrentHopperSize() const;

    /**
     * Sets the current amount of resources in the hopper.
     * 
     * @param extraction_rate The new extraction rate.
     */
    void SetCurrentHopperSize(float hopper_size);
    
    /**
     * @return The maximum number of resources that can fit into the hopper.
     */
    uint32_t GetMaxHopperSize() const;

    /**
     * Sets the maximum number of resources that can fit into the hopper.
     * 
     * @param extraction_rate The new extraction rate.
     */
    void SetMaxHopperSize(uint32_t hopper_size);
        
    /**
     * @return True if the installation is updating, false if not.
     */
    bool IsUpdating() const;
    
    /**
     * Starts updating the installation if not updating.
     */
    void StartUpdating();

    /**
     * Stops pdating the installation if updating.
     */
    void StopUpdating();
    
    /**
     * Toggles the updating state of the installation.
     */
    void ToggleUpdating();

    /**
     * @return the current hopper contents.
     */
	swganh::messages::containers::NetworkSortedVector<HopperItem> GetHopperContents() const;

    /**
     * Adds a quantity of a resource to the hopper.
     *
     * @param global_id The resoruce to add.
     * @param quantity The quantity of the resource to add.
     */
    void AddToHopper(uint64_t global_id, float quantity);
    
    /**
     * Removes a resource from the hopper.
     *
     * @param global_id The resource to remove from the hopper.
     */
    void RemoveHopperItem(uint64_t global_id);

    /**
     * Updates the quantity of resources for a type in the hopper.
     *
     * @param global_id The resource type to update.
     * @param quantity The new quantity for the resource type.
     */ 
    void UpdateHopperItem(uint64_t global_id, float quantity);

    /**
     * Resets the contents of the hopper.
     *
     * @param hopper The new contents of the hopper.
     */
    void ResetContents(std::vector<HopperItem> hopper);

    /**
     * Clears all of the hopper contents.
     */
    void ClearAllHopperContents();
    
    /**
     * @return The percentage condition the installation is in.
     */
    uint8_t GetConditionPercentage() const;

    /**
     * Sets the percentage condition the installation is in.
     *
     * @param condition The new condition.
     */
    void SetConditionPercentage(uint8_t condition);

	swganh::messages::containers::NetworkSortedVector<ResourceId> GetResourceIds_();
	swganh::messages::containers::NetworkSortedVector<ResourceString> GetResourceNames_();
	swganh::messages::containers::NetworkSortedVector<ResourceString> GetResourceTypes_();

private:
	typedef swganh::ValueEvent<std::shared_ptr<Installation>> InstallationEvent;

    std::atomic<bool> is_active_;
    float power_reserve_;
    float power_cost_;
    
	swganh::messages::containers::NetworkSortedVector<ResourceId> resource_pool_ids_;
	swganh::messages::containers::NetworkSortedVector<ResourceString> resource_names_;
	swganh::messages::containers::NetworkSortedVector<ResourceString> resource_types_;
	
    std::atomic<uint64_t> selected_resource_;
    
	std::atomic<uint32_t> displayed_max_extraction_rate_;
	float max_extraction_rate_;

    float current_extraction_rate_;
    float current_hopper_size_;
    std::atomic<uint32_t> max_hopper_size_;
    
	std::atomic<bool> is_updating_;
	swganh::messages::containers::NetworkSortedVector<HopperItem> hopper_;

    std::atomic<uint8_t> condition_percent_;
};

}}  // namespace swganh::object
