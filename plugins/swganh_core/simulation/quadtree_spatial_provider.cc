// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "quadtree_spatial_provider.h"

#include "anh/logger.h"

using std::shared_ptr;

using anh::app::KernelInterface;
using swganh::object::Object;
using namespace quadtree;

QuadtreeSpatialProvider::QuadtreeSpatialProvider(anh::app::KernelInterface* kernel)
	: SpatialProviderInterface(kernel)
	, root_node_(ROOT, Region(Point(-8300.0f, -8300.0f), Point(8300.0f, 8300.0f)), 0, 9, nullptr)
{
}

QuadtreeSpatialProvider::~QuadtreeSpatialProvider(void)
{
}

void QuadtreeSpatialProvider::AddObject(shared_ptr<Object> obj)
{
	root_node_.InsertObject(obj);
}

void QuadtreeSpatialProvider::RemoveObject(shared_ptr<Object> obj)
{
	root_node_.RemoveObject(obj);
}

void QuadtreeSpatialProvider::UpdateObject(shared_ptr<Object> obj, glm::vec3 old_position, glm::vec3 new_position)
{
	root_node_.UpdateObject(obj, old_position, new_position);

}

std::vector<std::shared_ptr<swganh::object::Object>> QuadtreeSpatialProvider::GetObjectsInRange(glm::vec3 point, float range)
{
	return std::vector<shared_ptr<Object>>();
}
