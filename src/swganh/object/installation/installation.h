
#ifndef SWGANH_OBJECT_INSTALLATION_INSTALLATION_H_
#define SWGANH_OBJECT_INSTALLATION_INSTALLATION_H_

#include <cstdint>
#include <string>
#include <vector>

#include "swganh/object/object.h"
#include "swganh/object/tangible/tangible.h"

namespace swganh {
namespace object {
namespace installation {

class Installation : public swganh::object::tangible::Tangible
{
public:
    struct Resource
    {
        uint64_t global_id;
        std::string name;
        std::string type;
    };

    struct HopperItem
    {
        uint64_t global_id;
        float quantity;
    };

public:
    /**
     * \return The type of this object instance.
     */
    uint32_t GetType() const;
    const static uint32_t type = 0x2FB6FB8C;

    /**
     * \return True if the installation is active, false if not.
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
     * \return The amount of power/energy put into the installation.
     */
    float GetPowerReserve() const;
    
    /**
     * Sets the power reserves for the installation.
     */
    void SetPowerReserve(float power_reserve);

    /**
     * \return This is a decimal value for the amount of power being 
     *  consumed PER HOUR of operation.
     */
    float GetPowerCost() const;

    /**
     * Sets the power consumption rate.
     *
     * \param power_cost The new power consumption rate.
     */
    void SetPowerCost(float power_cost);

    /**
     * \return A list of available resources for consumption.
     */
    std::vector<Resource> GetAvailableResources() const;

    /**
     * Adds an available resource to the installation.
     * 
     * \param global_id The identifier for the resource.
     * \param name The name of the resource.
     * \param type The type/category of the resource.
     */
    void AddAvailableResource(uint64_t global_id, std::string name, std::string type);

    /**
     * Removes available resources by the specied id.
     *
     * \param global_id The resource to remove.
     */
    void RemoveAvailableResourceById(uint64_t global_id);
    
    /**
     * Updates an existing resource. Not generally used as resource name/types
     * don't change.
     *
     * \param global_id The identifier for the resource.
     * \param name The name of the resource.
     * \param type The type/category of the resource.
     */
    void UpdateResource(uint64_t global_id, std::string name, std::string type);
    
    /**
     * Resets the list of available resources.
     *
     * \param available_resource_pool The new resource pool.
     */
    void ResetAvailableResources(std::vector<Resource> available_resource_pool);
    
    /**
     * Clears out the list of available resources.
     */
    void ClearAllAvailableResources();

    /**
     * \return The max possible extraction rate on this machine.
     */
    float GetMaxExtractionRate() const;

    /**
     * Sets the maximum possible extraction rate.
     *
     * \param extraction_rate New maximum extraction rate.
     */
    void SetMaxExtractionRate(float extraction_rate);
    
    /**
     * \return The current percentage efficiency of extraction.
     */
    float GetCurrentExtractionRate() const;

    /**
     * Sets the current percentage efficiency.
     * 
     * \param extraction_rate The new extraction rate.
     */
    void SetCurrentExtractionRate(float extraction_rate);
    
    /**
     * \return The current amount of resources in the hopper.
     */
    float GetCurrentHopperSize() const;

    /**
     * Sets the current amount of resources in the hopper.
     * 
     * \param extraction_rate The new extraction rate.
     */
    void SetCurrentHopperSize(float hopper_size);
    
    /**
     * \return The maximum number of resources that can fit into the hopper.
     */
    float GetMaxHopperSize() const;

    /**
     * Sets the maximum number of resources that can fit into the hopper.
     * 
     * \param extraction_rate The new extraction rate.
     */
    void SetMaxHopperSize(float hopper_size);
        
    /**
     * \return True if the installation is updating, false if not.
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
     * \return the current hopper contents.
     */
    std::vector<HopperItem> GetHopperContents() const;

    /**
     * Adds a quantity of a resource to the hopper.
     *
     * \param global_id The resoruce to add.
     * \param quantity The quantity of the resource to add.
     */
    void AddToHopper(uint64_t global_id, float quantity);
    
    /**
     * Removes a resource from the hopper.
     *
     * \param global_id The resource to remove from the hopper.
     */
    void RemoveHopperItem(uint64_t global_id);

    /**
     * Updates the quantity of resources for a type in the hopper.
     *
     * \param global_id The resource type to update.
     * \param quantity The new quantity for the resource type.
     */ 
    void UpdateHopperItem(uint64_t global_id, float quantity);

    /**
     * Resets the contents of the hopper.
     *
     * \param hopper The new contents of the hopper.
     */
    void ResetContents(std::vector<HopperItem> hopper);

    /**
     * Clears all of the hopper contents.
     */
    void ClearAllHopperContents();
    
    /**
     * \return The percentage condition the installation is in.
     */
    uint8_t GetConditionPercentage() const;

    /**
     * Sets the percentage condition the installation is in.
     *
     * \param condition The new condition.
     */
    void SetConditionPercentage(uint8_t condition);

private:
    bool is_active_;
    float power_reserve_;
    float power_cost_;
    std::vector<Resource> available_resource_pool_;
    uint64_t selected_resource_;
    float max_extraction_rate_;
    float current_extraction_rate_;
    float current_hopper_size_;
    uint32_t max_hopper_size_;
    bool is_updating_;
    std::vector<HopperItem> hopper_;
    uint8_t condition_percent_;
};

}}}  // namespace swganh::object::installation

#endif  // SWGANH_OBJECT_INSTALLATION_INSTALLATION_H_
