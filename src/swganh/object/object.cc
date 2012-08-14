// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "object.h"

#include <glm/gtx/transform2.hpp>

#include "object_events.h"

#include "anh/logger.h"
#include "anh/crc.h"
#include "anh/observer/observer_interface.h"


#include "pub14_core/messages/scene_create_object_by_crc.h"
#include "pub14_core/messages/update_containment_message.h"
#include "pub14_core/messages/scene_destroy_object.h"

#include "pub14_core/messages/base_baselines_message.h"
#include "pub14_core/messages/scene_end_baselines.h"

#include "swganh/object/permissions/container_permissions_interface.h"

using namespace anh::observer;
using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

Object::Object()
    : object_id_(0)
    , template_string_("")
    , position_(glm::vec3(0,0,0))
    , orientation_(glm::quat(0,0,0,0))
    , complexity_(0)
    , stf_name_file_("")
    , stf_name_string_("")
    , custom_name_(L"")
    , volume_(0)
	, arrangement_id_(-2)
{
}

bool Object::HasController()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return controller_ != nullptr;
}

shared_ptr<ObjectController> Object::GetController()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return controller_;
}

void Object::SetController(const shared_ptr<ObjectController>& controller)
{
    {
	    boost::lock_guard<boost::mutex> lock(object_mutex_);
        controller_ = controller;
    }

    Subscribe(controller);
}

void Object::ClearController()
{
    shared_ptr<ObjectController> controller;

    {
	    boost::lock_guard<boost::mutex> lock(object_mutex_);
        controller = controller_;
        controller_.reset();
    }

    Unsubscribe(controller);
}

void Object::AddObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> obj, int32_t arrangement_id)
{
	if(requester == nullptr || container_permissions_->canInsert(shared_from_this(), requester, obj))
	{
		LOG(warning) << "INSERTING " << obj->GetObjectId() << " INTO " << this->GetObjectId();
		//Add Object To Datastructure
		arrangement_id = __InternalInsert(obj, arrangement_id);

		//Update our observers with the new object
		std::for_each(aware_objects_.begin(), aware_objects_.end(), [&] (std::shared_ptr<Object> object) {
			obj->AddAwareObject(object);		
		});
	}
	else
	{
		LOG(warning) << "NOT INSERTING " << obj->GetObjectId() << " INTO " << GetObjectId() <<" DUE TO INSUFFICIENT PERMISSIONS";
	}
}

void Object::RemoveObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> oldObject)
{
	if(requester == nullptr || container_permissions_->canRemove(shared_from_this(), requester, oldObject))
	{
		LOG(warning) << "REMOVING " << oldObject->GetObjectId() << " FROM " << this->GetObjectId();
	
		//Update our observers about the dead object
		std::for_each(aware_objects_.begin(), aware_objects_.end(), [&] (std::shared_ptr<Object> object) {
			oldObject->RemoveAwareObject(object);		
		});

		//Remove Object from Datastructure
		for(auto& slot : slot_descriptor_)
		{
			slot.second->remove_object(oldObject);
		}
		oldObject->SetContainer(nullptr);
	}
	else
	{
		LOG(warning) << "NOT REMOVING " << oldObject->GetObjectId() << " FROM " << GetObjectId() <<" DUE TO INSUFFICIENT PERMISSIONS";
	}
}

void Object::TransferObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, int32_t arrangement_id)
{
	if(	requester == nullptr || 
		this->GetPermissions()->canRemove(shared_from_this(), requester, object) && 
		newContainer->GetPermissions()->canInsert(newContainer, requester, object))
	{
		LOG(warning) << "TRANSFER " << object->GetObjectId() << " FROM " << this->GetObjectId() << " TO " << newContainer->GetObjectId();
		//Perform the transfer
		for(auto& slot : slot_descriptor_)
		{
			slot.second->remove_object(object);
		}

		arrangement_id = newContainer->__InternalInsert(object, arrangement_id);

		//Split into 3 groups -- only ours, only new, and both ours and new
		std::set<std::shared_ptr<Object>> oldObservers, newObservers, bothObservers;

		object->ViewAwareObjects([&] (std::shared_ptr<Object>& observer) {
			oldObservers.insert(observer);
		});
	
		newContainer->ViewAwareObjects([&] (std::shared_ptr<Object>& observer) 
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
		});

		//Send Creates to only new
		for(auto& observer : newObservers) {
			object->AddAwareObject(observer);
		}

		//Send updates to both
		for(auto& observer : bothObservers) {
			object->SendUpdateContainmentMessage(observer->GetController());
		}

		//Send destroys to only ours
		for(auto& observer : oldObservers) {
			object->RemoveAwareObject(observer);
		}
	}
	else
	{
		LOG(warning) << "NOT TRANSFERRING " << object->GetObjectId() << " FROM " << GetObjectId() <<" DUE TO INSUFFICIENT PERMISSIONS";
	}
}

