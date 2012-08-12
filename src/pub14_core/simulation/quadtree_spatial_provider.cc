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

static int VIEWING_RANGE = 128;

QuadtreeSpatialProvider::QuadtreeSpatialProvider()
	: root_node_(ROOT, Region(Point(-8300.0f, -8300.0f), Point(8300.0f, 8300.0f)), 0, 9, nullptr)
{
}

QuadtreeSpatialProvider::~QuadtreeSpatialProvider(void)
{
	__this.reset();
}

void QuadtreeSpatialProvider::AddObject(shared_ptr<Object> object)
{
	LOG(warning) << "QUADTREE AddObject " << object->GetObjectId();
	root_node_.InsertObject(object);
	object->SetContainer(__this);
	
	// Make objects aware
	ViewObjects(0, true, [&](shared_ptr<Object> found_object){
		found_object->AddAwareObject(object);
		object->AddAwareObject(found_object);
	}, object);
}

void QuadtreeSpatialProvider::RemoveObject(shared_ptr<Object> object)
{
	LOG(warning) << "QUADTREE RemoveObject " << object->GetObjectId();
	root_node_.RemoveObject(object);
	object->SetContainer(nullptr);

    ViewObjects(0, false, [&](shared_ptr<Object> found_object){
		found_object->RemoveAwareObject(object);
		object->RemoveAwareObject(found_object);
	}, object);
}

void QuadtreeSpatialProvider::UpdateObject(shared_ptr<Object> obj, glm::vec3 old_position, glm::vec3 new_position)
{
	root_node_.UpdateObject(obj, old_position, new_position);

	auto new_objects = root_node_.Query(GetQueryBoxViewRange(obj));
	
	std::vector<std::shared_ptr<Object>> deleted_objects;
	
	obj->ViewAwareObjects([&] (std::shared_ptr<Object> aware_object) 
	{
		// If we are top level (aka SI can see us)
		if (aware_object->GetContainer()->GetObjectId() == GetObjectId())
		{
			auto new_itr = new_objects.find(aware_object);
			if(new_itr != new_objects.end())
			{
				new_objects.erase(new_itr);
			}
			else
			{
				deleted_objects.push_back(aware_object);
			}
		}
	});

	for(auto& to_delete : deleted_objects)
	{
		//Send Destroy
		obj->RemoveAwareObject(to_delete);
		to_delete->RemoveAwareObject(obj);
	}

	//New Objects
	for_each(new_objects.begin(), new_objects.end(), [&](std::shared_ptr<Object> new_obj)
	{
		new_obj->AddAwareObject(obj);
		obj->AddAwareObject(new_obj);
	});
}

void QuadtreeSpatialProvider::TransferObject(std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer)
{
	LOG(warning) << "QUADTREE TRANSFER " << object->GetObjectId() << " FROM " << this->GetObjectId() << " TO " << newContainer->GetObjectId();
	//Perform the transfer
	if (object != newContainer)
	{
		root_node_.RemoveObject(object);
		newContainer->__InternalInsert(object);

		//Split into 3 groups -- only ours, only new, and both ours and new
		std::set<std::shared_ptr<Object>> oldObservers, newObservers, bothObservers;

		object->ViewAwareObjects([&] (std::shared_ptr<Object>& observer) {
			oldObservers.insert(observer);
		});

		newContainer->ViewAwareObjects([&] (std::shared_ptr<Object>& observer) {
			auto itr = oldObservers.find(observer);
			if(itr == oldObservers.end()) {
				oldObservers.erase(itr);
				bothObservers.insert(observer);
			} else {
				newObservers.insert(observer);
			}
		});

		//Send Creates to only new
		for_each(newObservers.begin(), newObservers.end(), [&object](shared_ptr<Object> observer)
		{
			object->AddAwareObject(observer);
		});

		//Send updates to both
		for_each(bothObservers.begin(), bothObservers.end(), [&object](shared_ptr<Object> observer)
		{
			object->SendUpdateContainmentMessage(observer->GetController());
		});

		//Send destroys to only ours
		for_each(oldObservers.begin(), oldObservers.end(), [&object](shared_ptr<Object> observer)
		{
			object->RemoveAwareObject(observer);
		});
	}
}

void QuadtreeSpatialProvider::ViewObjects(uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func, std::shared_ptr<Object> hint)
{
	LOG(warning) << "QUADTREE VIEW OBJECTS ";
	std::set<std::shared_ptr<Object>> contained_objects;
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

	root_node_.InsertObject(object);
	object->SetContainer(__this);
}

QueryBox QuadtreeSpatialProvider::GetQueryBoxViewRange(std::shared_ptr<Object> object)
{
	auto position = object->GetPosition();
	return QueryBox(Point(position.x - VIEWING_RANGE, position.z - VIEWING_RANGE), Point(position.x + VIEWING_RANGE, position.z + VIEWING_RANGE));
	
}