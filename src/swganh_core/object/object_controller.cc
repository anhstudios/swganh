// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "object_controller.h"

#include "swganh_core/messages/obj_controller_message.h"
#include "swganh_core/messages/out_of_band.h"
#include "swganh_core/messages/chat_system_message.h"
#include "swganh_core/messages/controllers/show_fly_text.h"
#include "swganh/connection/connection_client_interface.h"
#include "swganh_core/object/object.h"

#ifndef WIN32
#include <boost/regex.hpp>
    using boost::regex;
    using boost::regex_search;
    using boost::smatch;
#else
    #include <regex>
#endif

using namespace std;
using namespace swganh::connection;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;

ObjectController::ObjectController(
    shared_ptr<Object> object,
    shared_ptr<ConnectionClientInterface> client)
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

shared_ptr<ConnectionClientInterface> ObjectController::GetRemoteClient()
{
    return client_;
}

void ObjectController::SetRemoteClient(shared_ptr<ConnectionClientInterface> remote_client)
{
    client_ = remote_client;
}

void ObjectController::Notify(BaseSwgMessage* message)
{
	swganh::ByteBuffer buffer;
	message->Serialize(buffer);
    client_->SendTo(buffer);
}