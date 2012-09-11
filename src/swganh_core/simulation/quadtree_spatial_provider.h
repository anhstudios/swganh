// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/simulation/spatial_provider_interface.h"
#include "swganh/object/permissions/container_permissions_interface.h"
#include "node.h"

namespace swganh {
namespace simulation {

class QuadtreeSpatialProvider 
	: public swganh::simulation::SpatialProviderInterface
{
public:
	QuadtreeSpatialProvider();
	virtual ~QuadtreeSpatialProvider(void);

	uint64_t GetObjectId() { return 0; }

	//Object Management
	virtual void AddObject(std::shared_ptr<swganh::object::Object> requester, std::shared_ptr<swganh::object::Object> newObject, int32_t arrangement_id=-2);
	virtual void RemoveObject(std::shared_ptr<swganh::object::Object> requester, std::shared_ptr<swganh::object::Object> oldObject);
	virtual void TransferObject(std::shared_ptr<swganh::object::Object> requester, std::shared_ptr<swganh::object::Object> object, std::shared_ptr<ContainerInterface> newContainer, int32_t arrangement_id=-2);
	virtual void UpdateObject(std::shared_ptr<swganh::object::Object> obj, glm::vec3 old_position, glm::vec3 new_position);

	// FOR USE BY TRANSFER OBJECT DO NOT CALL IN OUTSIDE CODE
	virtual int32_t __InternalInsert(std::shared_ptr<swganh::object::Object> object, int32_t arrangement_id=-2);
	virtual void __InternalViewObjects(std::shared_ptr<swganh::object::Object> requester, uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<swganh::object::Object>)> func);

	virtual void __InternalViewAwareObjects(std::function<void(std::shared_ptr<swganh::object::Object>)> func, std::shared_ptr<swganh::object::Object> hint=nullptr);

	virtual std::shared_ptr<ContainerInterface> GetContainer() { return nullptr; }
	virtual void SetContainer(const std::shared_ptr<ContainerInterface>& container) {}

	virtual glm::vec3 __InternalGetAbsolutePosition();

	void SetThis(std::shared_ptr<ContainerInterface> si) { __this = si; }
private:
	std::shared_ptr<ContainerInterface> __this;
	quadtree::Node root_node_;
	quadtree::QueryBox GetQueryBoxViewRange(std::shared_ptr<swganh::object::Object> object);
};

}} // swganh::simulation
