// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "object.h"

#include <glm/gtx/transform2.hpp>
#include <sstream>

#include "object_events.h"

#include "swganh/logger.h"
#include "swganh/crc.h"
#include "swganh/observer/observer_interface.h"


#include "swganh_core/messages/scene_create_object_by_crc.h"
#include "swganh_core/messages/update_containment_message.h"
#include "swganh_core/messages/scene_destroy_object.h"

#include "swganh_core/messages/base_baselines_message.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/messages/controllers/object_menu_response.h"

#include "swganh_core/object/permissions/container_permissions_interface.h"

using namespace swganh::observer;
using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

Object::Object()
    : database_persisted_(true)
    , in_snapshot_(false)
    , object_id_(0)
    , scene_id_(0)
	, instance_id_(0)
    , template_string_("")
    , position_(glm::vec3(0,0,0))
    , orientation_(glm::quat())
    , complexity_(0)
    , stf_name_file_("")
    , stf_name_string_("")
    , custom_name_(L"")
    , volume_(1)
	, arrangement_id_(-2)
	, attributes_template_id(-1)
	, collision_length_(0.0f)
	, collision_height_(0.0f)
	, collidable_(false)
	, controller_(nullptr)
	, event_dispatcher_(nullptr)
{
}

Object::~Object()
{
}

bool Object::HasController() {
    auto lock = AcquireLock();
    return HasController(lock);
}

bool Object::HasController(boost::unique_lock<boost::mutex>& lock)
{
    return controller_ != nullptr;
}

shared_ptr<ObserverInterface> Object::GetController() {
    auto lock = AcquireLock();
    return GetController(lock);
}

shared_ptr<ObserverInterface> Object::GetController(boost::unique_lock<boost::mutex>& lock)
{
    return controller_;
}

void Object::SetController(const shared_ptr<ObserverInterface>& controller) {
    auto lock = AcquireLock();
    SetController(controller, lock);
}

void Object::SetController(const shared_ptr<ObserverInterface>& controller, boost::unique_lock<boost::mutex>& lock)
{
	controller_ = controller;
    Subscribe(controller, lock);
}

void Object::ClearController() {
    auto lock = AcquireLock();
    ClearController(lock);
}

void Object::ClearController(boost::unique_lock<boost::mutex>& lock)
{
    for(auto& aware : aware_objects_) {
        aware->Unsubscribe(controller_, lock);
    }

    controller_.reset();
}

void Object::AddObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> obj, int32_t arrangement_id)
{
	//// CHECK PERMISSIONS ////
	if(requester == nullptr || container_permissions_->canInsert(shared_from_this(), requester, obj))
	{
		boost::upgrade_lock<boost::shared_mutex> lock(global_container_lock_);
		
		//Add Object To Datastructure
		{
			boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock(lock);
			arrangement_id = __InternalInsert(obj, obj->GetPosition(), arrangement_id);
		}

		//Update our observers with the new object
		std::for_each(aware_objects_.begin(), aware_objects_.end(), [&] (std::shared_ptr<Object> object) {
			obj->__InternalAddAwareObject(object, true);		
			object->__InternalAddAwareObject(obj, true);
		});
	}
}

void Object::RemoveObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> oldObject)
{
	//// CHECK PERMISSIONS ////
	if(requester == nullptr || container_permissions_->canRemove(shared_from_this(), requester, oldObject))
	{
		boost::upgrade_lock<boost::shared_mutex> lock(global_container_lock_);

		//Update our observers about the dead object
		std::for_each(aware_objects_.begin(), aware_objects_.end(), [&] (std::shared_ptr<Object> object) {
			oldObject->__InternalRemoveAwareObject(object, true);	
			object->__InternalRemoveAwareObject(oldObject, true);
		});

		{
			boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock(lock);
			
			//Remove Object from Datastructure
			for(auto& slot : slot_descriptor_)
			{
				slot.second->remove_object(oldObject);
			}
			oldObject->SetContainer(nullptr);
		}
	}
}

void Object::TransferObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, glm::vec3 new_position, int32_t arrangement_id)
{
	//// CHECK PERMISSIONS ////
	if(	requester == nullptr || (
		this->GetPermissions()->canRemove(shared_from_this(), requester, object) && 
		newContainer->GetPermissions()->canInsert(newContainer, requester, object)))
	{
		boost::upgrade_lock<boost::shared_mutex> uplock(global_container_lock_);
		
		{
			boost::upgrade_to_unique_lock<boost::shared_mutex> unique(uplock);

			//Perform the transfer
			for(auto& slot : slot_descriptor_)
			{
				slot.second->remove_object(object);
			}

			arrangement_id = newContainer->__InternalInsert(object, new_position, arrangement_id);
		}

		//Split into 3 groups -- only ours, only new, and both ours and new
		std::set<std::shared_ptr<Object>> oldObservers, newObservers, bothObservers;

		object->__InternalViewAwareObjects([&] (std::shared_ptr<Object> observer) {
			oldObservers.insert(observer);
		});
	
		newContainer->__InternalViewAwareObjects([&] (std::shared_ptr<Object> observer) 
		{
			if(newContainer->GetPermissions()->canView(newContainer, observer))
			{
				auto itr = oldObservers.find(observer);
				if(itr != oldObservers.end())
				{
					oldObservers.erase(itr);
					bothObservers.insert(observer);
				} 
				else 
				{
					newObservers.insert(observer);
				}
			}
		}, requester);

		//Send Creates to only new
		for(auto& observer : newObservers) {
			object->__InternalAddAwareObject(observer, true);
		}

		//Send updates to both
		for(auto& observer : bothObservers) {
			object->SendUpdateContainmentMessage(observer->GetController());
		}

		//Send destroys to only ours
		for(auto& observer : oldObservers) {
			object->__InternalRemoveAwareObject(observer, true);
		}
	}
}

