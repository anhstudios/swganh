// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "quadtree_spatial_provider.h"

#include "swganh/logger.h"

#include "swganh_core/object/object.h"
#include "swganh_core/object/permissions/world_permission.h"

using std::shared_ptr;

using namespace swganh::observer;
using namespace swganh::object;
using namespace swganh::simulation;
using namespace quadtree;

static int VIEWING_RANGE = 128;

QuadtreeSpatialProvider::QuadtreeSpatialProvider()
	: root_node_(ROOT, Region(quadtree::Point(-8300.0f, -8300.0f), 
	quadtree::Point(8300.0f, 8300.0f)), 0, 9, nullptr)
{
	SetPermissions(std::shared_ptr<ContainerPermissionsInterface>(new WorldPermission()));
}

QuadtreeSpatialProvider::~QuadtreeSpatialProvider(void)
{
	__this.reset();
}

void QuadtreeSpatialProvider::AddObject(std::shared_ptr<swganh::object::Object> requester, shared_ptr<Object> object, int32_t arrangement_id)
{
	boost::upgrade_lock<boost::shared_mutex> uplock(global_container_lock_);
	{
		boost::upgrade_to_unique_lock<boost::shared_mutex> unique(uplock);
		root_node_.InsertObject(object);
		object->SetContainer(__this);
		object->SetArrangementId(arrangement_id);
		object->SetSceneId(scene_id_);
	}

	CheckCollisions(object);

	// Make objects aware
	__InternalViewObjects(object, 0, true, [&](shared_ptr<Object> found_object){
		found_object->__InternalAddAwareObject(object);
		object->__InternalAddAwareObject(found_object);
	});
}

void QuadtreeSpatialProvider::RemoveObject(std::shared_ptr<swganh::object::Object> requester,shared_ptr<Object> object)
{
	boost::upgrade_lock<boost::shared_mutex> uplock(global_container_lock_);

    __InternalViewObjects(object, 0, false, [&](shared_ptr<Object> found_object){
		found_object->__InternalRemoveAwareObject(object);
		object->__InternalRemoveAwareObject(found_object);
	});

	{
		boost::upgrade_to_unique_lock<boost::shared_mutex> unique(uplock);
		root_node_.RemoveObject(object);
		object->SetContainer(nullptr);
	}
}

void QuadtreeSpatialProvider::UpdateObject(shared_ptr<Object> obj, const swganh::object::AABB& old_bounding_volume, const swganh::object::AABB& new_bounding_volume)
{
	std::vector<std::shared_ptr<Object>> deleted_objects;

	boost::upgrade_lock<boost::shared_mutex> uplock(global_container_lock_);

	auto old_objects = root_node_.Query(QueryBox(quadtree::Point(old_bounding_volume.max_corner().x() - VIEWING_RANGE, old_bounding_volume.max_corner().y() - VIEWING_RANGE), 
		quadtree::Point(old_bounding_volume.min_corner().x() + VIEWING_RANGE, old_bounding_volume.min_corner().y() + VIEWING_RANGE)));
	{
		boost::upgrade_to_unique_lock<boost::shared_mutex> unique(uplock);
		root_node_.UpdateObject(obj, old_bounding_volume, new_bounding_volume);
	}

	CheckCollisions(obj);

	for(auto object : root_node_.Query(GetQueryBoxViewRange(obj))) 
	{
		auto itr = old_objects.find(object);
		if(itr != old_objects.end()) {
			//It's not old, that's all we can do for now.
			old_objects.erase(itr);
		} else {
			//It's new! Update!
			object->__InternalAddAwareObject(obj);
			obj->__InternalAddAwareObject(object);
		}
	}

	for(auto object : old_objects) 
	{
		//It's old! Toss it!
		obj->__InternalRemoveAwareObject(object);
		object->__InternalRemoveAwareObject(obj);
	}
}

