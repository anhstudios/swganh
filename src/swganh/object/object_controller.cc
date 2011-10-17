
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

const shared_ptr<Object>& ObjectController::GetObject() const
{
    return object_;
}

const shared_ptr<RemoteClient>& ObjectController::GetRemoteClient() const
{
    return client_;
}

void ObjectController::Notify(const anh::ByteBuffer& message)
{
    client_->Send(message);
}