void Object::__InternalViewObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func)
{
	//// CHECK PERMISSIONS ////
	if(requester == nullptr || container_permissions_->canView(shared_from_this(), requester))
	{
		uint32_t requester_instance = 0;
		if(requester)
			requester_instance = requester->GetInstanceId();

		//// ITERATE THROUGH ALL SLOTS ////
		for(auto& slot : slot_descriptor_)
		{
			slot.second->view_objects([&] (const std::shared_ptr<Object>& object) {
				uint32_t object_instance = object->GetInstanceId();
				if(object_instance == 0 || object_instance == requester_instance)
				{
					if(topDown)
						func(object);

					if(max_depth != 1)
						object->__InternalViewObjects(requester, (max_depth == 0) ? 0 : max_depth-1, topDown, func);

					if(!topDown)
						func(object);
				}
			});
		}
	}
}

void Object::__InternalGetObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown, std::list<std::shared_ptr<Object>>& out)
{
	//// CHECK PERMISSIONS ////
	if(requester == nullptr || container_permissions_->canView(shared_from_this(), requester))
	{
		uint32_t requester_instance = 0;
		if(requester)
			requester_instance = requester->GetInstanceId();

		//// ITERATE THROUGH ALL OBJECT SLOTS ////
		for(auto& slot : slot_descriptor_)
		{
			slot.second->view_objects([&] (const std::shared_ptr<Object>& object) {
				uint32_t object_instance = object->GetInstanceId();
				if(object_instance == 0 || object_instance == requester_instance)
				{
					if(topDown)
						out.push_back(object);

					if(max_depth != 1)
						object->__InternalGetObjects(requester, (max_depth == 0) ? 0 : max_depth-1, topDown, out);

					if(!topDown)
						out.push_back(object);
				}
			});
		}
	}
}

int32_t Object::__InternalInsert(std::shared_ptr<Object> object, glm::vec3 new_position, int32_t arrangement_id)
{
	std::shared_ptr<Object> removed_object = nullptr;
	if(arrangement_id == -2)
		arrangement_id = GetAppropriateArrangementId(object);

	if (arrangement_id < 4)
	{
		// Remove object in existing slot
		removed_object = slot_descriptor_[arrangement_id]->insert_object(object);
		if (removed_object)
		{
			// Transfer it out, put it in the place the replacing object came from
			removed_object->__InternalTransfer(nullptr, removed_object, object->GetContainer());
		}
	}
	else
	{
		auto& arrangement = object->slot_arrangements_[arrangement_id-4];
		for (auto& i : arrangement)
		{
			slot_descriptor_[i]->insert_object(object);			
			// Remove object in existing slot
			//removed_object = 
			//if (removed_object && removed_object != object)
			//{
			//	// Transfer it out, put it in the place the replacing object came from
			//	removed_object->__InternalTransfer(nullptr, removed_object, object->GetContainer());
			//}
		}
	}
	object->SetArrangementId(arrangement_id);
	object->SetContainer(shared_from_this());
	object->SetSceneId(scene_id_);

	//Time to update the position to the new coordinates/update AABB
	object->SetPosition(new_position);

	//Because we may have calculated it internally, send the arrangement_id used back
	//To the caller so it can send the appropriate update.
	return arrangement_id;
}

void Object::SwapSlots(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, int32_t new_arrangement_id)
{
	boost::upgrade_lock<boost::shared_mutex> uplock(global_container_lock_);
	
	{
		boost::upgrade_to_unique_lock<boost::shared_mutex> unique(uplock);
		for(auto& slot : slot_descriptor_)
		{
			slot.second->remove_object(object);
		}
		__InternalInsert(object, object->GetPosition(), new_arrangement_id);
	}

	__InternalViewAwareObjects([&] (std::shared_ptr<Object> aware_object) {
		object->SendUpdateContainmentMessage(object->GetController());
	});
}

void Object::__InternalTransfer(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, int32_t arrangement_id)
{
	// we are already locked
	if(	requester == nullptr || (
	this->GetPermissions()->canRemove(shared_from_this(), requester, object) && 
	newContainer->GetPermissions()->canInsert(newContainer, requester, object)))
	{
		arrangement_id = newContainer->__InternalInsert(object, object->GetPosition(), arrangement_id);

		//Split into 3 groups -- only ours, only new, and both ours and new
		std::set<std::shared_ptr<Object>> oldObservers, newObservers, bothObservers;
	
		object->__InternalViewAwareObjects([&] (std::shared_ptr<Object> observer) {
			oldObservers.insert(observer);
		});
	
		newContainer->__InternalViewAwareObjects([&] (std::shared_ptr<Object> observer) 
		{
			if(newContainer->GetPermissions()->canView(newContainer, observer))
			{
				auto itr = oldObservers.find(observer);
				if(itr != oldObservers.end())
				{
					oldObservers.erase(itr);
					bothObservers.insert(observer);
				} 
				else 
				{
					newObservers.insert(observer);
				}
			}
		}, requester);
	
		//Send Creates to only new
		for(auto& observer : newObservers) {
			object->__InternalAddAwareObject(observer, true);
		}
	
		//Send updates to both
		for(auto& observer : bothObservers) {
			object->SendUpdateContainmentMessage(observer->GetController());
		}
	
		//Send destroys to only ours
		for(auto& observer : oldObservers) {
			object->__InternalRemoveAwareObject(observer, true);
		}
	}
}


