
#include "swganh/object/object.h"

#include "anh/observer/observer_interface.h"
#include "swganh/messages/base_baselines_message.h"
#include "swganh/object/object_controller.h"
#include "swganh/object/object_message_builder.h"

using namespace anh::observer;
using namespace std;
using namespace swganh::object;
using namespace swganh::messages;
using boost::optional;

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
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_3);
        message.data.write<uint16_t>(2); // update type
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

DeltasMessage Object::CreateDeltasMessage(uint16_t view_type) const
{        
    DeltasMessage message;
    message.object_id = GetObjectId();
    message.object_type = GetType();
    message.view_type = view_type;

    return message;
}

const std::shared_ptr<Object>& Object::GetParent() const
{
    return parent_;
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

bool Object::IsDirty() const
{
    return !deltas_.empty();
}

void Object::MakeClean()
{
    baselines_.clear();
    deltas_.clear();
    
    optional<BaselinesMessage> message;
    
    for_each(begin(baselines_builders_), end(baselines_builders_),
        [this, &message] (BaselinesBuilder& builder)
    {       
        if (!(message = builder()))
        {
            return;
        }

        NotifyObservers(*message);
        baselines_.push_back(*message);
    });
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
        
bool Object::HasPrivilegedView_(uint64_t object_id) const
{    
    if (object_id == GetObjectId() || 
        (GetParent() && object_id == GetParent()->GetObjectId())) 
    {
        return true;
    }
    
    return false;
}

void Object::SetPosition(glm::vec3 position)
{
    position_ = position;
}
void Object::SetOrientation(glm::quat orientation)
{
    orientation_ = orientation;
}
void Object::SetContainer(uint64_t container)
{
    container_id_ = container;
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
optional<BaselinesMessage> Object::GetBaseline3()
{
    return optional<BaselinesMessage>(move(ObjectMessageBuilder::BuildBaseline3(this)));
}

optional<BaselinesMessage> Object::GetBaseline6()
{
    return optional<BaselinesMessage>(move(ObjectMessageBuilder::BuildBaseline6(this)));
}