void Object::ViewObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func, std::shared_ptr<Object> hint)
{
	if(requester == nullptr || container_permissions_->canView(shared_from_this(), requester))
	{
		for(auto& slot : slot_descriptor_)
		{
			slot.second->view_objects([&] (const std::shared_ptr<Object>& object) {
				if(topDown)
					func(object);

				if(max_depth != 1)
					object->ViewObjects(requester, (max_depth == 0) ? 0 : max_depth-1, topDown, func);

				if(!topDown)
					func(object);
			});
		}
	}
}

int32_t Object::__InternalInsert(std::shared_ptr<Object> object, int32_t arrangement_id)
{
	if(arrangement_id == -2)
		arrangement_id = GetAppropriateArrangementId(object);

	if (arrangement_id < 4)
	{
		slot_descriptor_[arrangement_id]->insert_object(object);
	}
	else
	{
		auto& arrangement = object->slot_arrangements_[arrangement_id-4];
		for (auto& i : arrangement)
		{
			slot_descriptor_[i]->insert_object(object);
		}
	}
	object->SetArrangementId(arrangement_id);
	object->SetContainer(shared_from_this());

	//Because we may have calculated it internally, send the arrangement_id used back
	//To the caller so it can send the appropriate update.
	return arrangement_id;
}

void Object::SwapSlots(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, int32_t new_arrangement_id)
{
	for(auto& slot : slot_descriptor_)
	{
		slot.second->remove_object(object);
	}
	__InternalInsert(object, new_arrangement_id);


	ViewAwareObjects([&] (std::shared_ptr<Object> aware_object) {
		object->SendUpdateContainmentMessage(object->GetController());
	});
}

void Object::AddAwareObject(std::shared_ptr<swganh::object::Object> object)
{	
	auto find_itr = aware_objects_.find(object);
	if(find_itr == aware_objects_.end())
	{
		auto observer = object->GetController();
		aware_objects_.insert(object);

		if(observer)
		{
			Subscribe(observer);
			SendCreateByCrc(observer);
			CreateBaselines(observer);

			if(GetPermissions()->canView(shared_from_this(), object))
			{
				for(auto& slot : slot_descriptor_)
				{
					slot.second->view_objects([&] (const std::shared_ptr<Object>& v) {
						v->AddAwareObject(object);
					});
				}
			}
		}
	}
}

void Object::ViewAwareObjects(std::function<void(std::shared_ptr<swganh::object::Object>)> func)
{
	std::for_each(aware_objects_.begin(), aware_objects_.end(), func);
}

void Object::RemoveAwareObject(std::shared_ptr<swganh::object::Object> object)
{
	auto find_itr = aware_objects_.find(object);
	if(find_itr != aware_objects_.end())
	{
		auto observer = object->GetController();
		aware_objects_.erase(object);

		if(observer)
		{
			for(auto& slot : slot_descriptor_)
			{
				slot.second->view_objects([&] (const std::shared_ptr<Object>& v) {
					v->RemoveAwareObject(object);
				});
			}

			SendDestroy(observer);
			Unsubscribe(observer);
		}
	}
}

string Object::GetTemplate()
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
	return template_string_;
}
void Object::SetTemplate(const string& template_string)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
	    template_string_ = template_string;
    }
    GetEventDispatcher()->Dispatch(make_shared<ObjectEvent>
        ("Object::Template",shared_from_this()));
}
void Object::SetObjectId(uint64_t object_id)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
	object_id_ = object_id;
}
uint64_t Object::GetObjectId()
{
    return object_id_;
}

wstring Object::GetCustomName()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return custom_name_;
}

void Object::SetCustomName(wstring custom_name)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        custom_name_ = custom_name;
    }
    
    GetEventDispatcher()->Dispatch(make_shared<ObjectEvent>
        ("Object::CustomName",shared_from_this()));
}

bool Object::HasObservers()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return !observers_.empty();
}

void Object::Subscribe(const shared_ptr<ObserverInterface>& observer)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto find_iter = observers_.find(observer);

    if (find_iter == observers_.end())
    {
        observers_.insert(observer);
    }
}

void Object::Unsubscribe(const shared_ptr<ObserverInterface>& observer)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto find_iter = observers_.find(observer);

    if (find_iter != observers_.end())
    {
        observers_.erase(find_iter);
    }
}