void Object::__InternalAddAwareObject(std::shared_ptr<swganh::object::Object> object, bool reverse_still_valid)
{	
	// If we're not already aware of the given object
	auto find_itr = aware_objects_.find(object);
	if(find_itr == aware_objects_.end())
	{
		// Make ourselves a aware of the object
		aware_objects_.insert(object);
		
		// If we're an object with an observer, send the packets
		if(!IsInSnapshot())
		{
			auto observer = object->GetController();
			if(observer)
			{
				Subscribe(observer);
				SendCreateByCrc(observer);
				CreateBaselines(observer);
			}
		}
	
		// Preserve the old reverse value, and generate the one for this level.
		bool old_reverse = reverse_still_valid;
		reverse_still_valid = GetPermissions()->canView(shared_from_this(), object);
		
		// Iterate through the slots, and handle all the sub-objects
		for(auto& slot : slot_descriptor_)
		{
			slot.second->view_objects([&] (const std::shared_ptr<Object>& v) {
				
				//If our parent didn't block view, object can see v as well.
				if(old_reverse)
					object->__InternalAddAwareObject(v, false);

				// If we didn't invalidate viewability, keep going.
				if(reverse_still_valid)
					v->__InternalAddAwareObject(object, reverse_still_valid);
			});
		}
	}
}

void Object::__InternalViewAwareObjects(std::function<void(std::shared_ptr<swganh::object::Object>)> func, std::shared_ptr<swganh::object::Object> hint)
{
	std::for_each(aware_objects_.begin(), aware_objects_.end(), func);
}

void Object::__InternalRemoveAwareObject(std::shared_ptr<swganh::object::Object> object, bool reverse_still_valid)
{
	auto find_itr = aware_objects_.find(object);
	if(find_itr != aware_objects_.end())
	{
		bool old_reverse = reverse_still_valid;
		reverse_still_valid = GetPermissions()->canView(shared_from_this(), object);

		for(auto& slot : slot_descriptor_)
		{
			slot.second->view_objects([&] (const std::shared_ptr<Object>& v) {
				if(reverse_still_valid)
					v->__InternalRemoveAwareObject(object, reverse_still_valid);

				if(old_reverse)
					object->__InternalRemoveAwareObject(v, false);
			});
		}

		aware_objects_.erase(find_itr);

		if(!IsInSnapshot())
		{
			auto observer = object->GetController();
			if(observer)
			{
				//DLOG(info) << "DELETING " << GetObjectId() << " FOR " << observer->GetId();
				SendDestroy(observer);
				Unsubscribe(observer);
			}
		}
	}
}

bool Object::__HasAwareObject(std::shared_ptr<Object> object)
{
	return aware_objects_.find(object) != aware_objects_.end();
}

void Object::__InternalGetAbsolutes(glm::vec3& pos, glm::quat& rot)
{
	auto parentContainer = GetContainer();
	if(parentContainer)
	{
		 parentContainer->__InternalGetAbsolutes(pos, rot);
	}
	else
	{
		pos = glm::vec3();
		rot = glm::quat();
	}

	auto lock = AcquireLock();
	pos = (rot * position_) + pos;
	rot = rot * orientation_;
}

string Object::GetTemplate() {
    auto lock = AcquireLock();
    return GetTemplate(lock);
}

string Object::GetTemplate(boost::unique_lock<boost::mutex>& lock)
{
	return template_string_;
}

void Object::SetTemplate(const string& template_string) {
    auto lock = AcquireLock();
    SetTemplate(template_string, lock);
}

void Object::SetTemplate(const string& template_string, boost::unique_lock<boost::mutex>& lock)
{
	template_string_ = template_string;
	DISPATCH(Object, Template);
}

void Object::SetObjectId(uint64_t object_id) {
    auto lock = AcquireLock();
    SetObjectId(object_id, lock);
}

void Object::SetObjectId(uint64_t object_id, boost::unique_lock<boost::mutex>& lock)
{
    object_id_ = object_id;
}

uint64_t Object::GetObjectId() {
    auto lock = AcquireLock();
    return GetObjectId(lock);
}

uint64_t Object::GetObjectId(boost::unique_lock<boost::mutex>& lock)
{
    return object_id_;
}

wstring Object::GetCustomName() {
    auto lock = AcquireLock();
    return GetCustomName(lock);
}

wstring Object::GetCustomName(boost::unique_lock<boost::mutex>& lock)
{
    return custom_name_;
}

void Object::SetCustomName(wstring custom_name) {
    auto lock = AcquireLock();
    SetCustomName(custom_name, lock);
}

void Object::SetCustomName(wstring custom_name, boost::unique_lock<boost::mutex>& lock)
{
    custom_name_ = custom_name;
    DISPATCH(Object, CustomName);
}

std::wstring Object::GetFirstName() const {
    auto lock = AcquireLock();
    return GetFirstName(lock);
}

