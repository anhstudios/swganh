
#ifndef SWGANH_OBJECT_MANUFACTURE_SCHEMATIC_H_
#define SWGANH_OBJECT_MANUFACTURE_SCHEMATIC_H_

#include <cstdint>
#include <string>
#include <vector>

#include "swganh/object/base_object.h"

namespace swganh {
namespace object {

class ManufactureSchematic : public BaseObject
{
public:
    struct Property
    {
        std::string property_stf_file;
        std::string property_stf_name;
        float value;
    };

    struct Slot 
    {
        uint32_t index;
        std::string slot_stf_file;
        std::string slot_stf_name;
        uint32_t type;
        uint64_t ingredient;
        uint32_t ingredient_quantity;
        uint32_t clean;
    };

    struct Experiment
    {
        uint16_t index;
        std::string experiment_stf_file; 
        std::string experiment_stf_name;
        float value;
        float offset;
        float size;
        float max_value;
    };

    struct Customization
    {
        uint16_t index;
        std::string name;
        uint32_t pallet_selection;
        uint32_t pallet_start_index;
        uint32_t pallet_end_index;
    };

public:

    uint32_t GetSchematicQuantity() const;
    void ResetSchematicQuantity(uint32_t quantity);
    void IncreaseSchematicQuantity(uint32_t increase_by);

    std::vector<Property> GetProperties() const;
    void AddProperty(
        std::string property_stf_file,
        std::string property_stf_name,
        float value);
    void RemoveProperty(
        std::string property_stf_file,
        std::string property_stf_name,
        float value);
    void UpdateProperty(
        std::string property_stf_file,
        std::string property_stf_name,
        float value);

    std::wstring GetCreatorName() const;
    void SetCreatorName(std::wstring creator);

    uint32_t GetComplexity() const;
    void SetComplexity(uint32_t complexity);

    float GetSchematicDataSize() const;
    void SetSchematicDataSize(float schematic_data_size);

    std::vector<uint8_t> GetCustomizationString() const;
    void SetCustomizationString(std::vector<uint8_t> customization_string);

    std::string GetCustomizationModel() const;
    void GetCustomizationModel(std::string customization_model);

    std::string GetPrototypeModel() const;
    uint32_t GetPrototypeCrc() const;
    void SetPrototypeModel(std::string prototype_model);

    bool IsActive() const;
    void Activate();
    void Deactive();
    void ToggleActive();
    
    uint8_t GetSlotCount() const;
    void IncreaseSlotCount();
    void DecreaseSlotCount();
    void ResetSlotCount(uint8_t slot_count);

    std::vector<Slot> GetSlots() const;
    void RemoveSlot(uint16_t index);
    /**
     * @return The index the slot was added into.
     */
    uint16_t AddSlot(
        std::string slot_stf_file,
        std::string slot_stf_name,
        uint32_t type,
        uint64_t ingredient,
        uint32_t ingredient_quantity,
        uint32_t clean);
    void UpdateSlot(
        uint16_t index,
        std::string slot_stf_file,
        std::string slot_stf_name,
        uint32_t type,
        uint64_t ingredient,
        uint32_t ingredient_quantity,
        uint32_t clean);
    void ResetSlots(std::vector<Slot> slots);
    void ClearAllSlots();
    
    std::vector<Experiment> GetExperiments() const;
    void RemoveExperiment(uint16_t index);
    /**
     * @return The index the experiment was added into.
     */
    uint16_t AddExperiment(
        std::string experiment_stf_file, 
        std::string experiment_stf_name,
        float value,
        float offset,
        float size,
        float max_value);
    void UpdateExperiment(
        uint16_t index,
        std::string experiment_stf_file, 
        std::string experiment_stf_name,
        float value,
        float offset,
        float size,
        float max_value);
    void ResetExperiments(std::vector<Experiment> experiments);
    void ClearAllExperiments();
    
    std::vector<Customization> GetCustomizations() const;
    void RemoveCustomization(uint16_t index);
    /**
     * @return The index the slot was added into.
     */
    uint16_t AddCustomization(
        std::string name,
        uint32_t pallet_selection,
        uint32_t pallet_start_index,
        uint32_t pallet_end_index);
    void UpdateCustomization(
        uint16_t index,
        std::string name,
        uint32_t pallet_selection,
        uint32_t pallet_start_index,
        uint32_t pallet_end_index);
    void ResetCustomizations(std::vector<Customization> customizations);
    void ClearAllCustomizations();

    bool IsReady() const;
    void ToggleReady();

private:
    uint32_t schematic_quantity_;
    std::vector<Property> properties_;
    std::wstring creator_;
    uint32_t complexity_;
    float schematic_data_size_;
    std::vector<uint8_t> customization_;
    std::string customization_model_;
    std::string prototype_model_;
    bool is_active_;
    uint8_t slot_count_;
    std::vector<Slot> slots_;
    std::vector<Experiment> experiments_;
    std::vector<Customization> customizations_;
    float risk_factor_;
    bool is_ready_;
};

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_MANUFACTURE_SCHEMATIC_H_
