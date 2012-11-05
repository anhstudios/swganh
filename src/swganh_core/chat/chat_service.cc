// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "chat_service.h"

#ifdef WIN32
#include <regex>
#else
#include <boost/regex.hpp>
#endif

#include "swganh/logger.h"

#include "swganh/service/service_manager.h"
#include "swganh/service/service_directory_interface.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh_core/messages/controllers/spatial_chat.h"
#include "swganh_core/messages/obj_controller_message.h"
#include "swganh_core/messages/chat_instant_message_to_character.h"
#include "swganh_core/messages/chat_instant_message_to_client.h"
#include "swganh_core/messages/chat_persistent_message_to_client.h"
#include "swganh_core/messages/chat_persistent_message_to_server.h"
#include "swganh_core/messages/chat_on_send_instant_message.h"

#include "swganh_core/object/object.h"

#include "swganh/connection/connection_client_interface.h"
#include "swganh/connection/connection_service_interface.h"
#include "swganh/command/command_service_interface.h"
#include "swganh/simulation/simulation_service_interface.h"

#include "spatial_chat_internal_command.h"

using namespace swganh::app;
using namespace swganh::service;
using namespace std;
using namespace swganh::chat;
using namespace swganh::command;
using namespace swganh::connection;
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
	const std::shared_ptr<swganh::object::Object>& actor,
	const std::shared_ptr<swganh::object::Object>& target,
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
    
    actor->NotifyObservers(&spatial_chat);
}

void ChatService::Startup()
{
	simulation_service_ = kernel_->GetServiceManager()->GetService<swganh::simulation::SimulationServiceInterface>("SimulationService");

	auto connection_service = kernel_->GetServiceManager()->GetService<ConnectionServiceInterface>("ConnectionService");
    
    connection_service->RegisterMessageHandler(&ChatService::HandleChatInstantMessageToCharacter, this);

	command_service_ = kernel_->GetServiceManager()->GetService<swganh::command::CommandServiceInterface>("CommandService");

    command_service_->AddCommandCreator("spatialchatinternal",
        [] (
        swganh::app::SwganhKernel* kernel,
        const CommandProperties& properties)
    {
        return std::make_shared<SpatialChatInternalCommand>(kernel, properties);
    });
}


void ChatService::HandleChatInstantMessageToCharacter(
    const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
    swganh::messages::ChatInstantMessageToCharacter* message)
{
	auto sender = simulation_service_->GetObjectById(client->GetController()->GetId());

    if (!sender)
    {
        return;
    }

    // @TODO Filter input (possibly via plugin class)

    auto receiver = simulation_service_->GetObjectByCustomName(message->recipient_character_name);
    
    uint32_t receiver_status = ChatOnSendInstantMessage::FAILED;
    
    if (receiver)
    {
        receiver_status = ChatOnSendInstantMessage::OK;
        
        ChatInstantMessageToClient instant_message(
            "SWG",
            kernel_->GetServiceDirectory()->galaxy().name(),            
            sender->GetFirstName(),
            message->message);

        receiver->GetController()->Notify(&instant_message);
    }

    ChatOnSendInstantMessage response;
    response.sequence_number = message->sequence_number;
    response.success_flag = receiver_status;

    sender->GetController()->Notify(&response);
}