std::wstring Object::GetFirstName(boost::unique_lock<boost::mutex>& lock) const
{
    std::size_t pos = custom_name_.find(L" ");
    return custom_name_.substr(0, pos);
}

std::wstring Object::GetSurName() const {
    auto lock = AcquireLock();
    return GetSurName(lock);
}

std::wstring Object::GetSurName(boost::unique_lock<boost::mutex>& lock) const
{
    std::size_t pos = custom_name_.find(L" ");
    return custom_name_.substr(pos, std::string::npos);
}

bool Object::HasObservers() {
    auto lock = AcquireLock();
    return HasObservers(lock);
}

bool Object::HasObservers(boost::unique_lock<boost::mutex>& lock)
{
    return !observers_.empty();
}

void Object::Subscribe(const shared_ptr<ObserverInterface>& observer) {
    auto lock = AcquireLock();
    Subscribe(observer, lock);
}

void Object::Subscribe(const shared_ptr<ObserverInterface>& observer, boost::unique_lock<boost::mutex>& lock)
{
    if (observers_.find(observer) == observers_.end())
    {
        observers_.insert(observer);
    }
}

void Object::Unsubscribe(const shared_ptr<ObserverInterface>& observer) {
    auto lock = AcquireLock();
    Unsubscribe(observer, lock);
}

void Object::Unsubscribe(const shared_ptr<ObserverInterface>& observer, boost::unique_lock<boost::mutex>& lock)
{
    observers_.erase(observer);
}

void Object::NotifyObservers(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    NotifyObservers(message, lock);
}

void Object::NotifyObservers(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
    std::for_each(
        observers_.begin(),
        observers_.end(),
        [&message] (const std::shared_ptr<swganh::observer::ObserverInterface>& observer)
    {
        observer->Notify(message);
    });
}

void Object::AddDeltasUpdate(DeltasMessage* message)
{
    NotifyObservers(message);
}

void Object::SetPosition(glm::vec3 position) {
    auto lock = AcquireLock();
    SetPosition(position, lock);
}

void Object::SetPosition(glm::vec3 position, boost::unique_lock<boost::mutex>& lock)
{
	position_ = position;
    BuildSpatialProfile(lock);
	DISPATCH(Object, Position);
}

void Object::UpdatePosition(const glm::vec3& new_position, const glm::quat& orientation, shared_ptr<Object> parent)
{
	SetOrientation(orientation);
	// Call an Event that gets handled by the movement manager

	GetEventDispatcher()->Dispatch(make_shared<UpdatePositionEvent>
		("Object::UpdatePosition", parent, shared_from_this(), new_position));
}

glm::vec3 Object::GetPosition() {
    auto lock = AcquireLock();
    return GetPosition(lock);
}

glm::vec3 Object::GetPosition(boost::unique_lock<boost::mutex>& lock)
{
	return position_;
}

bool Object::InRange(glm::vec3 target, float range)
{
	if (glm::distance(GetPosition(), target) > range)
	{
		return false;
	}
	return true;
}

float Object::RangeTo(glm::vec3 target)
{
	return glm::distance(GetPosition(), target);
}

void Object::SetOrientation(glm::quat orientation) {
    auto lock = AcquireLock();
    SetOrientation(orientation, lock);
}

void Object::SetOrientation(glm::quat orientation, boost::unique_lock<boost::mutex>& lock)
{
	orientation_ = orientation;
	DISPATCH(Object, Orientation);
}

glm::quat Object::GetOrientation() {
    auto lock = AcquireLock();
    return GetOrientation(lock);
}

glm::quat Object::GetOrientation(boost::unique_lock<boost::mutex>& lock)
{
	return orientation_;
}

void Object::FaceObject(const std::shared_ptr<Object>& object)
{
    auto target_position = object->GetPosition();
    {
	    FacePosition(target_position);
    }    
}

void Object::FacePosition(const glm::vec3& position) {
    auto lock = AcquireLock();
    FacePosition(position, lock);
}

void Object::FacePosition(const glm::vec3& position, boost::unique_lock<boost::mutex>& lock)
{
	
    // Create a mirror direction vector for the direction we want to face.
    glm::vec3 direction_vector = glm::normalize(position - position_);
    direction_vector.x = -direction_vector.x;

    // Create a lookat matrix from the direction vector and convert it to a quaternion.
    orientation_ = glm::toQuat(glm::lookAt(
                                    direction_vector, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));                        

    // If in the 3rd quadrant the signs need to be flipped.
    if (orientation_.y <= 0.0f && orientation_.w >= 0.0f) {
        orientation_.y = -orientation_.y;
        orientation_.w = -orientation_.w; 
    }
	DISPATCH(Object, Orientation);
}

uint8_t Object::GetHeading() { return GetHeading(GetOrientation()); }
uint8_t Object::GetHeading(glm::quat orientation)
{
    float heading = 0.0f;

    if (glm::length(orientation) > 0.0f)
    {
        float s = sqrt(1 - (orientation.w * orientation.w));
        if (s != 0.0f)
        {
            if (orientation.y < 0.0f && orientation.w > 0.0f) 
            {
                orientation.y *= -1;
	        	orientation.w *= -1;
            }

			float radians = 2.0f * acos(orientation.w);
			float t = radians / 0.06283f;
			heading = (orientation.y / s) * t;
        }
    }

	return static_cast<uint8_t>(heading);
}

