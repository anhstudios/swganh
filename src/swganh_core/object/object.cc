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

#include "swganh/object/permissions/container_permissions_interface.h"

using namespace swganh::observer;
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
	, database_persisted_(true)
	, in_snapshot_(false)
{
	menu_response_ = make_shared<swganh::messages::controllers::ObjectMenuResponse>();
}

bool Object::HasController()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return controller_ != nullptr;
}

shared_ptr<ObserverInterface> Object::GetController()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return controller_;
}

void Object::SetController(const shared_ptr<ObserverInterface>& controller)
{
    {
	    boost::lock_guard<boost::mutex> lock(object_mutex_);
        controller_ = controller;
    }

    Subscribe(controller);
}

void Object::ClearController()
{
    shared_ptr<ObserverInterface> controller;

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
		boost::upgrade_lock<boost::shared_mutex> lock(global_container_lock_);
		
		//Add Object To Datastructure
		{
			boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock(lock);
			arrangement_id = __InternalInsert(obj, arrangement_id);
		}

		//Update our observers with the new object
		std::for_each(aware_objects_.begin(), aware_objects_.end(), [&] (std::shared_ptr<Object> object) {
			obj->__InternalAddAwareObject(object);		
			object->__InternalAddAwareObject(obj);
		});
	}
}

void Object::RemoveObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> oldObject)
{
	if(requester == nullptr || container_permissions_->canRemove(shared_from_this(), requester, oldObject))
	{
		boost::upgrade_lock<boost::shared_mutex> lock(global_container_lock_);

		//Update our observers about the dead object
		std::for_each(aware_objects_.begin(), aware_objects_.end(), [&] (std::shared_ptr<Object> object) {
			oldObject->__InternalRemoveAwareObject(object);	
			object->__InternalRemoveAwareObject(oldObject);
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

void Object::TransferObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, int32_t arrangement_id)
{
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

			arrangement_id = newContainer->__InternalInsert(object, arrangement_id);
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
		});

		//Send Creates to only new
		for(auto& observer : newObservers) {
			object->__InternalAddAwareObject(observer);
		}

		//Send updates to both
		for(auto& observer : bothObservers) {
			object->SendUpdateContainmentMessage(observer->GetController());
		}

		//Send destroys to only ours
		for(auto& observer : oldObservers) {
			object->__InternalRemoveAwareObject(observer);
		}
	}
}

void Object::__InternalViewObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func)
{
	if(requester == nullptr || container_permissions_->canView(shared_from_this(), requester))
	{
		for(auto& slot : slot_descriptor_)
		{
			slot.second->view_objects([&] (const std::shared_ptr<Object>& object) {
				if(topDown)
					func(object);

				if(max_depth != 1)
					object->__InternalViewObjects(requester, (max_depth == 0) ? 0 : max_depth-1, topDown, func);

				if(!topDown)
					func(object);
			});
		}
	}
}

int32_t Object::__InternalInsert(std::shared_ptr<Object> object, int32_t arrangement_id)
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
			// Remove object in existing slot
			removed_object = slot_descriptor_[i]->insert_object(object);			
			if (removed_object)
			{
				// Transfer it out, put it in the place the replacing object came from
				removed_object->__InternalTransfer(nullptr, removed_object, object->GetContainer());
			}
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
	boost::upgrade_lock<boost::shared_mutex> uplock(global_container_lock_);
	
	{
		boost::upgrade_to_unique_lock<boost::shared_mutex> unique(uplock);
		for(auto& slot : slot_descriptor_)
		{
			slot.second->remove_object(object);
		}
		__InternalInsert(object, new_arrangement_id);
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
			arrangement_id = newContainer->__InternalInsert(object, arrangement_id);

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
		});

		//Send Creates to only new
		for(auto& observer : newObservers) {
			object->__InternalAddAwareObject(observer);
		}

		//Send updates to both
		for(auto& observer : bothObservers) {
			object->SendUpdateContainmentMessage(observer->GetController());
		}

		//Send destroys to only ours
		for(auto& observer : oldObservers) {
			object->__InternalRemoveAwareObject(observer);
		}
	}
}