void QuadtreeSpatialProvider::TransferObject(std::shared_ptr<swganh::object::Object> requester,std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, int32_t arrangement_id)
{
	//Perform the transfer
	if (object != newContainer)
	{
		boost::upgrade_lock<boost::shared_mutex> uplock(global_container_lock_);
		{
			boost::upgrade_to_unique_lock<boost::shared_mutex> unique(uplock);
			root_node_.RemoveObject(object);
			arrangement_id = newContainer->__InternalInsert(object, arrangement_id);
		}

		//Split into 3 groups -- only ours, only new, and both ours and new
		std::set<std::shared_ptr<Object>> oldObservers, newObservers, bothObservers;

		object->__InternalViewAwareObjects([&] (std::shared_ptr<Object> observer) {
			oldObservers.insert(observer);
		});

		newContainer->__InternalViewAwareObjects([&] (std::shared_ptr<Object> observer) {
			if(newContainer->GetPermissions()->canView(newContainer, observer))
			{
				auto itr = oldObservers.find(observer);
				if(itr != oldObservers.end()) {
					oldObservers.erase(itr);
					bothObservers.insert(observer);
				} else {
					newObservers.insert(observer);
				}
			}
		});

		//Send Creates to only new
		for_each(newObservers.begin(), newObservers.end(), [&object](shared_ptr<Object> observer)
		{
			object->__InternalAddAwareObject(observer);
		});

		//Send updates to both
		for_each(bothObservers.begin(), bothObservers.end(), [&object](shared_ptr<Object> observer)
		{
			object->SendUpdateContainmentMessage(observer->GetController());
		});

		//Send destroys to only ours
		for_each(oldObservers.begin(), oldObservers.end(), [&object](shared_ptr<Object> observer)
		{
			object->__InternalRemoveAwareObject(observer);
		});
	}
}

void QuadtreeSpatialProvider::ViewObjectsInRange(glm::vec3 position, float radius, uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<swganh::object::Object>)> func)
{
	std::set<std::shared_ptr<Object>> contained_objects;

	boost::shared_lock<boost::shared_mutex> lock(global_container_lock_);
	contained_objects = root_node_.Query(QueryBox(swganh::object::Point(position.x - radius, position.z - radius), 
		swganh::object::Point(position.x + radius, position.z + radius)));

	for (auto& object : contained_objects)
	{
		if (topDown)
			func(object);

		if (max_depth != 1)
			object->__InternalViewObjects(nullptr, (max_depth == 0 ? 0 : max_depth - 1), topDown, func);

		if (!topDown)
			func(object);
	}

}

void QuadtreeSpatialProvider::__InternalViewObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func)
{
	std::set<std::shared_ptr<Object>> contained_objects;
	uint32_t requester_instance = 0;
	if (requester)
	{
		requester_instance = requester->GetInstanceId();
		contained_objects = root_node_.Query(GetQueryBoxViewRange(requester));		
	}

	for (auto& object : contained_objects)
	{
		uint32_t object_instance = object->GetInstanceId();

		if(object_instance == 0 || object_instance == requester_instance)
		{
			if (topDown)
				func(object);

			if (max_depth != 1)
				object->__InternalViewObjects(requester, (max_depth == 0 ? 0 : max_depth - 1), topDown, func);

			if (!topDown)
				func(object);
		}
	}
}

void QuadtreeSpatialProvider::__InternalViewAwareObjects(std::function<void(std::shared_ptr<swganh::object::Object>)> func, std::shared_ptr<swganh::object::Object> hint)
{
	__InternalViewObjects(hint, 0, true, func);
}

int32_t QuadtreeSpatialProvider::__InternalInsert(std::shared_ptr<Object> object, int32_t arrangement_id)
{
	root_node_.InsertObject(object);
	object->SetContainer(__this);
	object->SetSceneId(scene_id_);
	return -1;
}

glm::vec3 QuadtreeSpatialProvider::__InternalGetAbsolutePosition()
{
	return glm::vec3(0, 0, 0);
}

QueryBox QuadtreeSpatialProvider::GetQueryBoxViewRange(std::shared_ptr<Object> object)
{
	auto position = object->__InternalGetAbsolutePosition();
	return QueryBox(quadtree::Point(position.x - VIEWING_RANGE, position.z - VIEWING_RANGE), quadtree::Point(position.x + VIEWING_RANGE, position.z + VIEWING_RANGE));	
}

std::set<std::shared_ptr<swganh::object::Object>> QuadtreeSpatialProvider::Query(boost::geometry::model::polygon<swganh::object::Point> query_box)
{
	std::set<std::shared_ptr<swganh::object::Object>> return_vector;
	QueryBox aabb;

	boost::geometry::envelope(query_box, aabb);

	return_vector = root_node_.Query(aabb); // Find objects without our AABB
	for(auto i = return_vector.begin(); i != return_vector.end(); i++)
	{
		// Do more precise intersection detection, if we are not colliding, erase.
		if(boost::geometry::intersects((*i)->GetWorldCollisionBox(), query_box) == false)
			i = return_vector.erase(i);
	}

	return return_vector;
}

