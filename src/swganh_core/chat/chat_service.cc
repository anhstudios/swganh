// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "chat_service.h"
#include "chat_room_provider_interface.h"
#include "chat_user_provider_interface.h"

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

#include "swganh/plugin/plugin_manager.h"
#include "swganh/database/database_manager.h"
#include "swganh/service/service_manager.h"
#include "swganh/service/service_directory_interface.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh_core/messages/controllers/spatial_chat.h"
#include "swganh_core/messages/obj_controller_message.h"
#include "swganh_core/messages/chat/chat_instant_message_to_character.h"
#include "swganh_core/messages/chat/chat_instant_message_to_client.h"
#include "swganh_core/messages/chat/chat_persistent_message_to_client.h"
#include "swganh_core/messages/chat/chat_persistent_message_to_server.h"
#include "swganh_core/messages/chat/chat_on_send_instant_message.h"
#include "swganh_core/messages/chat/chat_on_send_persistent_message.h"
#include "swganh_core/messages/chat/chat_request_persistent_message.h"
#include "swganh_core/messages/chat/chat_delete_persistent_message.h"
#include "swganh_core/messages/chat/chat_friends_list_update.h"
#include "swganh_core/messages/chat/chat_request_room_list.h"
#include "swganh_core/messages/chat/chat_query_room.h"
#include "swganh_core/messages/chat/chat_send_to_room.h"
#include "swganh_core/messages/chat/chat_remove_avatar_from_room.h"
#include "swganh_core/messages/chat/chat_create_room.h"
#include "swganh_core/messages/chat/chat_destroy_room.h"
#include "swganh_core/messages/chat/chat_enter_room_by_id.h"

#include "swganh_core/object/object.h"
#include "swganh_core/object/player/player.h"
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
	, galaxy_name_(kernel->GetAppConfig().galaxy_name)
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
	equipment_service_ = kernel_->GetServiceManager()->GetService<swganh::equipment::EquipmentServiceInterface>("EquipmentService");

	room_provider_ = kernel_->GetPluginManager()->CreateObject<ChatRoomProviderInterface>("Chat::RoomProvider");
	user_provider_ = room_provider_->GetUserProvider();
}

