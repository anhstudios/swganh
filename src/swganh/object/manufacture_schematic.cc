
#include "swganh/object/manufacture_schematic.h"

#include "anh/crc.h"

using namespace std;
using namespace swganh::object;

uint32_t ManufactureSchematic::GetSchematicQuantity() const
{
    return schematic_quantity_;
}

void ManufactureSchematic::ResetSchematicQuantity(uint32_t quantity)
{
    schematic_quantity_ = (quantity > 1000) ? 1000 : quantity;
}

void ManufactureSchematic::IncreaseSchematicQuantity(uint32_t increase_by)
{
    uint32_t tmp = schematic_quantity_ + increase_by;
    schematic_quantity_ = (tmp > 1000) ? 1000 : tmp;
}

std::vector<ManufactureSchematic::Property> ManufactureSchematic::GetProperties() const
{
    return properties_;
}

void ManufactureSchematic::AddProperty(
    std::string property_stf_file,
    std::string property_stf_name,
    float value)
{
    auto find_iter = find_if(
        properties_.begin(),
        properties_.end(),
        [&property_stf_name] (const Property& stored_property)
    {
        return stored_property.property_stf_name.compare(property_stf_name) == 0;
    });

    if (find_iter != properties_.end())
    {
        // Already in the list.
        return;
    }

    Property new_property;
    new_property.property_stf_file = move(property_stf_file);
    new_property.property_stf_name = move(property_stf_name);
    new_property.value = value;

    properties_.push_back(move(new_property));
}

void ManufactureSchematic::RemoveProperty(
    std::string property_stf_file,
    std::string property_stf_name,
    float value)
{
    auto find_iter = find_if(
        properties_.begin(),
        properties_.end(),
        [&property_stf_name] (const Property& stored_property)
    {
        return stored_property.property_stf_name.compare(property_stf_name) == 0;
    });

    if (find_iter == properties_.end())
    {
        // Not in the list.
        return;
    }

    properties_.erase(find_iter);
}

void ManufactureSchematic::UpdateProperty(
    std::string property_stf_file,
    std::string property_stf_name,
    float value)
{
    auto find_iter = find_if(
        properties_.begin(),
        properties_.end(),
        [&property_stf_name] (const Property& stored_property)
    {
        return stored_property.property_stf_name.compare(property_stf_name) == 0;
    });

    if (find_iter == properties_.end())
    {
        // Not in the list.
        return;
    }

    find_iter->property_stf_file = move(property_stf_file);
    find_iter->property_stf_name = move(property_stf_name);
    find_iter->value = value;
}

std::wstring ManufactureSchematic::GetCreatorName() const
{
    return creator_;
}

void ManufactureSchematic::SetCreatorName(std::wstring creator)
{
    creator_ = move(creator);
}

uint32_t ManufactureSchematic::GetComplexity() const
{
    return complexity_;
}

void ManufactureSchematic::SetComplexity(uint32_t complexity)
{
    complexity_ = complexity;
}

float ManufactureSchematic::GetSchematicDataSize() const
{
    return schematic_data_size_;
}

void ManufactureSchematic::SetSchematicDataSize(float schematic_data_size)
{
    schematic_data_size_ = schematic_data_size;
}

std::vector<uint8_t> ManufactureSchematic::GetCustomizationString() const
{
    return customization_;
}

void ManufactureSchematic::SetCustomizationString(std::vector<uint8_t> customization_string)
{
    customization_ = move(customization_string);
}

std::string ManufactureSchematic::GetCustomizationModel() const
{
    return customization_model_;
}

void ManufactureSchematic::GetCustomizationModel(std::string customization_model)
{
    customization_model_ = customization_model;
}

std::string ManufactureSchematic::GetPrototypeModel() const
{
    return prototype_model_;
}

uint32_t ManufactureSchematic::GetPrototypeCrc() const
{
    return anh::memcrc(prototype_model_);
}

void ManufactureSchematic::SetPrototypeModel(std::string prototype_model)
{
    prototype_model_ = move(prototype_model);
}

