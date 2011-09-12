
#include "swganh/object/base_object.h"

#include "swganh/scene/scene.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::scene;
using namespace swganh::scene::messages;

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
    if (GetScene()->HasObservers(object_id_))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(2); // update type
        message.data.write(custom_name);
    
        GetScene()->UpdateObservers(object_id_, message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));            
    }
}

shared_ptr<Scene> BaseObject::GetScene()
{
    return scene_;        
}

/**
 * Returns the baselines created in the last reliable update. If
 * no baselines exist yet for the object a reliable update will be
 * triggered and the results of that returned.
 */
BaselinesCacheContainer BaseObject::GetBaselines()
{
    if (baselines_cache_.empty())
    {
        ReliableUpdate();
    }
    
    return baselines_cache_;
}
    
/**
 * @return The deltas created since the last reliable update.
 */
DeltasCacheContainer BaseObject::GetDeltas()
{
    if (!deltas_cache_.empty())
    {
        return deltas_cache_;
    }
    
    return DeltasCacheContainer();
}

BaselinesMessage BaseObject::CreateBaselinesMessage(uint16_t view_type)
{
    BaselinesMessage message;
    message.object_id = GetObjectId();
    message.object_type = GetType();
    message.view_type = view_type;

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

void BaseObject::ReliableUpdate()
{
    baselines_cache_.clear();
    deltas_cache_.clear();
    
    OnReliableUpdate();
}
