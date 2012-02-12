
#include "swganh/object/object_controller.h"

#include "swganh/messages/obj_controller_message.h"
#include "swganh/network/remote_client.h"
#include "swganh/object/object.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::network;
using namespace swganh::object;

ObjectController::ObjectController(
    shared_ptr<Object> object,
    shared_ptr<RemoteClient> client)
    : object_(object)
    , client_(client)
{
}

ObjectController::~ObjectController()
{
    object_->ClearController();
}

uint64_t ObjectController::GetId() const
{
    if (!object_)
    {
        return 0;
    }

    return object_->GetObjectId();
}

shared_ptr<Object> ObjectController::GetObject() const
{
    return object_;
}

shared_ptr<RemoteClient> ObjectController::GetRemoteClient()
{
    return client_;
}

void ObjectController::SetRemoteClient(shared_ptr<RemoteClient> remote_client)
{
    client_ = remote_client;
}

void ObjectController::Notify(const anh::ByteBuffer& message)
{
    client_->Send(message);
}

void ObjectController::Notify(const ObjControllerMessage& message)
{
    ObjControllerMessage tmp = message;
    
    tmp.object_id = object_->GetObjectId();
    
    ObserverInterface::Notify(tmp);
}
