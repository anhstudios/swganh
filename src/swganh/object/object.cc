// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "object.h"

#include <glm/gtx/transform2.hpp>

#include "object_events.h"

#include "anh/logger.h"
#include "anh/crc.h"
#include "anh/observer/observer_interface.h"
#include "swganh/messages/base_baselines_message.h"
#include "swganh/messages/scene_create_object_by_crc.h"
#include "swganh/messages/scene_end_baselines.h"
#include "swganh/messages/update_containment_message.h"

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

void Object::AddContainedObject(shared_ptr<Object> object)
{
	int32_t arrangement_id = -2;
    {
	    boost::lock_guard<boost::mutex> lock(object_mutex_);
		arrangement_id = GetAppropriateArrangementId(object);
    }
	
	AddContainedObject(object, arrangement_id);
}

void Object::AddContainedObject(std::shared_ptr<Object> object, int32_t arrangement_id)
{
    {
	    boost::lock_guard<boost::mutex> lock(object_mutex_);

		// Add to first slot if can't find appropriate
		if (arrangement_id < 4)
			slot_descriptor_[arrangement_id]->insert_object(object);
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
    }
			
    if (HasController())
    {
        object->Subscribe(GetController());
		object->MakeClean(GetController());
    }
}

bool Object::IsContainerForObject(const shared_ptr<Object>& object)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    /*return contained_objects_.find(object->GetObjectId()) != contained_objects_.end();*/
	return false;
}

void Object::RemoveContainedObject(const shared_ptr<Object>& object)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    
	for (auto& slot : slot_descriptor_)
	{
		slot.second->remove_object(object);
	}

    if (HasController())
    {
        object->Unsubscribe(GetController());
    }
}

Object::ObjectMap Object::GetContainedObjects()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    ObjectMap object_map;

	for (auto& descriptor : slot_descriptor_)
	{
		descriptor.second->view_objects([&](const std::shared_ptr<Object> other){
			object_map.insert(ObjectMap::value_type(other->GetObjectId(), other));
		});
	}

	return object_map;
}

void Object::AddAwareObject(const shared_ptr<Object>& object)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        if (aware_objects_.find(object->GetObjectId()) == aware_objects_.end())
        {
            aware_objects_.insert(make_pair(object->GetObjectId(), object));
        }
    }
    if (object->HasController()) {
        Subscribe(object->GetController());
        MakeClean(object->GetController());
    }
}

bool Object::IsAwareOfObject(const shared_ptr<Object>& object)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return aware_objects_.find(object->GetObjectId()) != aware_objects_.end();
}

void Object::RemoveAwareObject(const shared_ptr<Object>& object)
{
    {
	    boost::lock_guard<boost::mutex> lock(object_mutex_);
        auto find_iter = aware_objects_.find(object->GetObjectId());

        if (find_iter != aware_objects_.end())
        {
            return;
        }

        aware_objects_.erase(find_iter);
    }

    if (HasController())
    {
        object->Unsubscribe(GetController());
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
    auto find_iter = std::find_if(
        observers_.begin(),
        observers_.end(),
        [&observer] (const std::shared_ptr<ObserverInterface>& stored_observer)
    {
        return observer == stored_observer;
    });

    if (find_iter != observers_.end())
    {
        return;
    }

    observers_.push_back(observer);
}

void Object::Unsubscribe(const shared_ptr<ObserverInterface>& observer)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto find_iter = std::find_if(
        observers_.begin(),
        observers_.end(),
        [&observer] (const std::shared_ptr<ObserverInterface>& stored_observer)
    {
        return observer == stored_observer;
    });

    if (find_iter == observers_.end())
    {
        return;
    }

    observers_.erase(find_iter);
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
void Object::MakeClean(std::shared_ptr<swganh::object::ObjectController> controller)
{
    ClearBaselines();
    ClearDeltas();
    // SceneCreateObjectByCrc
    swganh::messages::SceneCreateObjectByCrc scene_object;
    scene_object.object_id = GetObjectId();
    scene_object.object_crc = anh::memcrc(GetTemplate());
    scene_object.position = GetPosition();
	scene_object.orientation = GetOrientation();
    scene_object.byte_flag = 0;
    controller->Notify(scene_object);

    CreateBaselines(controller);

    if (GetContainer())
    {
        UpdateContainmentMessage containment_message;
        containment_message.container_id = GetContainer()->GetObjectId();
        containment_message.object_id = GetObjectId();
        containment_message.containment_type = arrangement_id_;

        controller->Notify(containment_message);
    }
    
    OnMakeClean(controller);
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
        tmp.w *= -1;
    }

    return static_cast<uint8_t>((glm::angle(tmp) / 6.283f) * 100);
}

void Object::SetContainer(const std::shared_ptr<Object>& container)
{
    {
	    boost::lock_guard<boost::mutex> lock(object_mutex_);
        container_ = container;
    }

    GetEventDispatcher()->Dispatch(make_shared<ObjectEvent>
        ("Object::Container",shared_from_this()));
}

shared_ptr<Object> Object::GetContainer()
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

void Object::CreateBaselines( std::shared_ptr<ObjectController> controller)
{
    GetEventDispatcher()->Dispatch(make_shared<ControllerEvent>
        ("Object::Baselines", shared_from_this(), controller));
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