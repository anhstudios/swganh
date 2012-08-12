// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef QUADTREE_SPATIAL_PROVIDER_H_
#define QUADTREE_SPATIAL_PROVIDER_H_

#include <boost/thread/mutex.hpp>

#include "swganh/simulation/spatial_provider_interface.h"
#include "node.h"

namespace swganh_core {
namespace simulation {

class QuadtreeSpatialProvider 
	: public swganh::simulation::SpatialProviderInterface
{
public:
	QuadtreeSpatialProvider();
	virtual ~QuadtreeSpatialProvider(void);

	uint64_t GetObjectId() { return 0; }

	//Object Management
	virtual void AddObject(std::shared_ptr<swganh::object::Object> newObject);
	virtual void AddObject(std::shared_ptr<swganh::object::Object> newObject, int32_t arrangement_id);
	virtual void RemoveObject(std::shared_ptr<swganh::object::Object> oldObject);
	virtual void TransferObject(std::shared_ptr<swganh::object::Object> object, std::shared_ptr<ContainerInterface> newContainer);
	virtual void TransferObject(std::shared_ptr<swganh::object::Object> object, std::shared_ptr<ContainerInterface> newContainer, int32_t arrangement_id);
	virtual void UpdateObject(std::shared_ptr<swganh::object::Object> obj, glm::vec3 old_position, glm::vec3 new_position);
	virtual void ViewObjects(uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<swganh::object::Object>)>, std::shared_ptr<swganh::object::Object> hint = nullptr);

	// FOR USE BY TRANSFER OBJECT DO NOT CALL IN OUTSIDE CODE
	virtual void __InternalInsert(std::shared_ptr<swganh::object::Object> object, int32_t arrangement_id);
	virtual void __InternalInsert(std::shared_ptr<swganh::object::Object> object);
	virtual void LockObjectMutex() { spatial_mutex_.lock(); }
	virtual void UnlockObjectMutex() { spatial_mutex_.unlock(); }

	void SetThis(std::shared_ptr<ContainerInterface> si) { __this = si; }
private:
	std::shared_ptr<ContainerInterface> __this;
	boost::mutex spatial_mutex_;
	quadtree::Node root_node_;
	quadtree::QueryBox GetQueryBoxViewRange(std::shared_ptr<swganh::object::Object> object);
};

}} // swganh_core::simulation

#endif // QUADTREE_SPATIAL_PROVIDER_H_