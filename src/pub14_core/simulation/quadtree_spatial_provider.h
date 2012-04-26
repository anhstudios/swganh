// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef QUADTREE_SPATIAL_PROVIDER_H_
#define QUADTREE_SPATIAL_PROVIDER_H_

#include "swganh/simulation/spatial_provider_interface.h"
#include "node.h"

class QuadtreeSpatialProvider : public swganh::simulation::SpatialProviderInterface
{
public:
	QuadtreeSpatialProvider(anh::app::KernelInterface* kernel);
	virtual ~QuadtreeSpatialProvider(void);

	virtual void AddObject(std::shared_ptr<swganh::object::Object> obj);
	virtual void RemoveObject(std::shared_ptr<swganh::object::Object> obj);
	virtual void UpdateObject(std::shared_ptr<swganh::object::Object> obj, glm::vec3 old_position, glm::vec3 new_position);

	virtual std::vector<std::shared_ptr<swganh::object::Object>> GetObjectsInRange(glm::vec3 point, float range);

private:
	quadtree::Node root_node_;
};

#endif // QUADTREE_SPATIAL_PROVIDER_H_