
#include "swganh/object/base_object.h"

#include "anh/observer/observer_interface.h"

using namespace anh::observer;
using namespace std;
using namespace swganh::object;
using namespace swganh::scene;
using namespace swganh::scene::messages;
using boost::optional;

uint64_t BaseObject::GetObjectId() const
{
    return object_id_;
}

std::wstring BaseObject::GetCustomName() const
{
    return custom_name_;
}

void BaseObject::SetCustomName(std::wstring custom_name)
{
    custom_name_ = custom_name;
    
    // Only build a message if there are observers.
    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(2); // update type
        message.data.write(custom_name);
    
        AddDeltasUpdate(message);    
    }
}

BaselinesMessage BaseObject::CreateBaselinesMessage(uint16_t view_type, uint16_t opcount)
{
    BaselinesMessage message;
    message.object_id = GetObjectId();
    message.object_type = GetType();
    message.view_type = view_type;
    //message.sub_opcount = opcount;
    
    return message;
}

DeltasMessage BaseObject::CreateDeltasMessage(uint16_t view_type)
{        
    DeltasMessage message;
    message.object_id = GetObjectId();
    message.object_type = GetType();
    message.view_type = view_type;

    return message;
}
const std::shared_ptr<BaseObject>& BaseObject::GetParent() const
{
    return parent_;
}

bool BaseObject::HasObservers() const
{
    return !observers_.empty();
}

void BaseObject::Subscribe(const shared_ptr<ObserverInterface>& observer)
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

void BaseObject::Unsubscribe(const shared_ptr<ObserverInterface>& observer)
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
    
void BaseObject::NotifyObservers(const anh::ByteBuffer& message)
{
    for_each(
        observers_.begin(),
        observers_.end(),
        [&message] (const std::shared_ptr<ObserverInterface>& observer)
    {
        observer->Notify(message);
    });
}

bool BaseObject::IsDirty() const
{
    return !deltas_.empty();
}

void BaseObject::MakeClean()
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

const BaselinesCacheContainer& BaseObject::GetBaselines(uint64_t viewer_id) const
{
    return baselines_;
}

const DeltasCacheContainer& BaseObject::GetDeltas(uint64_t viewer_id) const
{
    return deltas_;
}

void BaseObject::AddDeltasUpdate(DeltasMessage message)
{
    NotifyObservers(message);
    deltas_.push_back(move(message));
}

void BaseObject::AddBaselinesBuilders_()
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
        
bool BaseObject::HasPrivilegedView_(uint64_t object_id) const
{    
    if (object_id == GetObjectId() || 
        (GetParent() && object_id == GetParent()->GetObjectId())) 
    {
        return true;
    }
    
    return false;
}

boost::optional<swganh::scene::messages::BaselinesMessage> BaseObject::GetBaseline3()
{
    auto message = CreateBaselinesMessage(VIEW_3);
    message.data.write(GetComplexity());
    message.data.write(GetStfNameFile());
    // spacer
    message.data.write(0);
    message.data.write(GetStfNameString());
    message.data.write(GetCustomName());
    message.data.write(GetVolume());

    return boost::optional<BaselinesMessage>(std::move(message));
}

boost::optional<swganh::scene::messages::BaselinesMessage> BaseObject::GetBaseline6()
{
    auto message = CreateBaselinesMessage(VIEW_6);
    //message.data.write(GetScene()->GetId());

    return boost::optional<BaselinesMessage>(std::move(message));
}
