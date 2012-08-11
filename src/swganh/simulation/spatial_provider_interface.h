// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SIMULATION_SPATIAL_PROVIDER_H_
#define SWGANH_SIMULATION_SPATIAL_PROVIDER_H_

#include <memory>
#include <glm/glm.hpp>

#include "swganh/object/container_interface.h"

namespace swganh{
namespace object {
	class Object;
}}


namespace swganh {
namespace simulation {

/**
 * Provides Spatial Indexing functionality.
 */
class SpatialProviderInterface : public swganh::object::ContainerInterface
{
public:
	virtual uint64_t GetObjectId() = 0;

	//Object Management
	virtual void AddObject(std::shared_ptr<swganh::object::Object> newObject) = 0;
	virtual void RemoveObject(std::shared_ptr<swganh::object::Object> oldObject) = 0;
	virtual void TransferObject(std::shared_ptr<swganh::object::Object> object, std::shared_ptr<ContainerInterface> newContainer) = 0;

	virtual void ViewObjects(uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<swganh::object::Object>)> func, std::shared_ptr<swganh::object::Object> hint=nullptr) = 0;

	//FOR USE BY TRANSFER OBJECT ONLY. DO NOT CALL IN OUTSIDE CODE
	virtual void __InternalInsert(std::shared_ptr<swganh::object::Object> object) = 0;
		
	virtual void SpatialProviderInterface::UpdateObject(std::shared_ptr<swganh::object::Object> obj, glm::vec3 old_position, glm::vec3 new_position) = 0;

	virtual void LockObjectMutex() = 0;
	virtual void UnlockObjectMutex() = 0;
};

}} // namespace swganh::simulation

#endif // SWGANH_SIMULATION_SPATIAL_INDEX_PROVIDER_H_