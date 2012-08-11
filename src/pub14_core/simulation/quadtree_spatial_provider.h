// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef QUADTREE_SPATIAL_PROVIDER_H_
#define QUADTREE_SPATIAL_PROVIDER_H_

#include "swganh/object/container_interface.h"
#include "swganh/simulation/spatial_provider_interface.h"
#include "node.h"

namespace swganh_core {
namespace simulation {

class QuadtreeSpatialProvider : public swganh::object::ContainerInterface, public swganh::simulation::SpatialProviderInterface
{
public:
	QuadtreeSpatialProvider();
	virtual ~QuadtreeSpatialProvider(void);

	virtual uint64_t GetObjectId() { return 0; }

	//Object Management
	virtual void AddObject(std::shared_ptr<Object> newObject);
	virtual void RemoveObject(std::shared_ptr<Object> oldObject);
	virtual void TransferObject(std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer);
	virtual void UpdateObject(shared_ptr<Object> obj, glm::vec3 old_position, glm::vec3 new_position);
	virtual void ViewObjects(uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)>, std::shared_ptr<Object> hint = nullptr);

	// FOR USE BY TRANSFER OBJECT DO NOT CALL IN OUTSIDE CODE
	virtual void __InternalInsert(std::shared_ptr<Object> object);

	//Call to Create
	virtual void AddAwareObject(std::shared_ptr<anh::observer::ObserverInterface> object);
		
	//Call to View
	virtual void ViewAwareObjects(std::function<void(std::shared_ptr<anh::observer::ObserverInterface>)>);

	//Call to Destroy
	virtual void RemoveAwareObject(std::shared_ptr<anh::observer::ObserverInterface> object);

private:
	quadtree::Node root_node_;
	QueryBox GetQueryBoxViewRange(std::shared_ptr<Object> object);
};

}} // swganh_core::simulation

#endif // QUADTREE_SPATIAL_PROVIDER_H_