// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "quadtree_spatial_provider.h"

#include "anh/logger.h"

#include "swganh/object/object.h"

using std::shared_ptr;

using namespace anh::observer;
using namespace swganh::object;
using namespace swganh_core::simulation;
using namespace quadtree;

QuadtreeSpatialProvider::QuadtreeSpatialProvider()
	: root_node_(ROOT, Region(Point(-8300.0f, -8300.0f), Point(8300.0f, 8300.0f)), 0, 9, nullptr)
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

void QuadtreeSpatialProvider::TransferObject(std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer)
{
	
}

void QuadtreeSpatialProvider::ViewObjects(uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func, std::shared_ptr<Object> hint = nullptr)
{
	LOG(warning) << "QUADTREE VIEW OBJECTS ";
	std::vector<std::shared_ptr<Object>> contained_objects;
	if (hint)
	{
		contained_objects = root_node_.Query(GetQueryBoxViewRange(hint));
	}
	else
	{
		contained_objects = root_node_.GetContainedObjects();
	}

	for (auto& object : contained_objects)
	{
		if (topDown)
			func(object);

		if (max_depth != 1)
			object->ViewObjects((max_depth == 0 ? 0 : max_depth - 1), topDown, func);

		if (!topDown)
			func(object);
	}
}

void QuadtreeSpatialProvider::__InternalInsert(std::shared_ptr<Object> object)
{
	LOG(warning) << "QUADTREE __InternalInsert " << object->GetObjectId();

	root_node_.
}

void QuadtreeSpatialProvider::AddAwareObject(std::shared_ptr<anh::observer::ObserverInterface> object)
{

}
		
void QuadtreeSpatialProvider::ViewAwareObjects(std::function<void(std::shared_ptr<anh::observer::ObserverInterface>)>)
{

}

void QuadtreeSpatialProvider::RemoveAwareObject(std::shared_ptr<anh::observer::ObserverInterface> object)
{

}

QueryBox QuadtreeSpatialProvider::GetQueryBoxViewRange(std::shared_ptr<Object> object)
{
	auto position = object->GetPosition();
	return QueryBox(Point(position.x - 256, position.y + 256), Point(position.x + 256, position.y - 256));
}