void Object::__InternalAddAwareObject(std::shared_ptr<swganh::object::Object> object)
{	
	auto find_itr = aware_objects_.find(object);
	if(find_itr == aware_objects_.end())
	{
		auto observer = object->GetController();
		aware_objects_.insert(object);

		if(observer)
		{
			if(!IsInSnapshot())
			{
				DLOG(info) << "SENDING " << GetObjectId() << " TO " << observer->GetId();
				Subscribe(observer);
				SendCreateByCrc(observer);
				CreateBaselines(observer);
			}

			if(GetPermissions()->canView(shared_from_this(), object))
			{
				for(auto& slot : slot_descriptor_)
				{
					slot.second->view_objects([&] (const std::shared_ptr<Object>& v) {
						v->__InternalAddAwareObject(object);
					});
				}
			}
		}
	}
}

void Object::__InternalViewAwareObjects(std::function<void(std::shared_ptr<swganh::object::Object>)> func, std::shared_ptr<swganh::object::Object> hint)
{
	std::for_each(aware_objects_.begin(), aware_objects_.end(), func);
}

void Object::__InternalRemoveAwareObject(std::shared_ptr<swganh::object::Object> object)
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
					v->__InternalRemoveAwareObject(object);
				});
			}

			if(!IsInSnapshot())
			{
				DLOG(info) << "DELETING " << GetObjectId() << " FOR " << observer->GetId();
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

glm::vec3 Object::__InternalGetAbsolutePosition()
{
	glm::vec3 parentPos = GetContainer()->__InternalGetAbsolutePosition();

	boost::lock_guard<boost::mutex> lock(object_mutex_);
	return glm::vec3(parentPos.x + position_.x, parentPos.y + position_.y, parentPos.z + position_.z);
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

void Object::NotifyObservers(swganh::messages::BaseSwgMessage* message)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);

    std::for_each(
        observers_.begin(),
        observers_.end(),
        [&message] (const std::shared_ptr<swganh::observer::ObserverInterface>& observer)
    {
        observer->Notify(message);
    });
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

void Object::AddDeltasUpdate(DeltasMessage* message)
{
    NotifyObservers(message);

	boost::lock_guard<boost::mutex> lock(object_mutex_);
    deltas_.push_back(*message);
}
void Object::AddBaselineToCache(swganh::messages::BaselinesMessage* baseline)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    baselines_.push_back(*baseline);
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

    float heading = 0.0f;

    if (glm::length(tmp) > 0.0f)
    {
        float s = sqrt(1 - (tmp.w * tmp.w));
        if (s != 0.0f)
        {
            if (tmp.y < 0.0f && tmp.w > 0.0f) 
            {
                tmp.y *= -1;
	        	tmp.w *= -1;
            }

			float radians = 2.0f * acos(tmp.w);
			float t = radians / 0.06283f;
			heading = (tmp.y / s) * t;
        }
    }

	return static_cast<uint8_t>(heading);
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
    GetEventDispatcher()->Dispatch(make_shared<ObserverEvent>
        ("Object::Baselines", shared_from_this(), observer));
}

void Object::SendCreateByCrc(std::shared_ptr<swganh::observer::ObserverInterface> observer) 
{
	DLOG(info) << "SEND " << GetObjectId() << " TO " << observer->GetId();

	swganh::messages::SceneCreateObjectByCrc scene_object;
    scene_object.object_id = GetObjectId();
    scene_object.object_crc = swganh::memcrc(GetTemplate());
    scene_object.position = GetPosition();
	scene_object.orientation = GetOrientation();
    scene_object.byte_flag = 0;
    observer->Notify(&scene_object);

	SendUpdateContainmentMessage(observer);
}

void Object::SendUpdateContainmentMessage(std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
	if(observer == nullptr)
		return;

	uint64_t container_id = 0;
	if (GetContainer())
		container_id = GetContainer()->GetObjectId();

	DLOG(info) << "CONTAINMENT " << GetObjectId() << " INTO " << container_id << " ARRANGEMENT " << arrangement_id_;

	UpdateContainmentMessage containment_message;
	containment_message.container_id = container_id;
	containment_message.object_id = GetObjectId();
	containment_message.containment_type = arrangement_id_;
	observer->Notify(&containment_message);
}