void ChatService::Startup()
{
	auto connection_service = kernel_->GetServiceManager()->GetService<ConnectionServiceInterface>("ConnectionService");

	connection_service->RegisterMessageHandler(&ChatService::_handleInstantMessageToCharacter, this);
	connection_service->RegisterMessageHandler(&ChatService::_handlePersistentMessageToServer, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleRequestPersistentMessage, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleDeletePersistentMessage, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleRequestRoomList, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleQueryRoom, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleSendToRoom, this);
	//connection_service->RegisterMessageHandler(&ChatService::_handleJoinRoom, this);
	//connection_service->RegisterMessageHandler(&ChatService::_handlePartRoom, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleCreateRoom, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleDestroyRoom, this);
	//connection_service->RegisterMessageHandler(&ChatService::_handleBanAvatarFromRoom, this);
	//connection_service->RegisterMessageHandler(&ChatService::_handleUnbanAvatarFromRoom, this);
	//connection_service->RegisterMessageHandler(&ChatService::_handleInviteAvatarToRoom, this);
	//connection_service->RegisterMessageHandler(&ChatService::_handleUninviteAvatarToRoom, this);
	//connection_service->RegisterMessageHandler(&ChatService::_handleAddModToRoom, this);
	//connection_service->RegisterMessageHandler(&ChatService::_handleRemoveModFromRoom, this);
	
	//Spatial
    command_service_->AddCommandCreator<SpatialChatInternalCommand>("spatialchatinternal");
    
	//When a new player arrives we must load mail/friends for him
	kernel_->GetEventDispatcher()->Subscribe("ObjectReadyEvent",
	[this] (shared_ptr<swganh::EventInterface> incoming_event)
	{
	    auto& creo_obj = static_pointer_cast<ValueEvent<shared_ptr<Creature>>>(incoming_event)->Get();
        LoadMessageHeaders(creo_obj);
		HandleFriendsList(creo_obj, 1);
	});

	//When a player leaves we need to send messages to all friends
	kernel_->GetEventDispatcher()->Subscribe("Connection::PlayerRemoved",
	[this] (shared_ptr<swganh::EventInterface> incoming_event)
	{
	    auto& play_obj = static_pointer_cast<ValueEvent<shared_ptr<Player>>>(incoming_event)->Get();
		auto creo_obj = std::static_pointer_cast<Creature>(play_obj->GetContainer());
		HandleFriendsList(creo_obj, 0);
	});

	//When a player is added to a friend's list
	kernel_->GetEventDispatcher()->Subscribe("Player::AddFriend",
	[this] (shared_ptr<swganh::EventInterface> incoming_event)
	{
		auto& real_event = static_pointer_cast<ValueEvent<std::pair<std::shared_ptr<Player>, std::string>>>(incoming_event)->Get();
		auto creo_obj = std::static_pointer_cast<Creature>(real_event.first->GetContainer());

		uint64_t friend_id = user_provider_->GetIdFromFullName(real_event.second);
		auto controller = creo_obj->GetController(); 

		//If we're adding we simply insert
		auto lb = player_observers_.lower_bound(friend_id);
		if(lb != player_observers_.end() && !(player_observers_.key_comp()(friend_id, lb->first)))
		{
			lb->second.insert(controller);
		}
		else
		{
			std::set<std::shared_ptr<swganh::observer::ObserverInterface>> v;
			v.insert(controller);

			player_observers_.insert(lb, std::make_pair(friend_id, v));
		}

		//If they're online send the reverse message
		auto find_itr = online_players_.find(friend_id);
		if(find_itr != online_players_.end())
		{
			//Send friend list update message
			controller->Notify(&ChatFriendsListUpdate(galaxy_name_, real_event.second, 1));
		}

	});

	//When a player is removed from a friend's list
	kernel_->GetEventDispatcher()->Subscribe("Player::RemoveFriend",
	[this] (shared_ptr<swganh::EventInterface> incoming_event)
	{
		auto& real_event = static_pointer_cast<ValueEvent<std::pair<std::shared_ptr<Player>, std::string>>>(incoming_event)->Get();
		auto creo_obj = std::static_pointer_cast<Creature>(real_event.first->GetContainer());

		uint64_t friend_id = user_provider_->GetIdFromFullName(real_event.second);
		auto controller = creo_obj->GetController(); 

		//we need to find the proper entry and remove it
		auto find_itr = player_observers_.find(friend_id);
		if(find_itr != player_observers_.end())
		{
			auto& observer_set = find_itr->second;
			auto observer_find = observer_set.find(controller);
			if(observer_find != observer_set.end())
			{
				observer_set.erase(observer_find);
			}
		}
	});
}
   