void Object::SetContainer(const std::shared_ptr<ContainerInterface>& container) {
    auto lock = AcquireLock();
    SetContainer(container, lock);
}

void Object::SetContainer(const std::shared_ptr<ContainerInterface>& container, boost::unique_lock<boost::mutex>& lock)
{
    container_ = container;		
	DISPATCH(Object, Container);
}

shared_ptr<ContainerInterface> Object::GetContainer() {
    auto lock = AcquireLock();
    return GetContainer(lock);
}

shared_ptr<ContainerInterface> Object::GetContainer(boost::unique_lock<boost::mutex>& lock)
{
	return container_;
}

void Object::SetComplexity(float complexity) {
    auto lock = AcquireLock();
    SetComplexity(complexity, lock);
}

void Object::SetComplexity(float complexity, boost::unique_lock<boost::mutex>& lock)
{
	complexity_ = complexity;
	DISPATCH(Object, Complexity);
}

float Object::GetComplexity() {
    auto lock = AcquireLock();
    return GetComplexity(lock);
}

float Object::GetComplexity(boost::unique_lock<boost::mutex>& lock)
{
	return complexity_;
}

void Object::SetStfName(const string& stf_file_name, const string& stf_string) {
    auto lock = AcquireLock();
    SetStfName(stf_file_name, stf_string, lock);
}

void Object::SetStfName(const string& stf_file_name, const string& stf_string, boost::unique_lock<boost::mutex>& lock)
{
    stf_name_file_ = stf_file_name;
    stf_name_string_ = stf_string;
	DISPATCH(Object, StfName);
}

string Object::GetStfNameFile() {
    auto lock = AcquireLock();
    return GetStfNameFile(lock);
}

string Object::GetStfNameFile(boost::unique_lock<boost::mutex>& lock)
{
	return stf_name_file_;
}

string Object::GetStfNameString() {
    auto lock = AcquireLock();
    return GetStfNameString(lock);
}

string Object::GetStfNameString(boost::unique_lock<boost::mutex>& lock)
{
	return stf_name_string_;
}

void Object::SetVolume(uint32_t volume) {
    auto lock = AcquireLock();
    SetVolume(volume, lock);
}

void Object::SetVolume(uint32_t volume, boost::unique_lock<boost::mutex>& lock)
{
    volume_ = volume;
	DISPATCH(Object, Volume);
}

uint32_t Object::GetVolume() {
    auto lock = AcquireLock();
    return GetVolume(lock);
}

uint32_t Object::GetVolume(boost::unique_lock<boost::mutex>& lock)
{
	return volume_;
}

void Object::SetSceneId(uint32_t scene_id) {
    auto lock = AcquireLock();
    SetSceneId(scene_id, lock);
}

void Object::SetSceneId(uint32_t scene_id, boost::unique_lock<boost::mutex>& lock)
{
    scene_id_ = scene_id;
	DISPATCH(Object, SceneId);
}

uint32_t Object::GetSceneId() {
    auto lock = AcquireLock();
    return GetSceneId(lock);
}

uint32_t Object::GetSceneId(boost::unique_lock<boost::mutex>& lock)
{
	return scene_id_;
}

uint32_t Object::GetInstanceId() {
    auto lock = AcquireLock();
    return GetInstanceId(lock);
}

uint32_t Object::GetInstanceId(boost::unique_lock<boost::mutex>& lock)
{
	return instance_id_;
}

void Object::SetInstanceId(uint32_t instance_id) {
    auto lock = AcquireLock();
    SetInstanceId(instance_id, lock);
}

void Object::SetInstanceId(uint32_t instance_id, boost::unique_lock<boost::mutex>& lock)
{
	instance_id_ = instance_id;
	DISPATCH(Object, InstanceId);
}

int32_t Object::GetArrangementId()
{
	return arrangement_id_;
}

void Object::SetArrangementId(int32_t arrangement_id)
{
	arrangement_id_ = arrangement_id;
}

swganh::EventDispatcher* Object::GetEventDispatcher()
{
    return event_dispatcher_;
}

void Object::SetEventDispatcher(swganh::EventDispatcher* dispatcher)
{
    event_dispatcher_ = dispatcher;
}

void Object::CreateBaselines( std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
	if (event_dispatcher_)
	{
		GetEventDispatcher()->Dispatch(make_shared<ObserverEvent>
			("Object::Baselines", shared_from_this(), observer));
	}
}

void Object::SendCreateByCrc(std::shared_ptr<swganh::observer::ObserverInterface> observer) {
    auto lock = AcquireLock();
    SendCreateByCrc(observer, lock);
}

void Object::SendCreateByCrc(std::shared_ptr<swganh::observer::ObserverInterface> observer, boost::unique_lock<boost::mutex>& lock) 
{
	//DLOG(info) << "SEND [" << GetObjectId() << "] (" << GetTemplate() <<") TO " << observer->GetId();

	swganh::messages::SceneCreateObjectByCrc scene_object;
    scene_object.object_id = GetObjectId(lock);
    scene_object.object_crc = swganh::memcrc(GetTemplate(lock));
    scene_object.position = GetPosition(lock);
	scene_object.orientation = GetOrientation(lock);
    scene_object.byte_flag = 0;

    observer->Notify(&scene_object);

	SendUpdateContainmentMessage(observer, lock, true);
}