void Object::NotifyObservers(const anh::ByteBuffer& message)
{
    NotifyObservers<anh::ByteBuffer>(message);
}

bool Object::IsDirty()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return !deltas_.empty();
}
void Object::ClearBaselines()
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    baselines_.clear();
}
void Object::ClearDeltas()
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    deltas_.clear();
}

BaselinesCacheContainer Object::GetBaselines()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return baselines_;
}

DeltasCacheContainer Object::GetDeltas(uint64_t viewer_id)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return deltas_;
}

void Object::AddDeltasUpdate(DeltasMessage message)
{
    NotifyObservers(message);

	boost::lock_guard<boost::mutex> lock(object_mutex_);
    deltas_.push_back(move(message));
}
void Object::AddBaselineToCache(swganh::messages::BaselinesMessage baseline)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    baselines_.push_back(move(baseline));
}

void Object::SetPosition(glm::vec3 position)
{
    {
	    boost::lock_guard<boost::mutex> lock(object_mutex_);
        position_ = position;
    }

    GetEventDispatcher()->Dispatch(make_shared<ObjectEvent>
        ("Object::Position",shared_from_this()));
}
glm::vec3 Object::GetPosition()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
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
void Object::SetOrientation(glm::quat orientation)
{
    {
	    boost::lock_guard<boost::mutex> lock(object_mutex_);
        orientation_ = orientation;
    }

    GetEventDispatcher()->Dispatch(make_shared<ObjectEvent>
        ("Object::Orientation",shared_from_this()));
}
glm::quat Object::GetOrientation()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	return orientation_;
}
void Object::FaceObject(const std::shared_ptr<Object>& object)
{
    auto target_position = object->GetPosition();
    {
	    boost::lock_guard<boost::mutex> lock(object_mutex_);
        
        // Create a mirror direction vector for the direction we want to face.
        glm::vec3 direction_vector = glm::normalize(target_position - position_);
        direction_vector.x = -direction_vector.x;

        // Create a lookat matrix from the direction vector and convert it to a quaternion.
        orientation_ = glm::toQuat(glm::lookAt(
                                     direction_vector, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));                        

        // If in the 3rd quadrant the signs need to be flipped.
        if (orientation_.y <= 0.0f && orientation_.w >= 0.0f) {
            orientation_.y = -orientation_.y;
            orientation_.w = -orientation_.w;
 
        }
    }
    GetEventDispatcher()->Dispatch(make_shared<ObjectEvent>
        ("Object::Orientation",shared_from_this()));
}

uint8_t Object::GetHeading()
{
    glm::quat tmp;
    {
	    boost::lock_guard<boost::mutex> lock(object_mutex_);
        tmp = orientation_;
    }

    if (tmp.y < 0.0f && tmp.w > 0.0f) {
        tmp.y *= -1;
		tmp.w *= -1;
    }
	
    return static_cast<uint8_t>(glm::angle(tmp) / 0.0625f);
}

void Object::SetContainer(const std::shared_ptr<ContainerInterface>& container)
{
    {
	    boost::lock_guard<boost::mutex> lock(object_mutex_);
        container_ = container;		
    }

    GetEventDispatcher()->Dispatch(make_shared<ObjectEvent>
        ("Object::Container",shared_from_this()));
}

shared_ptr<ContainerInterface> Object::GetContainer()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	return container_;
}

void Object::SetComplexity(float complexity)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        complexity_ = complexity;
    }
    
    GetEventDispatcher()->Dispatch(make_shared<ObjectEvent>
        ("Object::Complexity",shared_from_this()));
}

float Object::GetComplexity()
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
	return complexity_;
}

void Object::SetStfName(const string& stf_file_name, const string& stf_string)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        stf_name_file_ = stf_file_name;
        stf_name_string_ = stf_string;
    }

    GetEventDispatcher()->Dispatch(make_shared<ObjectEvent>
        ("Object::StfName",shared_from_this()));
}

string Object::GetStfNameFile()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	return stf_name_file_;
}

string Object::GetStfNameString()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	return stf_name_string_;
}

void Object::SetVolume(uint32_t volume)
{
    volume_ = volume;

    GetEventDispatcher()->Dispatch(make_shared<ObjectEvent>
        ("Object::Volume",shared_from_this()));
}

uint32_t Object::GetVolume()
{
	return volume_;
}

void Object::SetSceneId(uint32_t scene_id)
{
    scene_id_ = scene_id;
        
    GetEventDispatcher()->Dispatch(make_shared<ObjectEvent>
        ("Object::SceneId",shared_from_this()));
}

