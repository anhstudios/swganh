// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/chat/chat_service.h"

#ifdef WIN32
#include <regex>
#else
#include <boost/regex.hpp>
#endif

#include "anh/logger.h"

#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/messages/controllers/spatial_chat.h"
#include "swganh/messages/obj_controller_message.h"

#include "swganh/object/object.h"
#include "swganh/object/object_controller.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/player/player.h"
#include "swganh/object/tangible/tangible.h"

#include "swganh/command/command_service.h"
#include "swganh/simulation/simulation_service.h"

#include "spatial_chat_internal_command.h"

using namespace anh::app;
using namespace anh::service;
using namespace std;
using namespace swganh::chat;
using namespace swganh::command;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;
using namespace swganh::simulation;

using swganh::app::SwganhKernel;

#ifdef WIN32
using std::wregex;
using std::wsmatch;
using std::regex_match;
#else
using boost::wregex;
using boost::wsmatch;
using boost::regex_match;
#endif

ChatService::ChatService(SwganhKernel* kernel)
    : kernel_(kernel)
{}

ServiceDescription ChatService::GetServiceDescription()
{
    ServiceDescription service_description(
        "ChatService",
        "chat",
        "0.1",
        "127.0.0.1", 
        0, 
        0, 
        0);

    return service_description;
}

void ChatService::SendSpatialChat(
	const std::shared_ptr<swganh::object::creature::Creature>& actor, // creature object
	const std::shared_ptr<swganh::object::tangible::Tangible>& target,	// target object
    wstring chat_message,
    uint16_t chat_type,
    uint16_t mood)
{    
    SpatialChat spatial_chat;
    spatial_chat.speaker_id = actor->GetObjectId();
    
    if (target)
    {
        spatial_chat.target_id = target->GetObjectId();
    }

    spatial_chat.message = chat_message;
    spatial_chat.chat_type = chat_type;
    spatial_chat.mood = mood;

    spatial_chat.language = static_cast<uint8_t>(0);
    
    actor->NotifyObservers(spatial_chat);
}

void ChatService::Start()
{
	auto command_service = kernel_->GetServiceManager()->GetService<swganh::command::CommandService>("CommandService");
    
    command_service->AddCommandCreator("spatialchatinternal",
        [] (
        swganh::app::SwganhKernel* kernel,
        const CommandProperties& properties,
        const std::shared_ptr<swganh::object::ObjectController>& controller,
        const swganh::messages::controllers::CommandQueueEnqueue& command_request)
    {
        return std::unique_ptr<SpatialChatInternalCommand>(new SpatialChatInternalCommand(kernel, properties, controller, command_request));
    });
}