void Object::SendUpdateContainmentMessage(std::shared_ptr<swganh::observer::ObserverInterface> observer, bool send_on_no_parent) {
    auto lock = AcquireLock();
    SendUpdateContainmentMessage(observer, lock, send_on_no_parent);
}

void Object::SendUpdateContainmentMessage(std::shared_ptr<swganh::observer::ObserverInterface> observer, boost::unique_lock<boost::mutex>& lock, bool send_on_no_parent)
{
	if(observer == nullptr)
		return;

	uint64_t container_id = 0;
	if (auto container = GetContainer(lock))
	{
		lock.unlock();
		container_id = container->GetObjectId();
		lock.lock();
	}

	if(send_on_no_parent || container_id != 0)
	{
		//DLOG(info) << "CONTAINMENT " << GetObjectId() << " INTO " << container_id << " ARRANGEMENT " << arrangement_id_;
		UpdateContainmentMessage containment_message;
		containment_message.container_id = container_id;
		containment_message.object_id = GetObjectId(lock);
		containment_message.containment_type = GetArrangementId();
		observer->Notify(&containment_message);
	}
}

void Object::SendDestroy(std::shared_ptr<swganh::observer::ObserverInterface> observer) {
    auto lock = AcquireLock();
    SendDestroy(observer, lock);
}

void Object::SendDestroy(std::shared_ptr<swganh::observer::ObserverInterface> observer, boost::unique_lock<boost::mutex>& lock)
{
	//DLOG(info) << "DESTROY " << GetObjectId() << " FOR " << observer->GetId();

	swganh::messages::SceneDestroyObject scene_object;
	scene_object.object_id = GetObjectId(lock);

	observer->Notify(&scene_object);
}

void Object::SetFlag(std::string flag) {
    auto lock = AcquireLock();
    SetFlag(flag, lock);
}

void Object::SetFlag(std::string flag, boost::unique_lock<boost::mutex>& lock)
{
    flags_.insert(flag);
}

void Object::RemoveFlag(std::string flag) {
    auto lock = AcquireLock();
    return RemoveFlag(flag, lock);
}

void Object::RemoveFlag(std::string flag, boost::unique_lock<boost::mutex>& lock)
{
    flags_.erase(flag);
}

bool Object::HasFlag(std::string flag) {
    auto lock = AcquireLock();
    return HasFlag(flag, lock);
}

bool Object::HasFlag(std::string flag, boost::unique_lock<boost::mutex>& lock)
{
    return flags_.find(flag) != flags_.end();
}

/// Slots

void Object::SetSlotInformation(ObjectSlots slots, ObjectArrangements arrangements) {
    auto lock = AcquireLock();
    SetSlotInformation(slots, arrangements, lock);
}

void Object::SetSlotInformation(ObjectSlots slots, ObjectArrangements arrangements, boost::unique_lock<boost::mutex>& lock)
{
	slot_descriptor_ = slots;
	slot_arrangements_ = arrangements;
}

int32_t Object::GetAppropriateArrangementId(std::shared_ptr<Object> other)
{
	if (slot_descriptor_.size() == 1)
		return -1;

	// Find appropriate arrangement
	int32_t arrangement_id = 4;
	int32_t filled_arrangement_id = -1;
	// In each arrangment
	for ( auto& arrangement : other->slot_arrangements_)
	{
		bool passes_completely = true;
		bool is_valid = true;
		// Each Slot
		for (auto& slot : arrangement)
		{
			// does slot exist in descriptor
			auto descriptor_iter = slot_descriptor_.find(slot);
			if (descriptor_iter == end(slot_descriptor_))
			{
				is_valid = false;
				break;
			}
			// is slot filled?
			else if (descriptor_iter->second->is_filled())
			{
				passes_completely = false;
			}
		}
		// if not filled return arrangement
		if (is_valid && passes_completely)
			return arrangement_id;
		else if (is_valid)
			filled_arrangement_id = arrangement_id;

		++arrangement_id;
	}
	return filled_arrangement_id;
}

ObjectSlots Object::GetSlotDescriptor()
{
	return slot_descriptor_;
}

ObjectArrangements Object::GetSlotArrangements()
{
	return slot_arrangements_;
}

bool Object::ClearSlot(int32_t slot_id) {
    auto lock = AcquireLock();
    return ClearSlot(slot_id, lock);
}

bool Object::ClearSlot(int32_t slot_id, boost::unique_lock<boost::mutex>& lock)
{
	bool cleared = false;
	auto slot_iter = slot_descriptor_.find(slot_id);
	if (slot_iter != slot_descriptor_.end())
	{
		auto slot = slot_iter->second;
		if (!slot->is_filled())
		{
			slot->view_objects([&](shared_ptr<Object> object){
				slot->remove_object(object);
				cleared = true;
			});
			
		}
	}
	return cleared;
}

shared_ptr<Object> Object::GetSlotObject(int32_t slot_id) {
    auto lock = AcquireLock();
    return GetSlotObject(slot_id, lock);
}

shared_ptr<Object> Object::GetSlotObject(int32_t slot_id, boost::unique_lock<boost::mutex>& lock)
{
	shared_ptr<Object> found = nullptr;
	auto slot_iter = slot_descriptor_.find(slot_id);
	if (slot_iter != slot_descriptor_.end())
	{
		auto slot = slot_iter->second;
		slot->view_objects([&](shared_ptr<Object> object){
			found = object;
		});
	}
	return found;
}

