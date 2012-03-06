
#include "swganh/object/object.h"

#include "anh/crc.h"
#include "anh/observer/observer_interface.h"
#include "swganh/messages/base_baselines_message.h"
#include "swganh/messages/scene_create_object_by_crc.h"
#include "swganh/messages/scene_end_baselines.h"
#include "swganh/messages/update_containment_message.h"
#include "swganh/object/object_message_builder.h"

using namespace anh::observer;
using namespace std;
using namespace swganh::object;
using namespace swganh::messages;
using boost::optional;

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
	AddBaselinesBuilders_();
}

bool Object::HasController() 
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return controller_ != nullptr;
}

shared_ptr<ObjectController> Object::GetController()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return controller_;
}

void Object::SetController(const shared_ptr<ObjectController>& controller)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    controller_ = controller;
    Subscribe(controller_);
}

void Object::ClearController()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    Unsubscribe(controller_);
    controller_.reset();
}

void Object::AddContainedObject(const shared_ptr<Object>& object, ContainmentType containment_type)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    if (contained_objects_.find(object->GetObjectId()) != contained_objects_.end())
    {
        // @TODO consider whether encountering this scenario is an error
        return;
    }

    contained_objects_.insert(make_pair(object->GetObjectId(), object));
    object->SetContainer(shared_from_this());

    if (HasController())
    {
        object->Subscribe(GetController());
    }
}

bool Object::IsContainerForObject(const shared_ptr<Object>& object)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return contained_objects_.find(object->GetObjectId()) != contained_objects_.end();
}

void Object::RemoveContainedObject(const shared_ptr<Object>& object)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    auto find_iter = contained_objects_.find(object->GetObjectId());

    if (find_iter == contained_objects_.end())
    {
        // @TODO consider whether encountering this scenario is an error
        return;
    }

    contained_objects_.erase(find_iter);    

    if (HasController())
    {
        object->Unsubscribe(GetController());
    }
}

Object::ObjectMap Object::GetContainedObjects()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return contained_objects_;
}

void Object::AddAwareObject(const shared_ptr<Object>& object)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    if (aware_objects_.find(object->GetObjectId()) != aware_objects_.end())
    {
        // @TODO consider whether encountering this scenario is an error
		// someone could be logging back in and we want to make them aware again, so we'll send clean baselines.
		if (object->HasController()) {
			MakeClean(object->GetController());
		}
        return;
    }

    aware_objects_.insert(make_pair(object->GetObjectId(), object));
    
    if (object->HasController()) {
        Subscribe(object->GetController());
        MakeClean(object->GetController());
    }
}

bool Object::IsAwareOfObject(const shared_ptr<Object>& object)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return aware_objects_.find(object->GetObjectId()) != aware_objects_.end();
}

void Object::RemoveAwareObject(const shared_ptr<Object>& object)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    auto find_iter = aware_objects_.find(object->GetObjectId());

    if (find_iter == aware_objects_.end())
    {
        // @TODO consider whether encountering this scenario is an error
        return;
    }

    aware_objects_.erase(find_iter);

    if (HasController())
    {
        object->Unsubscribe(GetController());
    }
}
const string& Object::GetTemplate()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	return template_string_;
}
void Object::SetTemplate(const string& template_string)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	template_string_ = template_string;
}
void Object::SetObjectId(uint64_t object_id)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	object_id_ = object_id;
}
uint64_t Object::GetObjectId()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return object_id_;
}

const wstring& Object::GetCustomName()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return custom_name_;
}

void Object::SetCustomName(wstring custom_name)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    custom_name_ = custom_name;
    
    // Only build a message if there are observers.
    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_3, 2);
        message.data.write(custom_name);
    
        AddDeltasUpdate(message);    
    }
}

BaselinesMessage Object::CreateBaselinesMessage(uint8_t view_type, uint16_t opcount)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    BaselinesMessage message;
    message.object_id = GetObjectId();
    message.object_type = GetType();
    message.view_type = view_type;
    message.object_opcount = opcount;
    
    return message;
}

DeltasMessage Object::CreateDeltasMessage(uint8_t view_type, uint16_t update_type, uint16_t update_count) 
{        
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    DeltasMessage message;
    message.object_id = GetObjectId();
    message.object_type = GetType();
    message.view_type = view_type;
    message.update_count = update_count;
    message.update_type = update_type;
    return message;
}

bool Object::HasObservers()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return !observers_.empty();
}

void Object::Subscribe(const shared_ptr<ObserverInterface>& observer)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
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
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
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
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    NotifyObservers<anh::ByteBuffer>(message);
}