void Object::SendDestroy(std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
	DLOG(info) << "DESTROY " << GetObjectId() << " FOR " << observer->GetId();

	swganh::messages::SceneDestroyObject scene_object;
	scene_object.object_id = GetObjectId();
	observer->Notify(&scene_object);
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

/// Slots

void Object::SetSlotInformation(ObjectSlots slots, ObjectArrangements arrangements)
{
	boost::lock_guard<boost::mutex> lg(object_mutex_);
	slot_descriptor_ = slots;
	slot_arrangements_ = arrangements;
}

int32_t Object::GetAppropriateArrangementId(std::shared_ptr<Object> other)
{
	if (slot_descriptor_.size() == 1)
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

void Object::SetMenuResponse(std::vector<swganh::messages::controllers::RadialOptions> radials)
{
	{
		boost::lock_guard<boost::mutex> lg(object_mutex_);
		menu_response_->radial_options = radials;
	}
	GetEventDispatcher()->Dispatch(make_shared<ObjectEvent>
        ("Object::SetMenuResponse", shared_from_this()));
}

std::shared_ptr<swganh::messages::controllers::ObjectMenuResponse> Object::GetMenuResponse()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	return menu_response_;
}

bool Object::IsDatabasePersisted()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	return database_persisted_;
}

bool Object::IsInSnapshot()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	return in_snapshot_;
}

void Object::SetDatabasePersisted(bool value)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	database_persisted_ = value;
}

void Object::SetInSnapshot(bool value)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	in_snapshot_ = value;
}

AttributesMap Object::GetAttributeMap()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	return attributes_map_;
}

boost::variant<float, int32_t, std::wstring> Object::GetAttribute(const std::string& name)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	auto find_iter = find_if(attributes_map_.begin(), attributes_map_.end(), [&](AttributesMap::value_type key_value)
	{
		return key_value.first == name;
	});
	if (find_iter != attributes_map_.end())
	{
		return find_iter->second;
	}	
	LOG(error) << "Attribute "<< name << " does not exist";	
	return L"";
	//throw std::runtime_error("Attribute " + name + " does not exist");
}

std::wstring Object::GetAttributeAsString(const std::string& name)
{
	try {
		auto val = GetAttribute(name);		
		wstringstream attribute;
		switch (val.which())
		{
			// float
			case 0:
				attribute << boost::get<float>(val);
				break;
			case 1:
				attribute << boost::get<int32_t>(val);
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

uint8_t Object::GetAttributeTemplateId()
{
	return attributes_template_id;
}

void Object::SetAttributeTemplateId(uint8_t attribute_template_id)
{
	attributes_template_id = attribute_template_id;
}

std::wstring Object::GetAttributeRecursiveAsString(const std::string& name)
{
	auto val = GetAttributeRecursive(name);
	wstringstream ss;
	switch(val.which())
		{
			// float
			case 0:
				 ss << boost::get<float>(val);
				break;
			case 1:
				ss << boost::get<int32_t>(val);
				break;
			case 2:
				ss << boost::get<wstring>(val);
				break;
		}		
	
	return ss.str();
}
boost::variant<float, int32_t, std::wstring> Object::GetAttributeRecursive(const std::string& name)
{
	auto val = GetAttribute(name);
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		float float_val;
		int32_t int_val;
		wstring attr_val;
		switch(val.which())
		{
			// float
			case 0:
				 float_val = boost::get<float>(val);
				return AddAttributeRecursive<float>(float_val, name);			
			case 1:
				int_val = boost::get<int32_t>(val);
				return AddAttributeRecursive<int32_t>(int_val, name);			
			case 2:
				attr_val = boost::get<wstring>(val);
				return AddAttributeRecursive<wstring>(attr_val, name);			
		}	
		return boost::get<wstring>(val);
	}
}