#ifndef SWGANH_OBJECT_RESOURCE_CONTAINER_RESOURCE_CONTAINER_H_
#define SWGANH_OBJECT_RESOURCE_CONTAINER_RESOURCE_CONTAINER_H_

#include <glm/glm.hpp>

#include "swganh/object/tangible/base_tangible.h"

namespace swganh {
namespace object {
namespace resource_container {

class ResourceContainer : public swganh::object::tangible::BaseTangible
{
public:
    // RCNO
    ResourceContainer();
    ResourceContainer(const std::string& customization, std::vector<uint32_t> component_customization, uint32_t bitmask_options,
        uint32_t incap_timer, uint32_t condition_damage, uint32_t max_condition, bool is_static, std::vector<uint64_t> defenders,
        uint32_t current_quantity, uint64_t global_resource_id, uint32_t max_quantity, std::string resource_type, std::wstring name);

    virtual uint32_t GetType() const { return ResourceContainer::type; }
    const static uint32_t type = 0x52434e4f;
    uint32_t GetCurrentQuantity() { return current_quantity_; }
    void SetCurrentQuantity(uint32_t quantity);

    uint64_t GetGlobalResource() { return global_resource_id_; }
    void SetGlobalResource(uint64_t global_resource);

    uint32_t GetMaxQuantity() { return max_quantity_; }
    void SetMaxQuantity(uint32_t max_quantity);

    std::string GetResourceType() { return resource_type_; }
    void SetResourceType(const std::string& resource_type);

    std::wstring GetResourceName() { return variation_name_; }
    void SetResourceName(const std::wstring& name);

    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline3();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline6();

private:
    uint32_t current_quantity_;			                    // update 3
    uint64_t global_resource_id_;		                    // update 3
    uint32_t max_quantity_;				                    // update 6
    std::string resource_type_;			                    // update 6
    std::wstring variation_name_;		                    // update 6
};

}}} // swganh::object::resource_container

#endif //SWGANH_OBJECT_RESOURCE_CONTAINER_RESOURCE_CONTAINER_H_