bool Object::IsDirty() 
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return !deltas_.empty();
}

void Object::MakeClean(std::shared_ptr<swganh::object::ObjectController> controller)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    baselines_.clear();
    deltas_.clear();
    
    // SceneCreateObjectByCrc
    swganh::messages::SceneCreateObjectByCrc scene_object;
    scene_object.object_id = GetObjectId();
    scene_object.object_crc = anh::memcrc(GetTemplate());
    scene_object.position = GetPosition();
	scene_object.orientation = GetOrientation();
    controller->Notify(scene_object);
         
    if (GetContainer())
    {
        UpdateContainmentMessage containment_message;
        containment_message.container_id = GetContainer()->GetObjectId();
        containment_message.object_id = GetObjectId();
        containment_message.containment_type = 4;
    
        controller->Notify(containment_message);
    }

    // Baselines
    optional<BaselinesMessage> message;
    for_each(begin(baselines_builders_), end(baselines_builders_),
        [this, &message, &controller] (BaselinesBuilder& builder)
    {   
        if (!(message = builder()))
        {
            return;
        }

        controller->Notify(*message);
        baselines_.push_back(*message);
    });

    // SceneEndBaselines
    swganh::messages::SceneEndBaselines scene_end_baselines;
    scene_end_baselines.object_id = GetObjectId();
    controller->Notify(scene_end_baselines);

    OnMakeClean(controller);
}

const BaselinesCacheContainer& Object::GetBaselines(uint64_t viewer_id) 
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return baselines_;
}

const DeltasCacheContainer& Object::GetDeltas(uint64_t viewer_id) 
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return deltas_;
}

void Object::AddDeltasUpdate(DeltasMessage message)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    NotifyObservers(message);
    deltas_.push_back(move(message));
}

void Object::AddBaselinesBuilders_()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    baselines_builders_.push_back([this] () {
        return GetBaseline1();
    });
    
    baselines_builders_.push_back([this] () {
        return GetBaseline2();
    });
    
    baselines_builders_.push_back([this] () {
        return GetBaseline3();
    });
    
    baselines_builders_.push_back([this] () {
        return GetBaseline4();
    });
    
    baselines_builders_.push_back([this] () {
        return GetBaseline5();
    });
    
    baselines_builders_.push_back([this] () {
        return GetBaseline6();
    });

    baselines_builders_.push_back([this] () {
        return GetBaseline7();
    });

    baselines_builders_.push_back([this] () {
        return GetBaseline8();
    });

    baselines_builders_.push_back([this] () {
        return GetBaseline9();
    });
}

void Object::SetPosition(glm::vec3 position)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    position_ = position;
}
glm::vec3 Object::GetPosition()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
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
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    orientation_ = orientation;
}
glm::quat Object::GetOrientation()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	return orientation_;
}

uint8_t Object::GetHeading() 
{  
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    glm::quat tmp = orientation_;
    
    if (tmp.y < 0.0f && tmp.w > 0.0f) {
        tmp.w *= -1;
    }
    
    return static_cast<uint8_t>((glm::angle(tmp) / 6.283f) * 100);
}

void Object::SetContainer(const std::shared_ptr<Object>& container)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    container_ = container;
}

shared_ptr<Object> Object::GetContainer()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	return container_;
}

void Object::SetComplexity(float complexity)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    complexity_ = complexity;
    ObjectMessageBuilder::BuildComplexityDelta(this);
}

float Object::GetComplexity()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	return complexity_;
}

void Object::SetStfName(const string& stf_file_name, const string& stf_string)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    stf_name_file_ = stf_file_name;
    stf_name_string_ = stf_string;
    ObjectMessageBuilder::BuildStfNameDelta(this);
}

const string& Object::GetStfNameFile()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	return stf_name_file_;
}

const string& Object::GetStfNameString()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	return stf_name_string_;
}

void Object::SetVolume(uint32_t volume)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    volume_ = volume;
    ObjectMessageBuilder::BuildVolumeDelta(this);
}

uint32_t Object::GetVolume()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	return volume_;
}

void Object::SetSceneId(uint32_t scene_id)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	scene_id_ = scene_id;
}

uint32_t Object::GetSceneId()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	return scene_id_;
}

optional<BaselinesMessage> Object::GetBaseline3()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	return std::move(ObjectMessageBuilder::BuildBaseline3(this));
}

optional<BaselinesMessage> Object::GetBaseline6()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
	return std::move(ObjectMessageBuilder::BuildBaseline6(this));
}
