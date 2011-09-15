
#include "swganh/object/base_tangible.h"

#include "swganh/scene/scene.h"
#include "swganh/scene/messages/deltas_message.h"

using namespace swganh::object;
using namespace swganh::scene::messages;

void BaseTangible::AddCustomization(const std::string& customization)
{
    customization_.append(customization);
}

void BaseTangible::SetCustomization(const std::string& customization)
{
    customization_ = customization;
}

void BaseTangible::AddComponentCustomization(uint32_t customization)
{
    // add customization
    component_customization_list_.push_back(customization);
}

void BaseTangible::SetComponentCustomization(std::vector<uint32_t> component_customization)
{
    component_customization_list_ = component_customization;
}