std::set<std::pair<float, std::shared_ptr<swganh::object::Object>>> QuadtreeSpatialProvider::FindObjectsInRangeByTag(std::shared_ptr<swganh::object::Object> requester, const std::string& tag, float range)
{
	std::set<std::pair<float, std::shared_ptr<swganh::object::Object>>> obj_map;

	if(requester->GetContainer() != __this)
	{
		auto root_obj = requester->GetContainer();
		while(root_obj->GetContainer() != __this && root_obj->GetContainer() != nullptr)
			root_obj = root_obj->GetContainer();

		root_obj->ViewObjects(requester, 0, true, [=, &obj_map](std::shared_ptr<swganh::object::Object> object) {
			if(object->HasFlag(tag))
				obj_map.insert(std::pair<float, std::shared_ptr<swganh::object::Object>>(glm::distance(requester->GetAbsolutePosition(), object->GetAbsolutePosition()), object));
		});

		return obj_map;
	}

	QueryBox query_box;
	if(range < 0) // Query the whole map.
		query_box = QueryBox(quadtree::Point(-8300.0f, -8300.0f), quadtree::Point(8300.0f, 8300.0f));
	else // Generate query box.
		query_box = QueryBox(
			quadtree::Point(requester->GetPosition().x - (range /  2.0f), requester->GetPosition().z - (range / 2.0f)),
			quadtree::Point(requester->GetPosition().x + (range / 2.0f), requester->GetPosition().z + (range / 2.0f))
			);

	auto objects = root_node_.Query(query_box);
	for(auto& object : objects)
	{
		if(object->HasFlag(tag))
		{
			obj_map.insert(std::pair<float, std::shared_ptr<swganh::object::Object>>(glm::distance(requester->GetAbsolutePosition(), object->GetAbsolutePosition()),object));
			if(object->HasContainedObjects())
				object->ViewObjects(object, 0, true, [=, &obj_map](std::shared_ptr<Object> object) {
					if(object->HasFlag(tag))
						obj_map.insert(std::pair<float, std::shared_ptr<swganh::object::Object>>(glm::distance(requester->GetAbsolutePosition(), object->GetAbsolutePosition()), object));
			});
		}
	}

	return obj_map;
}

void QuadtreeSpatialProvider::CheckCollisions(std::shared_ptr<swganh::object::Object> object)
{
	if(object->IsCollidable() == false)
		return;

	auto objects = root_node_.Query(object->GetAABB());
	auto collided_objects = object->GetCollidedObjects();
	std::for_each(collided_objects.begin(), collided_objects.end(), [=, &objects](std::shared_ptr<swganh::object::Object> other) {
		auto iter = std::find(objects.begin(), objects.end(), other);
		
		if(iter == objects.end())
		{
			//std::cout << "Object::OnCollisionLeave " << object->GetObjectId() << " (" << object->GetTemplate() << ") <-> " << other->GetObjectId() << " (" << other->GetTemplate() << ")" << std::endl;
			object->RemoveCollidedObject(other);
			other->RemoveCollidedObject(object);

			object->OnCollisionLeave(other);
			other->OnCollisionLeave(object);
			
			objects.erase(iter);
		}
		else
		{
			// Make sure we still are intersecting.
			if(boost::geometry::intersects(object->GetWorldCollisionBox(), other->GetWorldCollisionBox()) == false) {
				//std::cout << "Object::OnCollisionLeave " << object->GetObjectId() << " (" << object->GetTemplate() << ") <-> " << other->GetObjectId() << " (" << other->GetTemplate() << ")" << std::endl;
				
				object->RemoveCollidedObject(other);
				other->RemoveCollidedObject(object);
				
				object->OnCollisionLeave(other);
				other->OnCollisionLeave(object);
				
				objects.erase(iter);
				return;
			}
			//std::cout << "Object::OnCollisionStay " << object->GetObjectId() << " (" << object->GetTemplate() << ") <-> " << other->GetObjectId() << " (" << other->GetTemplate() << ")" << std::endl;
			object->OnCollisionStay(other);
			other->OnCollisionStay(object);
		}
	});

	std::for_each(objects.begin(), objects.end(), [=](const std::shared_ptr<swganh::object::Object> other) {
		if(other->GetObjectId() == object->GetObjectId() || other->IsCollidable() == false)
			return;

		if(boost::geometry::intersects(object->GetWorldCollisionBox(), other->GetWorldCollisionBox()))
		{
			bool found = false;
			auto collided_objects = object->GetCollidedObjects();
			std::for_each(collided_objects.begin(), collided_objects.end(), [=, &found](std::shared_ptr<Object> collided_object){
				if(collided_object->GetObjectId() == other->GetObjectId())
				{
					found = true;
				}
			});

			if(!found)
			{
				//std::cout << "Object::OnCollisionEnter " << object->GetObjectId() << " (" << object->GetTemplate() << ") <-> " << other->GetObjectId() << " (" << other->GetTemplate() << ")" << std::endl;
				
				object->AddCollidedObject(other);
				other->AddCollidedObject(object);

				object->OnCollisionEnter(other);
				other->OnCollisionEnter(object);
			}
		}
	});
}