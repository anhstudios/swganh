// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "chat_service.h"

#include <ctime>

#ifdef WIN32
#include <regex>
#else
#include <boost/regex.hpp>
#endif

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include "swganh/logger.h"

#include "swganh/database/database_manager_interface.h"
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
#include "swganh_core/messages/chat_on_send_persistent_message.h"
#include "swganh_core/messages/chat_request_persistent_message.h"
#include "swganh_core/messages/chat_delete_persistent_message.h"
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
    , db_manager_(kernel->GetDatabaseManager())
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
    connection_service->RegisterMessageHandler(&ChatService::HandleChatPersistentMessageToServer, this);
    connection_service->RegisterMessageHandler(&ChatService::HandleChatRequestPersistentMessage, this);
    connection_service->RegisterMessageHandler(&ChatService::HandleChatDeletePersistentMessage, this);

	command_service_ = kernel_->GetServiceManager()->GetService<swganh::command::CommandServiceInterface>("CommandService");

    command_service_->AddCommandCreator("spatialchatinternal",
        [] (
        swganh::app::SwganhKernel* kernel,
        const CommandProperties& properties)
    {
        return std::make_shared<SpatialChatInternalCommand>(kernel, properties);
    });

    
	kernel_->GetEventDispatcher()->Subscribe(
		"ObjectReadyEvent",
	[this] (shared_ptr<swganh::EventInterface> incoming_event)
	{
	    const auto& player_obj = static_pointer_cast<ValueEvent<shared_ptr<Object>>>(incoming_event)->Get();
        LoadMessageHeaders(player_obj);
	});
}
   
uint64_t ChatService::GetReceiverIdByCustomName(std::string custom_name)
{
    uint64_t object_id = 0;

    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement("SELECT sf_GetObjectIdByCustomName(?);"));
        statement->setString(1, custom_name);

        auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery());

        while (result_set->next()) {
            object_id = result_set->getUInt64(1);
        }

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return object_id;
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

void ChatService::HandleChatPersistentMessageToServer(
    const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
    swganh::messages::ChatPersistentMessageToServer* message)
{
	auto sender = simulation_service_->GetObjectById(client->GetController()->GetId());

    if (!sender)
    {
        return;
    }

    // @TODO Filter input (possibly via plugin class)
    auto receiver_id = GetReceiverIdByCustomName(message->recipient);
    auto receiver = simulation_service_->GetObjectById(receiver_id);
    
    uint32_t receiver_status = ChatOnSendInstantMessage::FAILED;
    
    if (receiver_id)
    {
        receiver_status = ChatOnSendInstantMessage::OK;
        
        auto firstname = sender->GetFirstName();

        auto timestamp = static_cast<uint32_t>(time(NULL));

        auto message_id = PersistMessage(receiver_id, std::string(firstname.begin(), firstname.end()), "SWG",
            kernel_->GetServiceDirectory()->galaxy().name(), message->subject, message->message, message->attachment_data, timestamp);

        if (receiver)
        {
            
            ChatPersistentMessageToClient persistent_message;
            persistent_message.game_name = "SWG";
            persistent_message.server_name = kernel_->GetServiceDirectory()->galaxy().name();
            persistent_message.mail_message_subject = message->subject;
            persistent_message.mail_message_id = message_id;
            persistent_message.request_type_flag = 1;
            persistent_message.sender_character_name = std::string(firstname.begin(), firstname.end());
            persistent_message.status = 'N';
            persistent_message.timestamp = timestamp;
                        
            receiver->GetController()->Notify(&persistent_message);
        }
    }

    ChatOnSendPersistentMessage response;
    response.sequence_number = message->mail_id;
    response.success_flag = receiver_status;

    sender->GetController()->Notify(&response);
}


void ChatService::HandleChatRequestPersistentMessage(
    const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
    swganh::messages::ChatRequestPersistentMessage* message)
{    
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement("CALL sp_MailGetMessage(?, ?);"));
        statement->setUInt(1, message->mail_message_id);
        statement->setUInt64(2, client->GetController()->GetId());

        auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery());

        while (result_set->next()) {
            
            uint32_t message_id = result_set->getUInt("id");
            uint32_t timestamp = result_set->getUInt("sent_time");
            uint8_t status = result_set->getUInt("status");
            std::string sender_game = result_set->getString("sender_game");
            std::string sender_galaxy = result_set->getString("sender_galaxy");
            std::string sender_name = result_set->getString("sender");
            std::string tmp = result_set->getString("subject");
            std::wstring subject(std::begin(tmp), std::end(tmp));

            tmp = result_set->getString("message");
            std::wstring message(std::begin(tmp), std::end(tmp));

            ChatPersistentMessageToClient persistent_message;
            persistent_message.game_name = sender_game;
            persistent_message.server_name = sender_galaxy;
            persistent_message.mail_message_subject = subject;
            persistent_message.mail_message_body = message;
            persistent_message.mail_message_id = message_id;
            persistent_message.request_type_flag = 0;
            persistent_message.sender_character_name = sender_name;
            persistent_message.status = status;
            persistent_message.timestamp = timestamp;
                        
            client->GetController()->Notify(&persistent_message);
        }
        
		while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void ChatService::HandleChatDeletePersistentMessage(
    const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
    swganh::messages::ChatDeletePersistentMessage* message)
{
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement("CALL sp_MailDeleteMessage(?, ?);"));
        statement->setUInt(1, message->mail_message_id);
        statement->setUInt64(2, client->GetController()->GetId());

        statement->execute();

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

uint32_t ChatService::PersistMessage(uint64_t receiver_id, std::string sender_name, std::string sender_game, std::string sender_galaxy, 
    std::wstring subject, std::wstring message, std::vector<char> attachments, uint32_t timestamp)
{    
    uint32_t message_id = 0;

    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement("SELECT sf_MailCreate(?, ?, ?, ?, ?, ?, ?, ?);"));
        statement->setString(1, sender_name);
        statement->setString(2, sender_game);
        statement->setString(3, sender_galaxy);
        statement->setUInt64(4, receiver_id);
        statement->setString(5, std::string(std::begin(subject), std::end(subject)));
        statement->setString(6, std::string(std::begin(message), std::end(message)));
        statement->setString(7, std::string(std::begin(attachments), std::end(attachments)));
        statement->setUInt(8, timestamp);

        auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery());

        while (result_set->next()) {
            message_id = result_set->getUInt(1);
        }

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return message_id;
}
    
void ChatService::LoadMessageHeaders(std::shared_ptr<swganh::object::Object> receiver)
{
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement("CALL sp_MailFetchHeaders(?);"));
        statement->setUInt64(1, receiver->GetObjectId());

        auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery());

        while (result_set->next()) {

            uint32_t message_id = result_set->getUInt("id");
            uint32_t timestamp = result_set->getUInt("sent_time");
            uint8_t status = result_set->getUInt("status");
            std::string sender_game = result_set->getString("sender_game");
            std::string sender_galaxy = result_set->getString("sender_galaxy");
            std::string sender_name = result_set->getString("sender");
            std::string tmp = result_set->getString("subject");
            std::wstring subject(std::begin(tmp), std::end(tmp));

            ChatPersistentMessageToClient persistent_message;
            persistent_message.game_name = sender_game;
            persistent_message.server_name = sender_galaxy;
            persistent_message.mail_message_subject = subject;
            persistent_message.mail_message_id = message_id;
            persistent_message.request_type_flag = 1;
            persistent_message.sender_character_name = sender_name;
            persistent_message.status = status;
            persistent_message.timestamp = timestamp;
                        
            receiver->GetController()->Notify(&persistent_message);
        }
		while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}


