
#ifndef SWGANH_OBJECT_MANUFACTURE_SCHEMATIC_MANUFACTURE_SCHEMATIC_H_
#define SWGANH_OBJECT_MANUFACTURE_SCHEMATIC_MANUFACTURE_SCHEMATIC_H_

#include <cstdint>
#include <string>
#include <vector>

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace manufacture_schematic {

class ManufactureSchematic : public swganh::object::Object
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
    // MSCO
    /**
     * @return The type of the object.
     */    
    uint32_t GetType() const;
    const static uint32_t type = 0x4D53434F;

    /** 
     * @return The number of factory runs available on the schematic.
     */
    uint32_t GetSchematicQuantity() const;
    
    /**
     * Sets the number of factory runs available on this schematic from 
     * 1 to 1000.
     *
     * @param quantity the new schematic quantity.
     */
    void SetSchematicQuantity(uint32_t quantity);

    /**
     * Increment the number of factory runs, can be a negative number.
     *
     * @param increment_by The number of runs to increment by.
     */
    void IncrementSchematicQuantity(int32_t increment_by);

    /**
     * @return property list.
     */
    std::vector<Property> GetProperties() const;
    
    /**
     * Adds a property to the properties list.
     *
     * Available properties can be found in the crafting.stf
     *
     * @param property_stf_file
     * @param property_stf_name
     * @param value Value of the property.
     */
    void AddProperty(
        std::string property_stf_file,
        std::string property_stf_name,
        float value);
    
    /**
     * Removes a property from the properties list.
     *
     * @param property_stf_file
     * @param property_stf_name
     * @param value Value of the property.
     */
    void RemoveProperty(
        std::string property_stf_file,
        std::string property_stf_name,
        float value);
    
    /**
     * Updates a property in the properties list.
     *
     * @param property_stf_file
     * @param property_stf_name
     * @param value Value of the property.
     */
    void UpdateProperty(
        std::string property_stf_file,
        std::string property_stf_name,
        float value);

    /**
     * @return the schematic creators name.
     */
    std::wstring GetCreatorName() const;
    void SetCreatorName(std::wstring creator);
    
    /**
     * @return the complexity of the schematic item.
     */
    uint32_t GetSchematicComplexity() const;

    /**
     * Sets the complexity of the schematic item.
     *
     * @param complexity The complexity of the schematic item.
     */
    void SetSchematicComplexity(uint32_t complexity);

    /**
     * @return The volume of the schematic object being created.
     */
    float GetSchematicDataSize() const;

    /**
     * Set the volume of the schematic being created.
     *
     * @param schematic_data_size the new volume.
     */
    void SetSchematicDataSize(float schematic_data_size);

    /**
     * @return the customization string set in the item customization phase.
     */
    std::vector<uint8_t> GetCustomizationString() const;

    /**
     * Sets the customization string set in the item customization phase.
     *
     * @param customization_string The customization string.
     */
    void SetCustomizationString(std::vector<uint8_t> customization_string);

    /**
     * By default is null, gets set via a delta update later in the crafting session.
     *
     * @return the object iff template string of the customization model.
     */
    std::string GetCustomizationModel() const;
    
    /**
     * Sets the customization model.
     *
     * @param customization_model The customization model.
     */
    void SetCustomizationModel(std::string customization_model);

    /**
     * @return the object iff template string of the prototype model.
     */
    std::string GetPrototypeModel() const;
    
    /**
     * @return the object iff template string of the prototype model in crc format.
     */
    uint32_t GetPrototypeCrc() const;
    
    /**
     * Set the object iff template string of the prototype model.
     *
     * @param prototype_model The prototype model.
     */
    void SetPrototypeModel(std::string prototype_model);

    /**
     * @return True for schematics that are being crafted or are being run in an
     *  active factory, false for idle in datapad or data container or inactive 
     *  factory.
     */
    bool IsActive() const;

    /**
     * Activate the schematic.
     */
    void Activate();
    
    /**
     * Deactivate the schematic.
     */
    void Deactive();
    
    /**
     * Toggle active status.
     */
    void ToggleActive();
    
    /**
     * @return The current slot count.
     */
    uint8_t GetSlotCount() const;
    
    /**
     * Increases slot count.
     */
    void IncreaseSlotCount();
    
    /**
     * Decreases slot count.
     */
    void DecreaseSlotCount();
    
    /**
     * Resets slot count.
     *
     * @param slot_count new slot count.
     */
    void ResetSlotCount(uint8_t slot_count);

    /**
     * @return the list of ingredient slots.
     */
    std::vector<Slot> GetSlots() const;

    /**
     * Removes an ingredient slot.
     *
     * @param index The ingredient slot to remove.
     */
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

    /**
     * Resets ingredient slots.
     *
     * @param slots New ingredients.
     */
    void ResetSlots(std::vector<Slot> slots);
    
    /**
     * Clears all ingredient slots.
     */
    void ClearAllSlots();
    
    /**
     * @return experiment list.
     */
    std::vector<Experiment> GetExperiments() const;

    /**
     * Removes an experiment.
     *
     * @param index experiment to remove.
     */
    void RemoveExperiment(uint16_t index);

    /**
     * Adds an experiment.
     *
     * @param experiment_stf_file
     * @param experiment_stf_name
     * @param value
     * @param offset
     * @param size
     * @param max_value
     * @return The index the experiment was added into.
     */
    uint16_t AddExperiment(
        std::string experiment_stf_file, 
        std::string experiment_stf_name,
        float value,
        float offset,
        float size,
        float max_value);

    /**
     * Updates an experiment.
     *
     * @param index
     * @param experiment_stf_file
     * @param experiment_stf_name
     * @param value
     * @param offset
     * @param size
     * @param max_value
     */
    void UpdateExperiment(
        uint16_t index,
        std::string experiment_stf_file, 
        std::string experiment_stf_name,
        float value,
        float offset,
        float size,
        float max_value);

    /**
     * Resets the experiments list.
     *
     * @param experiments The new experiments list.
     */
    void ResetExperiments(std::vector<Experiment> experiments);

    /** 
     * Clears all experiments.
     */
    void ClearAllExperiments();
    
    /**
     * @return customizations list.
     */
    std::vector<Customization> GetCustomizations() const;
    
    /**
     * Removes a customization.
     *
     * @param index customization to remove.
     */
    void RemoveCustomization(uint16_t index);

    /**
     * Adds a customization.
     *
     * @param name
     * @param pallet_selection
     * @param pallet_start_index
     * @param pallet_end_index
     *
     * @return The index the slot was added into.
     */
    uint16_t AddCustomization(
        std::string name,
        uint32_t pallet_selection,
        uint32_t pallet_start_index,
        uint32_t pallet_end_index);
    
    /**
     * Updates a customization.
     *
     * @param index
     * @param name
     * @param pallet_selection
     * @param pallet_start_index
     * @param pallet_end_index
     */
    void UpdateCustomization(
        uint16_t index,
        std::string name,
        uint32_t pallet_selection,
        uint32_t pallet_start_index,
        uint32_t pallet_end_index);
    
    /**
     * Resets the customizations list.
     *
     * @param customizations The new customizations list.
     */
    void ResetCustomizations(std::vector<Customization> customizations);
    
    /** 
     * Clears all customizations.
     */
    void ClearAllCustomizations();

    /**
     * @return ready state of the schematic.
     *
     * @TODO what is this?
     */
    bool IsReady() const;

    /**
     * Toggles ready state.
     */
    void ToggleReady();

private:
    void BuildSlotDelta_(uint8_t update_type, uint8_t sub_type, std::vector<Slot>::iterator slot_iterator);
    uint32_t schematic_quantity_;
    uint32_t property_counter_;
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
    uint32_t slot_counter_;
    uint32_t ingredients_counter_;
    std::vector<Experiment> experiments_;
    uint32_t experiment_counter_;
    std::vector<Customization> customizations_;
    uint32_t customization_counter_;
    float risk_factor_;
    bool is_ready_;
};

}}}  // namespace swganh::object::manufacture_schematic

#endif  // SWGANH_OBJECT_MANUFACTURE_SCHEMATIC_H_