bool Object::IsDatabasePersisted() {
    auto lock = AcquireLock();
    return IsDatabasePersisted(lock);
}

bool Object::IsDatabasePersisted(boost::unique_lock<boost::mutex>& lock)
{
	return database_persisted_;
}

bool Object::IsInSnapshot() {
    auto lock = AcquireLock();
    return IsInSnapshot(lock);
}

bool Object::IsInSnapshot(boost::unique_lock<boost::mutex>& lock)
{
	return in_snapshot_;
}

void Object::SetDatabasePersisted(bool value) {
    auto lock = AcquireLock();
    SetDatabasePersisted(value, lock);
}

void Object::SetDatabasePersisted(bool value, boost::unique_lock<boost::mutex>& lock)
{
	database_persisted_ = value;
}

void Object::SetInSnapshot(bool value) {
    auto lock = AcquireLock();
    SetInSnapshot(value, lock);
}

void Object::SetInSnapshot(bool value, boost::unique_lock<boost::mutex>& lock)
{
	in_snapshot_ = value;
}

AttributesMap Object::GetAttributeMap() {
    auto lock = AcquireLock();
    return GetAttributeMap(lock);
}

AttributesMap Object::GetAttributeMap(boost::unique_lock<boost::mutex>& lock)
{
	return attributes_map_;
}

AttributeVariant Object::GetAttribute(const std::string& name) {
    auto lock = AcquireLock();
    return GetAttribute(name, lock);
}

AttributeVariant Object::GetAttribute(const std::string& name, boost::unique_lock<boost::mutex>& lock)
{
	auto find_iter = find_if(attributes_map_.begin(), attributes_map_.end(), [&](AttributesMap::value_type key_value)
	{
		return key_value.first == name;
	});

	if (find_iter != attributes_map_.end())
	{
		return find_iter->second;
	}	

	return boost::blank();
}

std::wstring Object::GetAttributeAsString(const std::string& name) {
    auto lock = AcquireLock();
    return GetAttributeAsString(name, lock);
}

std::wstring Object::GetAttributeAsString(const std::string& name, boost::unique_lock<boost::mutex>& lock)
{
	try {
		auto val = GetAttribute(name, lock);		
		wstringstream attribute;
		switch (val.which())
		{
			// float
			case 0:
				attribute << boost::get<float>(val);
				break;
			case 1:
				attribute << boost::get<int64_t>(val);
				break;
			case 2:
				return boost::get<wstring>(val);
		}
	} catch (std::exception& e) {
		LOG(error) << "Attribute " << name << " could not be converted to wstring";
		throw e;
	}
	return L"";
}

int8_t Object::GetAttributeTemplateId() {
    auto lock = AcquireLock();
    return GetAttributeTemplateId(lock);
}

int8_t Object::GetAttributeTemplateId(boost::unique_lock<boost::mutex>& lock)
{
	return attributes_template_id;
}

void Object::SetAttributeTemplateId(int8_t attribute_template_id) {
    auto lock = AcquireLock();
    SetAttributeTemplateId(attribute_template_id, lock);
}

void Object::SetAttributeTemplateId(int8_t attribute_template_id, boost::unique_lock<boost::mutex>& lock)
{
	attributes_template_id = attribute_template_id;
}

void Object::UpdateWorldCollisionBox(void)
{ 
	boost::shared_lock<boost::shared_mutex> shared(global_container_lock_);
	__InternalUpdateWorldCollisionBox();
}

void Object::__InternalUpdateWorldCollisionBox()
{
	glm::vec3 pos;
	glm::quat rotation;
	__InternalGetAbsolutes(pos, rotation);
	
	auto rot = glm::yaw(rotation);
	boost::geometry::strategy::transform::translate_transformer<Point, Point> translate(pos.x, pos.z);

	if(rot <= DBL_MAX && rot >= -DBL_MAX) // glm::yaw sometimes results in a non-real float (-1.#IND) that will cause problems if not filted through.
	{
		//std::cout << "Orientation: " << rot << std::endl;
		boost::geometry::strategy::transform::rotate_transformer<Point, Point, boost::geometry::degree> rotation(rot);
		boost::geometry::strategy::transform::ublas_transformer<Point, Point, 2, 2> rotationTranslate(boost::numeric::ublas::prod(translate.matrix(), rotation.matrix()));
		boost::geometry::transform(local_collision_box_, world_collision_box_, rotationTranslate);
	}
	else
	{
		boost::geometry::transform(local_collision_box_, world_collision_box_, translate);
	}
}

bool Object::HasAttribute(const std::string& name) {
    auto lock = AcquireLock();
    return HasAttribute(name, lock);
}

bool Object::HasAttribute(const std::string& name, boost::unique_lock<boost::mutex>& lock)
{
	return attributes_map_.find(name) != attributes_map_.end();
}

void Object::BuildSpatialProfile() {
    auto lock = AcquireLock();
    BuildSpatialProfile(lock);
}

void Object::BuildSpatialProfile(boost::unique_lock<boost::mutex>& lock)
{
	BuildCollisionBox(lock);
	UpdateAABB(lock);
}

void Object::BuildCollisionBox() {
    auto lock = AcquireLock();
    BuildCollisionBox(lock);
}