uint32_t Object::GetSceneId()
{
	return scene_id_;
}

int32_t Object::GetArrangementId()
{
	return arrangement_id_;
}

void Object::SetArrangementId(int32_t arrangement_id)
{
	arrangement_id_ = arrangement_id;
}

anh::EventDispatcher* Object::GetEventDispatcher()
{
    return event_dispatcher_;
}

void Object::SetEventDispatcher(anh::EventDispatcher* dispatcher)
{
    event_dispatcher_ = dispatcher;
}

void Object::CreateBaselines( std::shared_ptr<anh::observer::ObserverInterface> observer)
{
    GetEventDispatcher()->Dispatch(make_shared<ObserverEvent>
        ("Object::Baselines", shared_from_this(), observer));
}

void Object::SendCreateByCrc(std::shared_ptr<anh::observer::ObserverInterface> observer) 
{
	DLOG(warning) << "SENDING " << GetObjectId() << " CREATE TO " << observer->GetId();

	swganh::messages::SceneCreateObjectByCrc scene_object;
    scene_object.object_id = GetObjectId();
    scene_object.object_crc = anh::memcrc(GetTemplate());
    scene_object.position = GetPosition();
	scene_object.orientation = GetOrientation();
    scene_object.byte_flag = 0;
    observer->Notify(scene_object);

	SendUpdateContainmentMessage(observer);
}

void Object::SendUpdateContainmentMessage(std::shared_ptr<anh::observer::ObserverInterface> observer)
{
	uint64_t container_id = 0;
	if (GetContainer())
		container_id = GetContainer()->GetObjectId();

	UpdateContainmentMessage containment_message;
	containment_message.container_id = container_id;
	containment_message.object_id = GetObjectId();
	containment_message.containment_type = arrangement_id_;
	observer->Notify(containment_message);
}

void Object::SendDestroy(std::shared_ptr<anh::observer::ObserverInterface> observer)
{
	LOG(warning) << "OBJECT: " << GetObjectId() <<" SENDING DESTROY TO " << observer->GetId();
	swganh::messages::SceneDestroyObject scene_object;
	scene_object.object_id = GetObjectId();
	observer->Notify(scene_object);
}

void Object::SetFlag(std::string flag)
{
    boost::lock_guard<boost::mutex> lg(flags_mutex_);
    flags_.insert(flag);
}

void Object::RemoveFlag(std::string flag)
{
    boost::lock_guard<boost::mutex> lg(flags_mutex_);
    flags_.erase(flag);
}

bool Object::HasFlag(std::string flag)
{
    boost::lock_guard<boost::mutex> lg(flags_mutex_);

    return flags_.find(flag) != flags_.end();
}

void Object::LockObjectMutex()
{
	object_mutex_.lock();
}

void Object::UnlockObjectMutex()
{
	object_mutex_.unlock();
}

/// Slots

void Object::SetSlotInformation(ObjectSlots slots, ObjectArrangements arrangements)
{
	boost::lock_guard<boost::mutex> lg(object_mutex_);
	slot_descriptor_ = slots;
	slot_arrangements_ = arrangements;
}

int32_t Object::GetAppropriateArrangementId(std::shared_ptr<Object> other)
{
	if (slot_descriptor_.size() == 0)
		return -1;

	// Find appropriate arrangement
	int32_t arrangement_id = 4;
	int32_t filled_arrangement_id = 0;
	// In each arrangment
	for ( auto& arrangement : other->slot_arrangements_)
	{
		bool passes_completely = true;
		bool is_valid = true;
		// Each Slot
		for (auto& slot : arrangement)
		{
			// does slot exist in descriptor
			auto& descriptor_iter = slot_descriptor_.find(slot);
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
	boost::lock_guard<boost::mutex> lg(object_mutex_);
	return slot_descriptor_;
}
ObjectArrangements Object::GetSlotArrangements()
{
	boost::lock_guard<boost::mutex> lg(object_mutex_);
	return slot_arrangements_;
}
bool Object::ClearSlot(int32_t slot_id)
{
	boost::lock_guard<boost::mutex> lg(object_mutex_);
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
shared_ptr<Object> Object::GetSlotObject(int32_t slot_id)
{
	boost::lock_guard<boost::mutex> lg(object_mutex_);
	shared_ptr<Object> found = nullptr;
	auto slot_iter = slot_descriptor_.find(slot_id);
	if (slot_iter != slot_descriptor_.end())
	{
		auto slot = slot_iter->second;
		if (!slot->is_filled())
		{			
			slot->view_objects([&](shared_ptr<Object> object){
				found = object;
			});
		}
	}
	return found;
}