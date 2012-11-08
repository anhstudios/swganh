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
using swganh::observer::ObserverInterface;

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

bool ChatService::SendPersistentMessage(
    const std::string& recipient,
    const std::string& sender, 
    const std::string& sender_game, 
    const std::string& sender_galaxy, 
    const std::wstring& subject, 
    const std::wstring& message, 
    const std::vector<char>& attachments)
{
    uint64_t recipient_id = GetObjectIdByCustomName(recipient);
    
    if (!recipient_id)
    {
        DLOG(warning) << "Unable to find recipient for persistent message: " << recipient;
        return false;
    }

    uint8_t status = 'N'; // N for new
    uint32_t timestamp = static_cast<uint32_t>(time(NULL));
    uint32_t message_id = StorePersistentMessage(
        recipient_id, sender, sender_game, sender_galaxy, subject, message, attachments, status, timestamp);

    if (auto online_object = simulation_service_->GetObjectById(recipient_id))
    {
        SendChatPersistentMessageToClient(online_object->GetController(), sender, sender_game, sender_galaxy, subject, message_id, status, timestamp);
    }

    return true;
}
    
bool ChatService::SendPersistentMessage(
    const std::string& recipient,
    const std::string& sender,
    const std::wstring& subject, 
    const std::wstring& message)
{
    return SendPersistentMessage(recipient, sender, "SWG", kernel_->GetServiceDirectory()->galaxy().name(), subject, message, std::vector<char>());
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
   
uint64_t ChatService::GetObjectIdByCustomName(const std::string& custom_name)
{
    uint64_t object_id = 0;

    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = std::unique_ptr<sql::PreparedStatement>(
            conn->prepareStatement("SELECT sf_GetObjectIdByCustomName(?);"));

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


void ChatService::SendChatPersistentMessageToClient(
    const std::shared_ptr<ObserverInterface>& receiver, 
    const std::string& sender_name, 
    const std::string& sender_game, 
    const std::string& sender_galaxy, 
    const std::wstring& subject, 
    const std::wstring& message, 
    uint32_t message_id,
    uint8_t status,
    const std::vector<char>& attachments, 
    uint32_t timestamp)
{
    SendChatPersistentMessageToClient(receiver, sender_name, sender_game, sender_galaxy, subject, message, message_id, status, attachments, timestamp, true);
}

void ChatService::SendChatPersistentMessageToClient(
    const std::shared_ptr<ObserverInterface>& receiver, 
    const std::string& sender_name, 
    const std::string& sender_game, 
    const std::string& sender_galaxy, 
    const std::wstring& subject,
    uint32_t message_id,
    uint8_t status,
    uint32_t timestamp)
{
    SendChatPersistentMessageToClient(receiver, sender_name, sender_game, sender_galaxy, subject, L"", message_id, status, std::vector<char>(), timestamp, true);
}

void ChatService::SendChatPersistentMessageToClient(
    const std::shared_ptr<ObserverInterface>& receiver, 
    const std::string& sender_name, 
    const std::string& sender_game, 
    const std::string& sender_galaxy, 
    const std::wstring& subject, 
    const std::wstring& message, 
    uint32_t message_id,
    uint8_t status,
    const std::vector<char>& attachments, 
    uint32_t timestamp,
    uint8_t header_only)
{
    ChatPersistentMessageToClient persistent_message;
    persistent_message.game_name = sender_game;
    persistent_message.server_name = sender_galaxy;
    persistent_message.mail_message_subject = subject;
    persistent_message.mail_message_body = message;
    persistent_message.attachment_data = attachments;
    persistent_message.mail_message_id = message_id;
    persistent_message.request_type_flag = header_only;
    persistent_message.sender_character_name = sender_name;
    persistent_message.status = status;
    persistent_message.timestamp = timestamp;
                
    receiver->Notify(&persistent_message);
}

void ChatService::SendChatOnSendInstantMessage(
    const std::shared_ptr<swganh::observer::ObserverInterface>& receiver,
    uint32_t message_id,
    uint32_t recieved_status)
{
    ChatOnSendInstantMessage response;
    response.sequence_number = message_id;
    response.success_flag = recieved_status;

    receiver->Notify(&response);
}

void ChatService::SendChatOnSendPersistentMessage(
    const std::shared_ptr<swganh::observer::ObserverInterface>& receiver,
    uint32_t message_id,
    uint32_t recieved_status)
{    
    ChatOnSendPersistentMessage response;
    response.sequence_number = message_id;
    response.success_flag = recieved_status;

    receiver->Notify(&response);
}

void ChatService::SendChatInstantMessageToClient(
    const std::shared_ptr<swganh::observer::ObserverInterface>& receiver, 
    const std::string& sender_name, 
    const std::string& sender_game, 
    const std::string& sender_galaxy, 
    const std::wstring& message)
{
    ChatInstantMessageToClient instant_message;
    instant_message.sender_character_name = sender_name;
    instant_message.game_name = sender_game;
    instant_message.server_name = sender_galaxy;
    instant_message.message = message;

    receiver->Notify(&instant_message);
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

    auto clean_message = FilterMessage(message->message);        
    uint32_t receiver_status = ChatOnSendInstantMessage::FAILED;
    
    if (auto receiver = simulation_service_->GetObjectByCustomName(message->recipient_character_name))
    {
        receiver_status = ChatOnSendInstantMessage::OK;
        auto firstname = sender->GetFirstName();

        SendChatInstantMessageToClient(
            receiver->GetController(),           
            std::string(std::begin(firstname), std::end(firstname)),
            "SWG",
            kernel_->GetServiceDirectory()->galaxy().name(), 
            clean_message);
    }

    SendChatOnSendInstantMessage(sender->GetController(), message->sequence_number, receiver_status);
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

    uint32_t receiver_status = ChatOnSendInstantMessage::FAILED;
    auto clean_message = FilterMessage(message->message);    
    auto firstname = sender->GetFirstName();

    if (SendPersistentMessage(
            message->recipient, 
            std::string(firstname.begin(), firstname.end()), 
            "SWG", 
            kernel_->GetServiceDirectory()->galaxy().name(), 
            message->subject, 
            clean_message, 
            message->attachment_data))
    {
        receiver_status = ChatOnSendInstantMessage::OK;
    }

    SendChatOnSendPersistentMessage(sender->GetController(), message->mail_id, receiver_status);
}

void ChatService::HandleChatRequestPersistentMessage(
    const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
    swganh::messages::ChatRequestPersistentMessage* message)
{    
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = std::unique_ptr<sql::PreparedStatement>(
            conn->prepareStatement("CALL sp_MailGetMessage(?, ?);"));

        statement->setUInt(1, message->mail_message_id);
        statement->setUInt64(2, client->GetController()->GetId());

        auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());

        while (result_set->next()) {
            
            std::string tmp = result_set->getString("subject");
            std::wstring subject(std::begin(tmp), std::end(tmp));

            tmp = result_set->getString("message");
            std::wstring message(std::begin(tmp), std::end(tmp));

            tmp = result_set->getString("attachments");
            std::vector<char> attachments(std::begin(tmp), std::end(tmp));

            SendChatPersistentMessageToClient(client->GetController(), 
                result_set->getString("sender"),
                result_set->getString("sender_game"),
                result_set->getString("sender_galaxy"),
                subject,
                message,
                result_set->getUInt("id"),
                result_set->getUInt("status"),
                attachments,
                result_set->getUInt("sent_time"),
                false);
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
        auto statement = std::unique_ptr<sql::PreparedStatement>(
            conn->prepareStatement("CALL sp_MailDeleteMessage(?, ?);"));

        statement->setUInt(1, message->mail_message_id);
        statement->setUInt64(2, client->GetController()->GetId());

        statement->execute();
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

uint32_t ChatService::StorePersistentMessage(
    uint64_t recipient_id,
    const std::string& sender_name, 
    const std::string& sender_game, 
    const std::string& sender_galaxy, 
    const std::wstring& subject, 
    const std::wstring& message, 
    const std::vector<char>& attachments,
    uint8_t status,
    uint32_t timestamp)
{   
    uint32_t message_id = 0;

    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = std::unique_ptr<sql::PreparedStatement>(
            conn->prepareStatement("SELECT sf_MailCreate(?, ?, ?, ?, ?, ?, ?, ?, ?);"));

        statement->setString(1, sender_name);
        statement->setString(2, sender_game);
        statement->setString(3, sender_galaxy);
        statement->setUInt64(4, recipient_id);
        statement->setString(5, std::string(std::begin(subject), std::end(subject)));
        statement->setString(6, std::string(std::begin(message), std::end(message)));
        statement->setString(7, std::string(std::begin(attachments), std::end(attachments)));
        statement->setUInt(8, status);
        statement->setUInt(9, timestamp);

        auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());

        while(result_set->next()) 
        {
            message_id = result_set->getUInt(1);
        }

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return message_id;
}
    
void ChatService::LoadMessageHeaders(const std::shared_ptr<swganh::object::Object>& receiver)
{
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = std::unique_ptr<sql::PreparedStatement>(
            conn->prepareStatement("CALL sp_MailFetchHeaders(?);"));

        statement->setUInt64(1, receiver->GetObjectId());

        auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());

        while(result_set->next())
        {           
            std::string tmp = result_set->getString("subject");
            std::wstring subject(std::begin(tmp), std::end(tmp));
            
            SendChatPersistentMessageToClient(receiver->GetController(), 
                result_set->getString("sender"),
                result_set->getString("sender_game"),
                result_set->getString("sender_galaxy"),
                subject,
                result_set->getUInt("id"),
                result_set->getUInt("status"),
                result_set->getUInt("sent_time"));
        }

		while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

std::wstring ChatService::FilterMessage(const std::wstring& message)
{
    return message;
}
