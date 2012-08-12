// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "object.h"

#include <glm/gtx/transform2.hpp>

#include "object_events.h"

#include "anh/logger.h"
#include "anh/crc.h"
#include "anh/observer/observer_interface.h"


#include "swganh/messages/scene_create_object_by_crc.h"
#include "swganh/messages/update_containment_message.h"
#include "swganh/messages/scene_destroy_object.h"

#include "swganh/messages/base_baselines_message.h"
#include "swganh/messages/scene_end_baselines.h"


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

void Object::AddObject(std::shared_ptr<Object> newObject)
{
	LOG(warning) << "INSERTING " << newObject->GetObjectId() << " INTO " << this->GetObjectId();
	//Add Object To Datastructure
	contained_objects_.insert(ObjectMap::value_type(newObject->GetObjectId(), newObject));
	newObject->SetContainer(shared_from_this());

	//Update our observers with the new object
	std::for_each(aware_objects_.begin(), aware_objects_.end(), [&] (std::shared_ptr<Object> object) {
		newObject->AddAwareObject(object);		
	});
}

void Object::RemoveObject(std::shared_ptr<Object> oldObject)
{
	LOG(warning) << "REMOVING " << oldObject->GetObjectId() << " FROM " << this->GetObjectId();
	
	auto itr = contained_objects_.find(oldObject->GetObjectId());
	if(itr != contained_objects_.end())
	{
		//Update our observers about the dead object
		std::for_each(aware_objects_.begin(), aware_objects_.end(), [&] (std::shared_ptr<Object> object) {
			oldObject->RemoveAwareObject(object);		
		});

		//Remove Object from Datastructure
		contained_objects_.erase(itr);
		oldObject->SetContainer(nullptr);
	}
}

void Object::TransferObject(std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer)
{
	LOG(warning) << "TRANSFER " << object->GetObjectId() << " FROM " << this->GetObjectId() << " TO " << newContainer->GetObjectId();
	//Perform the transfer
	auto itr = contained_objects_.find(object->GetObjectId());
	if(itr != contained_objects_.end())
	{
		contained_objects_.erase(itr);
		newContainer->__InternalInsert(object);

		//Split into 3 groups -- only ours, only new, and both ours and new
		std::set<std::shared_ptr<Object>> oldObservers, newObservers, bothObservers;

		ViewAwareObjects([&] (std::shared_ptr<Object>& observer) {
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
}

void Object::ViewObjects(uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func, std::shared_ptr<Object> hint)
{
	for(auto& object : contained_objects_)
	{
		if(topDown)
			func(object.second);

		if(max_depth != 1)
			object.second->ViewObjects((max_depth == 0) ? 0 : max_depth-1, topDown, func);

		if(!topDown)
			func(object.second);
	}
}

void Object::__InternalInsert(std::shared_ptr<Object> object)
{
	LOG(warning) << "INTERNAL_INSERTING " << object->GetObjectId() << " INTO " << this->GetObjectId();
	contained_objects_.insert(ObjectMap::value_type(object->GetObjectId(), object));
	object->SetContainer(shared_from_this());
}

void Object::AddAwareObject(std::shared_ptr<swganh::object::Object> object)
{
	auto observer = object->GetController();
	aware_objects_.insert(object);

	if(observer)
	{
		Subscribe(observer);
		SendCreateByCrc(observer);
		CreateBaselines(observer);

		for(auto& v : contained_objects_)
		{
			v.second->AddAwareObject(object);
		}
	}
}

void Object::ViewAwareObjects(std::function<void(std::shared_ptr<swganh::object::Object>)> func)
{
	std::for_each(aware_objects_.begin(), aware_objects_.end(), func);
}

void Object::RemoveAwareObject(std::shared_ptr<swganh::object::Object> object)
{
	auto observer = object->GetController();
	aware_objects_.erase(object);

	if(observer)
	{
		for(auto& v : contained_objects_)
		{
			v.second->RemoveAwareObject(object);
		}

		SendDestroy(observer);
		Unsubscribe(observer);
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
void Object::MakeClean(std::shared_ptr<anh::observer::ObserverInterface> observer)
{
    ClearBaselines();
    ClearDeltas();
    // SceneCreateObjectByCrc
    

    CreateBaselines(observer);

    
    
    OnMakeClean(observer);
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
	containment_message.containment_type = 4;
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