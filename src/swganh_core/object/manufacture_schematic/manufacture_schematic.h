// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>

#include "swganh_core/object/intangible/intangible.h"

#include "swganh_core/messages/containers/network_vector.h"
#include "swganh_core/messages/containers/network_map.h"

#include "property.h"
#include "slot.h"
#include "customization.h"
#include "experiment.h"

namespace swganh {
namespace object {

class ManufactureSchematicFactory;
class ManufactureSchematicMessageBuilder;
class ManufactureSchematic : public swganh::object::Intangible
{
public:
	typedef ManufactureSchematicFactory FactoryType;
	typedef ManufactureSchematicMessageBuilder MessageBuilderType;

	ManufactureSchematic();
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
	uint32_t GetSchematicQuantity(boost::unique_lock<boost::mutex>& lock) const;
    
    /**
     * Sets the number of factory runs available on this schematic from 
     * 1 to 1000.
     *
     * @param quantity the new schematic quantity.
     */
    void SetSchematicQuantity(uint32_t quantity);
	void SetSchematicQuantity(uint32_t quantity, boost::unique_lock<boost::mutex>& lock);

    /**
     * Increment the number of factory runs, can be a negative number.
     *
     * @param increment_by The number of runs to increment by.
     */
    void IncrementSchematicQuantity(int32_t increment_by);
	void IncrementSchematicQuantity(int32_t increment_by, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return property list.
     */
	std::vector<Property> GetProperties() const;
    
	void SerializeProperties(swganh::messages::BaseSwgMessage* message);
	void SerializeProperties(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    /**
     * Adds a property to the properties list.
     *
     * Available properties can be found in the crafting.stf
     *
     * @param property_stf_file
     * @param property_stf_name
     * @param value Value of the property.
     */
    void AddProperty(std::string property_stf_file,std::string property_stf_name, float value);
	void AddProperty(std::string property_stf_file,std::string property_stf_name, float value, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Removes a property from the properties list.
     *
     * @param property_stf_file
     * @param property_stf_name
     * @param value Value of the property.
     */
    void RemoveProperty(std::string property_stf_file,std::string property_stf_name,float value);
	void RemoveProperty(std::string property_stf_file,std::string property_stf_name,float value, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Updates a property in the properties list.
     *
     * @param property_stf_file
     * @param property_stf_name
     * @param value Value of the property.
     */
    void UpdateProperty(std::string property_stf_file,std::string property_stf_name,float value);
	void UpdateProperty(std::string property_stf_file,std::string property_stf_name,float value, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return the schematic creators name.
     */
    std::wstring GetCreatorName() const;
	std::wstring GetCreatorName(boost::unique_lock<boost::mutex>& lock) const;

    void SetCreatorName(std::wstring creator);
	void SetCreatorName(std::wstring creator, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return the complexity of the schematic item.
     */
    uint32_t GetSchematicComplexity() const;
	uint32_t GetSchematicComplexity(boost::unique_lock<boost::mutex>& lock) const;

    /**
     * Sets the complexity of the schematic item.
     *
     * @param complexity The complexity of the schematic item.
     */
    void SetSchematicComplexity(uint32_t complexity);
	void SetSchematicComplexity(uint32_t complexity, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return The volume of the schematic object being created.
     */
    float GetSchematicDataSize() const;
	float GetSchematicDataSize(boost::unique_lock<boost::mutex>& lock) const;

    /**
     * Set the volume of the schematic being created.
     *
     * @param schematic_data_size the new volume.
     */
    void SetSchematicDataSize(float schematic_data_size);
	void SetSchematicDataSize(float schematic_data_size, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return the customization string set in the item customization phase.
     */
    std::string GetCustomizationString() const;
	std::string GetCustomizationString(boost::unique_lock<boost::mutex>& lock) const;

    /**
     * Sets the customization string set in the item customization phase.
     *
     * @param Customization string
     */
	void SetCustomizationString(const std::string& customization_string);
    void SetCustomizationString(const std::string& customization_string, boost::unique_lock<boost::mutex>& lock);

    /**
     * By default is null, gets set via a delta update later in the crafting session.
     *
     * @return the object iff template string of the customization model.
     */
    std::string GetCustomizationModel() const;
	std::string GetCustomizationModel(boost::unique_lock<boost::mutex>& lock) const;
    
    /**
     * Sets the customization model.
     *
     * @param customization_model The customization model.
     */
    void SetCustomizationModel(std::string customization_model);
	void SetCustomizationModel(std::string customization_model, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return the object iff template string of the prototype model.
     */
    std::string GetPrototypeModel() const;
	std::string GetPrototypeModel(boost::unique_lock<boost::mutex>& lock) const;
    
    /**
     * @return the object iff template string of the prototype model in crc format.
     */
    uint32_t GetPrototypeCrc() const;
	uint32_t GetPrototypeCrc(boost::unique_lock<boost::mutex>& lock) const;
    
    /**
     * Set the object iff template string of the prototype model.
     *
     * @param prototype_model The prototype model.
     */
    void SetPrototypeModel(std::string prototype_model);
	void SetPrototypeModel(std::string prototype_model, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return True for schematics that are being crafted or are being run in an
     *  active factory, false for idle in datapad or data container or inactive 
     *  factory.
     */
    bool IsActive() const;
	bool IsActive(boost::unique_lock<boost::mutex>& lock) const;

    /**
     * Activate the schematic.
     */
    void Activate();
	void Activate(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Deactivate the schematic.
     */
    void Deactive();
	void Deactive(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Toggle active status.
     */
    void ToggleActive();
	void ToggleActive(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return The current slot count.
     */
    uint8_t GetSlotCount() const;
	uint8_t GetSlotCount(boost::unique_lock<boost::mutex>& lock) const;
    
    /**
     * Increases slot count.
     */
    void IncreaseSlotCount();
	void IncreaseSlotCount(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Decreases slot count.
     */
    void DecreaseSlotCount();
	void DecreaseSlotCount(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Resets slot count.
     *
     * @param slot_count new slot count.
     */
    void ResetSlotCount(uint8_t slot_count);
	void ResetSlotCount(uint8_t slot_count, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return the list of ingredient slots.
     */
    std::vector<Slot> GetSlots();
	std::vector<Slot> GetSlots(boost::unique_lock<boost::mutex>& lock);

	void SerializeSlots(swganh::messages::BaseSwgMessage* message);
	void SerializeSlots(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    /**
     * Removes an ingredient slot.
     *
     * @param index The ingredient slot to remove.
     */
    void RemoveSlot(uint16_t index);
	void RemoveSlot(uint16_t index, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return The index the slot was added into.
     */
    uint16_t AddSlot(std::string slot_stf_file, std::string slot_stf_name, uint32_t type, 
		uint64_t ingredient, uint32_t ingredient_quantity, uint32_t clean);
	uint16_t AddSlot(std::string slot_stf_file, std::string slot_stf_name, uint32_t type, 
		uint64_t ingredient, uint32_t ingredient_quantity, uint32_t clean, boost::unique_lock<boost::mutex>& lock);

	uint16_t AddSlot(Slot slot);
	uint16_t AddSlot(Slot slot, boost::unique_lock<boost::mutex>& lock);

    void UpdateSlot(
        uint16_t index,
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
        uint32_t clean, boost::unique_lock<boost::mutex>& lock);

    /**
     * Resets ingredient slots.
     *
     * @param slots New ingredients.
     */
    void ResetSlots(std::vector<Slot> slots);
	void ResetSlots(std::vector<Slot> slots, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Clears all ingredient slots.
     */
    void ClearAllSlots();
	void ClearAllSlots(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return experiment list.
     */
    std::vector<Experiment> GetExperiments();
	std::vector<Experiment> GetExperiments(boost::unique_lock<boost::mutex>& lock);

	void SerializeExperiments(swganh::messages::BaseSwgMessage* message);
	void SerializeExperiments(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    /**
     * Removes an experiment.
     *
     * @param index experiment to remove.
     */
    void RemoveExperiment(uint16_t index);
	void RemoveExperiment(uint16_t index, boost::unique_lock<boost::mutex>& lock);

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

	uint16_t AddExperiment(
        std::string experiment_stf_file, 
        std::string experiment_stf_name,
        float value,
        float offset,
        float size,
        float max_value, boost::unique_lock<boost::mutex>& lock);

	uint16_t AddExperiment(Experiment experiment);
	uint16_t AddExperiment(Experiment experiment, boost::unique_lock<boost::mutex>& lock);

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

	void UpdateExperiment(
        uint16_t index,
        std::string experiment_stf_file, 
        std::string experiment_stf_name,
        float value,
        float offset,
        float size,
        float max_value, boost::unique_lock<boost::mutex>& lock);

    /**
     * Resets the experiments list.
     *
     * @param experiments The new experiments list.
     */
    void ResetExperiments(std::vector<Experiment> experiments);
	void ResetExperiments(std::vector<Experiment> experiments, boost::unique_lock<boost::mutex>& lock);

    /** 
     * Clears all experiments.
     */
    void ClearAllExperiments();
	void ClearAllExperiments(boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return customizations list.
     */
    std::vector<Customization> GetCustomizations();
	std::vector<Customization> GetCustomizations(boost::unique_lock<boost::mutex>& lock);
    
	void SerializeCustomizations(swganh::messages::BaseSwgMessage* message);
	void SerializeCustomizations(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    /**
     * Removes a customization.
     *
     * @param index customization to remove.
     */
    void RemoveCustomization(uint16_t index);
	void RemoveCustomization(uint16_t index, boost::unique_lock<boost::mutex>& lock);

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

	uint16_t AddCustomization(
        std::string name,
        uint32_t pallet_selection,
        uint32_t pallet_start_index,
        uint32_t pallet_end_index, boost::unique_lock<boost::mutex>& lock);
    
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

	void UpdateCustomization(
        uint16_t index,
        std::string name,
        uint32_t pallet_selection,
        uint32_t pallet_start_index,
        uint32_t pallet_end_index, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * Resets the customizations list.
     *
     * @param customizations The new customizations list.
     */
    void ResetCustomizations(std::vector<Customization> customizations);
	void ResetCustomizations(std::vector<Customization> customizations, boost::unique_lock<boost::mutex>& lock);
    
    /** 
     * Clears all customizations.
     */
    void ClearAllCustomizations();
	void ClearAllCustomizations(boost::unique_lock<boost::mutex>& lock);

    /**
     * @return ready state of the schematic.
     *
     * @TODO what is this?
     */
    bool IsReady() const;
	bool IsReady(boost::unique_lock<boost::mutex>& lock) const;

    /**
     * Toggles ready state.
     */
    void ToggleReady();
	void ToggleReady(boost::unique_lock<boost::mutex>& lock);

	typedef swganh::ValueEvent<std::shared_ptr<ManufactureSchematic>> ManufactureSchematicEvent;

private:
	swganh::containers::NetworkMap<std::string, Property> properties_;
    std::wstring creator_;
    uint32_t complexity_;
    float schematic_data_size_;
    std::string customization_;
    std::string customization_model_;
    std::string prototype_model_;
    bool is_active_;
    uint8_t slot_count_;
    
	swganh::containers::NetworkVector<Slot> slots_;
    swganh::containers::NetworkVector<Experiment> experiments_;
    swganh::containers::NetworkVector<Customization> customizations_;
    float risk_factor_;
    bool is_ready_;
};

}}  // namespace swganh::object