void Object::BuildCollisionBox(boost::unique_lock<boost::mutex>& lock)
{
	__BuildCollisionBox();

	lock.unlock();
	__InternalUpdateWorldCollisionBox();
	lock.lock();
}

void Object::UpdateAABB() {
    auto lock = AcquireLock();
    UpdateAABB(lock);
}

void Object::UpdateAABB(boost::unique_lock<boost::mutex>& lock) 
{ 
	boost::geometry::envelope(world_collision_box_, aabb_);
}

const std::set<std::shared_ptr<Object>>& Object::GetCollidedObjects(void) const {
    auto lock = AcquireLock();
    return GetCollidedObjects(lock);
}

const std::set<std::shared_ptr<Object>>& Object::GetCollidedObjects(boost::unique_lock<boost::mutex>& lock) const 
{ 
	return collided_objects_; 
}

void Object::AddCollidedObject(std::shared_ptr<Object> obj) {
    auto lock = AcquireLock();
    AddCollidedObject(obj, lock);
}

void Object::AddCollidedObject(std::shared_ptr<Object> obj, boost::unique_lock<boost::mutex>& lock)
{
	if(collided_objects_.insert(obj).second)
    {
        OnCollisionEnter(obj);
    }
}

void Object::RemoveCollidedObject(std::shared_ptr<Object> obj) {
    auto lock = AcquireLock();
    RemoveCollidedObject(obj, lock);
}

void Object::RemoveCollidedObject(std::shared_ptr<Object> obj, boost::unique_lock<boost::mutex>& lock)
{
	if(collided_objects_.erase(obj) > 0)
    {
        OnCollisionLeave(obj);
    }
}

const CollisionBox& Object::GetLocalCollisionBox(void) const {
    auto lock = AcquireLock();
    return GetLocalCollisionBox(lock);
}

const CollisionBox& Object::GetLocalCollisionBox(boost::unique_lock<boost::mutex>& lock) const 
{ 
	return local_collision_box_; 
}

const CollisionBox& Object::GetWorldCollisionBox(void) const {
    auto lock = AcquireLock();
    return GetWorldCollisionBox(lock);
}

const CollisionBox& Object::GetWorldCollisionBox(boost::unique_lock<boost::mutex>& lock) const 
{ 
	return world_collision_box_; 
}

const AABB& Object::GetAABB(void) const {
    auto lock = AcquireLock();
    return GetAABB(lock);
}

const AABB& Object::GetAABB(boost::unique_lock<boost::mutex>& lock) const 
{ 
	return aabb_; 
}

void Object::SetCollisionBoxSize(float length, float height) {
    auto lock = AcquireLock();
    SetCollisionBoxSize(length, height, lock);
}

void Object::SetCollisionBoxSize(float length, float height, boost::unique_lock<boost::mutex>& lock)
{
	collision_length_ = length;
	collision_height_ = height;
}

void Object::SetCollidable(bool collidable) {
    auto lock = AcquireLock();
    SetCollidable(collidable, lock);
}

void Object::SetCollidable(bool collidable, boost::unique_lock<boost::mutex>& lock) 
{ 
	collidable_ = collidable; 
}

bool Object::IsCollidable() const {
    auto lock = AcquireLock();
    return IsCollidable(lock);
}

bool Object::IsCollidable(boost::unique_lock<boost::mutex>& lock) const 
{ 
	return collidable_; 
}

void Object::__BuildCollisionBox(void)
{
	local_collision_box_.clear();
	if(collidable_)
	{
		boost::geometry::append(local_collision_box_, Point((-1.0f * collision_length_) / 2, (-1.0f * collision_length_) / 2));
		boost::geometry::append(local_collision_box_, Point((-1.0f * collision_length_) / 2, collision_length_ / 2));
		boost::geometry::append(local_collision_box_, Point(collision_length_ / 2, collision_length_ / 2));
		boost::geometry::append(local_collision_box_, Point(collision_length_ / 2, (-1.0f * collision_length_) / 2));
	}
	else
	{
		boost::geometry::append(local_collision_box_, Point(0.0f, 0.0f));
	}
}

boost::unique_lock<boost::mutex> Object::AcquireLock() const
{
	return boost::unique_lock<boost::mutex>(object_mutex_);
}

boost::unique_lock<boost::mutex> Object::AcquireLock(boost::defer_lock_t t) const
{
	return boost::unique_lock<boost::mutex>(object_mutex_, t);
}

boost::unique_lock<boost::mutex> Object::AcquireLock(boost::try_to_lock_t t) const
{
	return boost::unique_lock<boost::mutex>(object_mutex_, t);
}

boost::unique_lock<boost::mutex> Object::AcquireLock(boost::adopt_lock_t t) const
{
	return boost::unique_lock<boost::mutex>(object_mutex_, t);
}

std::tuple<boost::unique_lock<boost::mutex>, boost::unique_lock<boost::mutex>> LockSimultaneously(std::shared_ptr<Object>& obj1, std::shared_ptr<Object>& obj2)
{
	//Make the two unlocked unique_locks
	boost::unique_lock<boost::mutex> a = obj1->AcquireLock(boost::defer_lock);
	boost::unique_lock<boost::mutex> b = obj2->AcquireLock(boost::defer_lock);

	//Lock them deadlock safely
	boost::lock(a.mutex(), b.mutex());

	//Return them
	return std::make_tuple(std::move(a), std::move(b));
}
