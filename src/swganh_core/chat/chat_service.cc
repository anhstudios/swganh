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

#include "swganh/database/database_manager.h"
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
#include "swganh_core/messages/chat_request_room_list.h"
#include "swganh_core/messages/chat_query_room.h"
#include "swganh_core/messages/chat_send_to_room.h"
#include "swganh_core/messages/chat_remove_avatar_from_room.h"
#include "swganh_core/messages/chat_create_room.h"
#include "swganh_core/messages/chat_destroy_room.h"
#include "swganh_core/messages/chat_enter_room_by_id.h"
#include "swganh_core/messages/chat_room_list.h"

#include "swganh_core/object/object.h"
#include "swganh_core/object/creature/creature.h"

#include "swganh_core/connection/connection_client_interface.h"
#include "swganh_core/connection/connection_service_interface.h"
#include "swganh_core/command/command_service_interface.h"
#include "swganh_core/simulation/simulation_service_interface.h"

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
    : db_manager_(kernel->GetDatabaseManager())
    , kernel_(kernel)
{
    SetServiceDescription(ServiceDescription(
        "ChatService",
        "chat",
        "0.1",
        "127.0.0.1", 
        0, 
        0, 
        0));
}

ChatService::~ChatService()
{}

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

void ChatService::Initialize()
{
	command_service_ = kernel_->GetServiceManager()->GetService<swganh::command::CommandServiceInterface>("CommandService");
	simulation_service_ = kernel_->GetServiceManager()->GetService<swganh::simulation::SimulationServiceInterface>("SimulationService");
	
	room_provider_ = kernel->GetPluginManager()->CreateObject<ChatRoomProviderInterface>("Chat::RoomProvider");
	user_provider_ = room_provider_->GetUserProvider();
}

void ChatService::Startup()
{
	//Hit the db to load chat rooms/moderators/bans
	auto conn = db_manager_->getConnection("galaxy");
	auto statement = std::shared_ptr<sql::Statement>(conn->createStatement());
	statement->execute("CALL sp_LoadChatData();");

	//Load rooms
	std::unique_ptr<sql::ResultSet> result(statement->getResultSet());
	while(result->next())
	{
		auto pair = rooms_.emplace(result->getUInt(1), ChatRoom());
		if(pair.second)
		{
			auto& room = pair.first->second;
			room.is_private = (result->getUInt(2)) ? true : false;
			room.is_muted = (result->getUInt(3)) ? true : false;
			room.name = result->getString(4);
			room.owner_id = result->getUInt64(5);
			room.owner_name = result->getString(6);
			room.creator_id = result->getUInt64(7);
			room.creator_name = result->getString(8);
			room.title = result->getString(9);
		}
	}
	
	//Load bans
	statement->getMoreResults();
	result.reset(statement->getResultSet());
	while(result->next())
	{
		auto find_itr = rooms_.find(result->getInt(1));
		if(find_itr != rooms_.end())
		{
			find_itr->second.bans_.insert(result->getUInt64(2));
		}
	}
	
	//Load moderators
	uint64_t creature_id;
	statement->getMoreResults();
	result.reset(statement->getResultSet());
	while(result->next())
	{
		auto find_itr = rooms_.find(result->getInt(1));
		if(find_itr != rooms_.end())
		{
			Member m;
			creature_id = result->getUInt64(2);

			m.controller_ = simulation_service_->GetObjectById<Creature>(creature_id_)->GetController();
			m.is_present = m.creature_ != nullptr;
			m.is_moderator = true;
			m.is_invited = false;
			m.custom_name = result->getString(3);
			
			find_itr->second.members_.insert(std::make_pair<uint64_t, Member>(creature_id, std::move(m)));
		}
	}

	auto connection_service = kernel_->GetServiceManager()->GetService<ConnectionServiceInterface>("ConnectionService");
    
	//Tell Related
    connection_service->RegisterMessageHandler(&ChatService::HandleChatInstantMessageToCharacter, this);
    
	//Mail Related
	connection_service->RegisterMessageHandler(&ChatService::HandleChatPersistentMessageToServer, this);
    connection_service->RegisterMessageHandler(&ChatService::HandleChatRequestPersistentMessage, this);
    connection_service->RegisterMessageHandler(&ChatService::HandleChatDeletePersistentMessage, this);

	//Chat Room Related
	connection_service->RegisterMessageHandler(&ChatService::HandleChatRequestRoomList, this);
	connection_service->RegisterMessageHandler(&ChatService::HandleChatQueryRoom, this);
	connection_service->RegisterMessageHandler(&ChatService::HandleChatSendToRoom, this);
	connection_service->RegisterMessageHandler(&ChatService::HandleChatRemoveAvatarFromRoom, this);
	connection_service->RegisterMessageHandler(&ChatService::HandleChatCreateRoom, this);
	connection_service->RegisterMessageHandler(&ChatService::HandleChatDestroyRoom, this);
	connection_service->RegisterMessageHandler(&ChatService::HandleChatEnterRoomById, this);
	
	//Spatial
    command_service_->AddCommandCreator<SpatialChatInternalCommand>("spatialchatinternal");
    
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
            prefix_, 
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

void ChatService::HandleChatRequestRoomList(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatRequestRoomList* message)
{
	ChatRoomList room_list;
	{
		boost::unique_lock<boost::mutex> lock(room_mutex_);
		room_list.data.write<uint32_t>(rooms_.size());
		for(auto& room_pair : rooms_)
		{
			ChatRoom& room = room_pair.second;
			room_list.data.write<uint32_t>(room_pair.first);
			room_list.data.write<uint32_t>((room.is_private) ? 1 : 0);
			room_list.data.write<uint8_t>((room.is_muted) ? 1 : 0);
			room_list.data.write<std::string>("SWG." + prefix_ + "." + room.name);
			
			room_list.data.write<std::string>("SWG");
			room_list.data.write<std::string>(prefix_);
			room_list.data.write<std::string>(room.owner_name);
			
			room_list.data.write<std::string>("SWG");
			room_list.data.write<std::string>(prefix_);
			room_list.data.write<std::string>(room.creator_name);
			
			room_list.data.write<std::wstring>(std::wstring(room.title.begin(), room.title.end()));
		
			//Write out empty lists to save bandwidth.
			room_list.data.write<uint32_t>(0);
			room_list.data.write<uint32_t>(0);
		}
	}
	client->GetController()->Notify(&room_list);
}

void ChatService::HandleChatQueryRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatQueryRoom* message)
{
	LOG(info) << "CHAT QUERY ROOM";
	LOG(info) << "  unknown=" << message->unknown;
	LOG(info) << "  path=" << message->channel_path;
}

void ChatService::HandleChatSendToRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatSendToRoom* message)
{
	LOG(info) << "CHAT MESSAGE";
	LOG(info) << "  speaker=" << message->sender_character_name;
	LOG(info) << "  message=" << std::string(message->message.begin(), message->message.end());
	LOG(info) << "  channel_id=" << message->channel_id;
}
	
void ChatService::HandleChatRemoveAvatarFromRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatRemoveAvatarFromRoom* message)
{
}

void ChatService::HandleChatCreateRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatCreateRoom* message)
{
	LOG(info) << "CHAT CREATE ROOM";
	LOG(info) << "  public=" << message->public_flag;
	LOG(info) << "  moderated=" << message->moderation_flag;
	LOG(info) << "  channel_path" << message->channel_path;
}

void ChatService::HandleChatDestroyRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatDestroyRoom* message)
{
}
	
void ChatService::HandleChatEnterRoomById(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatEnterRoomById* message)
{
}
