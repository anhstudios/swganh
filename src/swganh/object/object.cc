
#include "swganh/object/object.h"

#include "anh/crc.h"
#include "anh/observer/observer_interface.h"
#include "swganh/messages/base_baselines_message.h"
#include "swganh/messages/scene_create_object_by_crc.h"
#include "swganh/messages/scene_end_baselines.h"
#include "swganh/messages/update_containment_message.h"
#include "swganh/object/object_controller.h"
#include "swganh/object/object_message_builder.h"
#include "swganh/network/remote_client.h"

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

bool Object::HasController() const
{
    return controller_;
}

const shared_ptr<ObjectController>& Object::GetController() const
{
    return controller_;
}

void Object::SetController(const shared_ptr<ObjectController>& controller)
{
    controller_ = controller;
    Subscribe(controller_);
}

void Object::ClearController()
{
    Unsubscribe(controller_);
    controller_.reset();
}

void Object::AddContainedObject(const shared_ptr<Object>& object, uint32_t containment_type)
{
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
    return contained_objects_.find(object->GetObjectId()) != contained_objects_.end();
}

void Object::RemoveContainedObject(const shared_ptr<Object>& object)
{
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
    return contained_objects_;
}

void Object::AddAwareObject(const shared_ptr<Object>& object)
{
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
    return aware_objects_.find(object->GetObjectId()) != aware_objects_.end();
}

void Object::RemoveAwareObject(const shared_ptr<Object>& object)
{
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

uint64_t Object::GetObjectId() const
{
    return object_id_;
}

wstring Object::GetCustomName() const
{
    return custom_name_;
}

void Object::SetCustomName(wstring custom_name)
{
    custom_name_ = custom_name;
    
    // Only build a message if there are observers.
    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_3, 2);
        message.data.write(custom_name);
    
        AddDeltasUpdate(message);    
    }
}

BaselinesMessage Object::CreateBaselinesMessage(uint16_t view_type, uint16_t opcount) const
{
    BaselinesMessage message;
    message.object_id = GetObjectId();
    message.object_type = GetType();
    message.view_type = view_type;
    message.object_opcount = opcount;
    
    return message;
}

DeltasMessage Object::CreateDeltasMessage(uint16_t view_type, uint16_t update_type, uint16_t update_count) const
{        
    DeltasMessage message;
    message.object_id = GetObjectId();
    message.object_type = GetType();
    message.view_type = view_type;
    message.update_count = update_count;
    message.update_type = update_type;
    return message;
}

bool Object::HasObservers() const
{
    return !observers_.empty();
}

void Object::Subscribe(const shared_ptr<ObserverInterface>& observer)
{
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

bool Object::IsDirty() const
{
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
}

const BaselinesCacheContainer& Object::GetBaselines(uint64_t viewer_id) const
{
    return baselines_;
}

const DeltasCacheContainer& Object::GetDeltas(uint64_t viewer_id) const
{
    return deltas_;
}

void Object::AddDeltasUpdate(DeltasMessage message)
{
    NotifyObservers(message);
    deltas_.push_back(move(message));
}

void Object::AddBaselinesBuilders_()
{
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
    position_ = position;
}
void Object::SetOrientation(glm::quat orientation)
{
    orientation_ = orientation;
}

uint8_t Object::GetHeading() const
{  
    glm::quat tmp = orientation_;
    
    if (tmp.y < 0.0f && tmp.w > 0.0f) {
        tmp.w *= -1;
    }
    
    return static_cast<uint8_t>((glm::angle(tmp) / 6.283f) * 100);
}

void Object::SetContainer(const std::shared_ptr<Object>& container)
{
    container_ = container;
}
void Object::SetComplexity(float complexity)
{
    complexity_ = complexity;
    ObjectMessageBuilder::BuildComplexityDelta(this);
}
void Object::SetStfNameFile(const string& stf_name_file)
{
    stf_name_file_ = stf_name_file;
    ObjectMessageBuilder::BuildStfNameDelta(this);
}
void Object::SetStfNameString(const string& stf_name_string)
{
    stf_name_string_ = stf_name_string;
    ObjectMessageBuilder::BuildStfNameDelta(this);
}
void Object::SetVolume(uint32_t volume)
{
    volume_ = volume;
    ObjectMessageBuilder::BuildVolumeDelta(this);
}
void Object::SetSceneId(uint32_t scene_id)
{
	scene_id_ = scene_id;
}
optional<BaselinesMessage> Object::GetBaseline3()
{
	return move(ObjectMessageBuilder::BuildBaseline3(this));
}

optional<BaselinesMessage> Object::GetBaseline6()
{
	return move(ObjectMessageBuilder::BuildBaseline6(this));
}