uint64_t ChatService::GetObjectIdByCustomName(const std::string& custom_name)
{
    return user_provider_->GetIdFromFullName(custom_name);
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
    
void ChatService::LoadMessageHeaders(const std::shared_ptr<swganh::object::Creature>& receiver)
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

void ChatService::HandleFriendsList(const std::shared_ptr<swganh::object::Creature>& receiver, uint8_t operation)
{
	auto player = std::static_pointer_cast<Player>(equipment_service_->GetEquippedObject(receiver, "ghost"));

	std::shared_ptr<ObserverInterface> controller;
	std::wstring custom_name;
	uint64_t object_id;
	{
		auto lock = receiver->AcquireLock();
		controller = receiver->GetController(lock);
		custom_name = receiver->GetCustomName(lock);
		object_id = receiver->GetObjectId(lock);
	}

	//Insert/Remove into/from the online players list
	if(operation) 
	{
		online_players_.insert(std::make_pair(object_id, controller));
	} 
	else 
	{
		auto find_itr = online_players_.find(object_id);
		if(find_itr != online_players_.end())
			online_players_.erase(find_itr);
	}

	//Send a message to all players in this player's observer list.
	auto find_itr = player_observers_.find(object_id);
	if(find_itr != player_observers_.end())
	{
		for_each(find_itr->second.begin(), find_itr->second.end(), 
		[&] (const std::shared_ptr<swganh::observer::ObserverInterface>& id) {
			//Send friend list update message
			id->Notify(&ChatFriendsListUpdate(galaxy_name_, std::string(custom_name.begin(), custom_name.end()), operation));
		});
	}

	//Add Player to observer lists for all relevant players on his friend's list
	auto friends_list = player->GetFriends();
	for_each(friends_list.begin(), friends_list.end(), [&] (const std::string& friend_name) {
		
		uint64_t friend_id = user_provider_->GetIdFromFullName(friend_name);

		if(operation)
		{
			//If we're adding we simply insert
			auto lb = player_observers_.lower_bound(friend_id);
			if(lb != player_observers_.end() && !(player_observers_.key_comp()(friend_id, lb->first)))
			{
				lb->second.insert(controller);
			}
			else
			{
				std::set<std::shared_ptr<swganh::observer::ObserverInterface>> v;
				v.insert(controller);

				player_observers_.insert(lb, std::make_pair(friend_id, v));
			}

			//If they're online send the reverse message
			auto find_itr = online_players_.find(friend_id);
			if(find_itr != online_players_.end())
			{
				//Send friend list update message
				controller->Notify(&ChatFriendsListUpdate(galaxy_name_, friend_name, 1));
			}
		}
		else
		{
			//Otherwise we need to find the proper entry and remove it
			auto find_itr = player_observers_.find(friend_id);
			if(find_itr != player_observers_.end())
			{
				auto& observer_set = find_itr->second;
				auto observer_find = observer_set.find(controller);
				if(observer_find != observer_set.end())
				{
					observer_set.erase(observer_find);
				}
			}
		}
	});

}

std::wstring ChatService::FilterMessage(const std::wstring& message)
{
    return message;
}

//Tell Related
void ChatService::_handleInstantMessageToCharacter(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatInstantMessageToCharacter* message)
{
	//Get both controllers
	std::shared_ptr<swganh::observer::ObserverInterface> sender_controller = client->GetController();
	std::shared_ptr<swganh::observer::ObserverInterface> recv_controller;

	std::string first_name = user_provider_->GetFirstNameFromFullName(message->recipient_character_name);

	auto recv_itr = online_players_.find(user_provider_->GetIdFromFirstName(first_name));
	if(recv_itr != online_players_.end())
	{
		recv_controller = recv_itr->second;
	}

	//Prep the message
	auto clean_message = FilterMessage(message->message);        
	uint32_t receiver_status = ChatOnSendInstantMessage::FAILED;
    
	if (sender_controller && recv_controller)
	{
		receiver_status = ChatOnSendInstantMessage::OK;

		SendChatInstantMessageToClient(
			recv_controller,           
			first_name,
			"SWG",
			kernel_->GetServiceDirectory()->galaxy().name(), 
			clean_message);
	}

	SendChatOnSendInstantMessage(sender_controller, message->sequence_number, receiver_status);
}
	
//Mail Related
void ChatService::_handlePersistentMessageToServer(
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
			galaxy_name_, 
			message->subject, 
			clean_message, 
			message->attachment_data))
	{
		receiver_status = ChatOnSendInstantMessage::OK;
	}

	SendChatOnSendPersistentMessage(sender->GetController(), message->mail_id, receiver_status);
}

void ChatService::_handleRequestPersistentMessage(
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

void ChatService::_handleDeletePersistentMessage(
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

//Chat Room Related
void ChatService::_handleRequestRoomList(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatRequestRoomList* message)
{
	room_provider_->SendRoomList(client->GetController());
}

void ChatService::_handleQueryRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatQueryRoom* message)
{
	room_provider_->SendQueryResults(client->GetController(), message->channel_path);
}

void ChatService::_handleSendToRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatSendToRoom* message)
{
	room_provider_->SendToChannel(
		message->channel_id, 
		message->message, 
		client->GetController(), 
		message->message_counter);
}

void ChatService::_handleJoinRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatJoinRoom* message)
{
}

void ChatService::_handlePartRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatPartRoom* message)
{
}

//Chat Room Management
void ChatService::_handleCreateRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatCreateRoom* message)
{
}

void ChatService::_handleDestroyRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatDestroyRoom* message)
{
}


//class Room User Management
void ChatService::_handleBanAvatarFromRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatBanAvatarFromRoom* message)
{
}

void ChatService::_handleUnbanAvatarFromRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatUnbanAvatarFromRoom* message)
{
}

void ChatService::_handleInviteAvatarToRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatInviteAvatarToRoom* message)
{
}

void ChatService::_handleUninviteAvatarToRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatUninviteAvatarToRoom* message)
{
}

void ChatService::_handleAddModToRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatAddModToRoom* message)
{
}

void ChatService::_handleRemoveModFromRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatRemoveModFromRoom* message)
{
}