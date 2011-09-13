#ifndef SWGANH_OBJECT_TAGIBLE_RESOURCE_CONTAINER_H_
#define SWGANH_OBJECT_TAGIBLE_RESOURCE_CONTAINER_H_

#include "swganh/object/tangible.h"
#include <glm\glm.hpp>

namespace swganh {
namespace object {
namespace tangible {

class ResourceContainer : public swganh::object::Tangible {

public:
    // RCNO
    virtual uint32_t GetType() { return ResourceContainer::type; }
    const static uint32_t type = 0x52434e4f;
private:
    uint32_t current_units_;			//update 3
	uint64_t global_resource_id_;		//update 3
	uint32_t max_quantity_;				//update 6
	std::string resource_type_;			//update 6
	std::wstring variation_name_;		//update 6
};

}}} // swganh::object::tangible


#endif //SWGANH_OBJECT_TAGIBLE_RESOURCE_CONTAINER_H_