bool ManufactureSchematic::IsActive() const
{
    return is_active_;
}

void ManufactureSchematic::Activate()
{
    if (!IsActive())
    {
        ToggleActive();
    }
}

void ManufactureSchematic::Deactive()
{
    if (IsActive())
    {
        ToggleActive();
    }
}

void ManufactureSchematic::ToggleActive()
{
    is_active_ = !is_active_;
}

uint8_t ManufactureSchematic::GetSlotCount() const
{
    return slot_count_;
}

void ManufactureSchematic::IncreaseSlotCount()
{
    ++slot_count_;
}

void ManufactureSchematic::DecreaseSlotCount()
{
    --slot_count_;
}

void ManufactureSchematic::ResetSlotCount(uint8_t slot_count)
{
    slot_count_ = slot_count;
}

std::vector<ManufactureSchematic::Slot> ManufactureSchematic::GetSlots() const
{
    return slots_;
}

void ManufactureSchematic::RemoveSlot(uint16_t index)
{
    auto find_iter = find_if(
        slots_.begin(),
        slots_.end(),
        [index] (const Slot& slot)
    {
        return index == slot.index;
    });

    if (find_iter == slots_.end())
    {
        // Not in the list.
        return;
    }

    slots_.erase(find_iter);
}

/**
 * @return The index the slot was added into.
 */
uint16_t ManufactureSchematic::AddSlot(
    std::string slot_stf_file,
    std::string slot_stf_name,
    uint32_t type,
    uint64_t ingredient,
    uint32_t ingredient_quantity,
    uint32_t clean)
{
    auto find_iter = find_if(
        slots_.begin(),
        slots_.end(),
        [&slot_stf_name] (const Slot& slot)
    {
        return slot.slot_stf_name.compare(slot_stf_name) == 0;
    });

    if (find_iter != slots_.end())
    {
        // Already in the list.
        return 0;
    }

    Slot slot;    
    slot.index = customizations_.back().index + 1;
    slot.slot_stf_file = move(slot_stf_file);
    slot.slot_stf_name = move(slot_stf_name);
    slot.type = type;
    slot.ingredient = ingredient;
    slot.ingredient_quantity = ingredient_quantity;
    slot.clean = clean;

    slots_.push_back(slot);

    return slot.index;
}

void ManufactureSchematic::UpdateSlot(
    uint16_t index,
    std::string slot_stf_file,
    std::string slot_stf_name,
    uint32_t type,
    uint64_t ingredient,
    uint32_t ingredient_quantity,
    uint32_t clean)
{
    auto find_iter = find_if(
        slots_.begin(),
        slots_.end(),
        [index] (const Slot& slot)
    {
        return index == slot.index;
    });

    if (find_iter == slots_.end())
    {
        // Not in the list.
        return;
    }
    
    find_iter->slot_stf_file = move(slot_stf_file);
    find_iter->slot_stf_name = move(slot_stf_name);
    find_iter->type = type;
    find_iter->ingredient = ingredient;
    find_iter->ingredient_quantity = ingredient_quantity;
    find_iter->clean = clean;
}

void ManufactureSchematic::ResetSlots(std::vector<ManufactureSchematic::Slot> slots)
{
    slots_ = move(slots);
}

void ManufactureSchematic::ClearAllSlots()
{
    slots_.clear();
}

std::vector<ManufactureSchematic::Experiment> ManufactureSchematic::GetExperiments() const
{
    return experiments_;
}

void ManufactureSchematic::RemoveExperiment(uint16_t index)
{
    auto find_iter = find_if(
        experiments_.begin(),
        experiments_.end(),
        [index] (const Experiment& experiment)
    {
        return index == experiment.index;
    });

    if (find_iter == experiments_.end())
    {
        // Not in the list.
        return;
    }

    experiments_.erase(find_iter);
}

/**
 * @return The index the experiment was added into.
 */
