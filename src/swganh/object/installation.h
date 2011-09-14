
#ifndef SWGANH_OBJECT_INSTALLATION_H_
#define SWGANH_OBJECT_INSTALLATION_H_

#include <cstdint>
#include <string>
#include <vector>

#include "swganh/object/base_object.h"
#include "swganh/object/base_tangible.h"

namespace swganh {
namespace object {

class Installation : public BaseTangible, public BaseObject
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
    virtual uint32_t GetType() { return Installation::type; }
    const static uint32_t type = 0x49544e4f;

    bool IsActive() const;
    void Activate();
    void Deactivate();
    void ToggleActive();

    float GetPowerReserve() const;
    void ResetPowerReserve(float power_reserve);

    float GetPowerCost() const;
    void ResetPowerCost(float power_cost);

    std::vector<Resource> GetAvailableResources() const;
    void AddAvailableResource(uint64_t global_id, std::string name, std::string type);
    void RemoveAvailableResourceById(uint64_t global_id);
    void UpdateResource(uint64_t global_id, std::string name, std::string type);
    void ResetAvailableResources(std::vector<Resource> available_resource_pool);
    void ClearAllAvailableResources();

    float GetMaxExtractionRate() const;
    void SetMaxExtractionRate(float extraction_rate);

    float GetCurrentExtractionRate() const;
    void SetCurrentExtractionRate(float extraction_rate);

    float GetCurrentHopperSize() const;
    void SetCurrentHopperSize(float hopper_size);

    float GetMaxHopperSize() const;
    void SetMaxHopperSize(float hopper_size);

    bool IsUpdating() const;
    void StartUpdating();
    void StopUpdating();
    void ToggleUpdating();
    
    std::vector<HopperItem> GetHopperContents() const;
    void AddToHopper(uint64_t global_id, float quantity);
    void RemoveHopperItem(uint64_t global_id);
    void UpdateHopperItem(uint64_t global_id, float quantity);
    void ResetContents(std::vector<HopperItem> hopper);
    void ClearAllHopperContents();

    uint8_t GetConditionPercentage() const;
    void SetConditionPercentage(uint8_t condition);

protected:
    virtual void OnReliableUpdate() {}

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

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_TANGIBLE_INSTALLATION_H_
