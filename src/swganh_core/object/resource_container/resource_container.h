// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <glm/glm.hpp>

#include "swganh_core/object/tangible/tangible.h"

namespace swganh {
namespace object {

class ResourceContainerFactory;
class ResourceContainerMessageBuilder;
class ResourceContainer : public swganh::object::Tangible
{
public:
	typedef ResourceContainerFactory FactoryType;
    typedef ResourceContainerMessageBuilder MessageBuilderType;

    // RCNO
    ResourceContainer();
    ResourceContainer(const std::string& customization, std::vector<uint32_t> component_customization, uint32_t bitmask_options,
        uint32_t incap_timer, uint32_t condition_damage, uint32_t max_condition, bool is_static, std::vector<uint64_t> defenders,
        uint32_t current_quantity, uint64_t global_resource_id, uint32_t max_quantity, std::string resource_type, std::wstring name);

    virtual uint32_t GetType() const { return ResourceContainer::type; }
    const static uint32_t type = 0x52434e4f;

    uint32_t GetCurrentQuantity();
    void SetCurrentQuantity(uint32_t quantity);

    uint64_t GetGlobalResource();
    void SetGlobalResource(uint64_t global_resource);

    uint32_t GetMaxQuantity();
    void SetMaxQuantity(uint32_t max_quantity);

    std::string GetResourceType();
    void SetResourceType(const std::string& resource_type);

    std::wstring GetResourceName();
    void SetResourceName(const std::wstring& name);

private:
	typedef swganh::ValueEvent<std::shared_ptr<ResourceContainer>> ResourceContainerEvent;

    uint32_t current_quantity_;			                    // update 3
    uint64_t global_resource_id_;		                    // update 3
    uint32_t max_quantity_;				                    // update 6
    std::string resource_type_;			                    // update 6
    std::wstring variation_name_;		                    // update 6
};

}} // swganh::object