uint16_t ManufactureSchematic::AddExperiment(
    std::string experiment_stf_file, 
    std::string experiment_stf_name,
    float value,
    float offset,
    float size,
    float max_value)
{
    auto find_iter = find_if(
        experiments_.begin(),
        experiments_.end(),
        [&experiment_stf_name] (const Experiment& experiment)
    {
        return experiment.experiment_stf_name.compare(experiment_stf_name) == 0;
    });

    if (find_iter != experiments_.end())
    {
        // Already in the list.
        return 0;
    }

    Experiment experiment;    
    experiment.index = customizations_.back().index + 1;
    experiment.experiment_stf_file = move(experiment_stf_file);
    experiment.experiment_stf_name = move(experiment_stf_name);
    experiment.value = value;
    experiment.offset = offset;
    experiment.size = size;
    experiment.max_value = max_value;

    experiments_.push_back(experiment);

    return experiment.index;
}

void ManufactureSchematic::UpdateExperiment(
    uint16_t index,
    std::string experiment_stf_file, 
    std::string experiment_stf_name,
    float value,
    float offset,
    float size,
    float max_value)
{
    auto find_iter = find_if(
        experiments_.begin(),
        experiments_.end(),
        [index] (const Experiment& experiment)
    {
        return index == experiment.index;
    });

    if (find_iter == experiments_.end())
    {
        // Not in the list.
        return;
    }

    find_iter->experiment_stf_file = move(experiment_stf_file);
    find_iter->experiment_stf_name = move(experiment_stf_name);
    find_iter->value = value;
    find_iter->offset = offset;
    find_iter->size = size;
    find_iter->max_value = max_value;
}

void ManufactureSchematic::ResetExperiments(std::vector<ManufactureSchematic::Experiment> experiments)
{
    experiments_ = move(experiments);
}

void ManufactureSchematic::ClearAllExperiments()
{
    experiments_.clear();
}

std::vector<ManufactureSchematic::Customization> ManufactureSchematic::GetCustomizations() const
{
    return customizations_;
}

void ManufactureSchematic::RemoveCustomization(uint16_t index)
{
    auto find_iter = find_if(
        customizations_.begin(),
        customizations_.end(),
        [index] (const Customization& customization)
    {
        return index == customization.index;
    });

    if (find_iter == customizations_.end())
    {
        // Not in the list.
        return;
    }

    customizations_.erase(find_iter);
}

/**
 * @return The index the slot was added into.
 */
uint16_t ManufactureSchematic::AddCustomization(
    std::string name,
    uint32_t pallet_selection,
    uint32_t pallet_start_index,
    uint32_t pallet_end_index)
{
    auto find_iter = find_if(
        customizations_.begin(),
        customizations_.end(),
        [name] (const Customization& customization)
    {
        return customization.name.compare(name) == 0;
    });

    if (find_iter != customizations_.end())
    {
        // Already in the list.
        return 0;
    }

    Customization customization;    
    customization.index = customizations_.back().index + 1;
    customization.name = move(name);
    customization.pallet_selection = pallet_selection;
    customization.pallet_start_index = pallet_start_index;
    customization.pallet_end_index = pallet_end_index;

    customizations_.push_back(customization);

    return customization.index;
}

void ManufactureSchematic::UpdateCustomization(
    uint16_t index,
    std::string name,
    uint32_t pallet_selection,
    uint32_t pallet_start_index,
    uint32_t pallet_end_index)
{
    auto find_iter = find_if(
        customizations_.begin(),
        customizations_.end(),
        [index] (const Customization& customization)
    {
        return index == customization.index;
    });

    if (find_iter == customizations_.end())
    {
        // Not in the list.
        return;
    }
    
    find_iter->name = move(name);
    find_iter->pallet_selection = pallet_selection;
    find_iter->pallet_start_index = pallet_start_index;
    find_iter->pallet_end_index = pallet_end_index;
}

void ManufactureSchematic::ResetCustomizations(std::vector<Customization> customizations)
{
    customizations_ = move(customizations);
}

void ManufactureSchematic::ClearAllCustomizations()
{
    customizations_.clear();
}

bool ManufactureSchematic::IsReady() const
{
    return is_ready_;
}

void ManufactureSchematic::ToggleReady()
{
    is_ready_ = !is_ready_;
}
