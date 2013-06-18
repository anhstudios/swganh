// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "chat_service.h"
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

#include "swganh_core/messages/chat/chat_messages.h"

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
	//Remove spaces from the galaxy name because that causes problems.
	//galaxy_name_.erase(std::remove(galaxy_name_.begin(), galaxy_name_.end(), ' '), galaxy_name_.end());

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
    const std::string& recipient, const std::string& sender, 
    const std::string& sender_game, const std::string& sender_galaxy, 
    const std::wstring& subject, const std::wstring& message, 
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
    const std::string& recipient, const std::string& sender,
    const std::wstring& subject, const std::wstring& message)
{
    return SendPersistentMessage(recipient, sender, "SWG", kernel_->GetServiceDirectory()->galaxy().name(), subject, message, std::vector<char>());
}

void ChatService::SendSpatialChat(
	const std::shared_ptr<swganh::object::Object>& actor, const std::shared_ptr<swganh::object::Object>& target,
    wstring chat_message, uint16_t chat_type, uint16_t mood)
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

	//Load rooms and other information
	user_provider_ = kernel_->GetPluginManager()->CreateObject<ChatUserProviderInterface>("Chat::UserProvider");
	
	try 
	{
		auto conn = db_manager_->getConnection("galaxy");
		{
			auto statement = std::shared_ptr<sql::Statement>(conn->createStatement());
			statement->execute("CALL sp_LoadChatData();");

			//Load rooms
			std::unique_ptr<sql::ResultSet> result(statement->getResultSet());
			while(result->next())
			{
				auto pair = rooms_.emplace(result->getUInt(1), Room());
				if(pair.second)
				{
					auto& room = pair.first->second;
					room.id_ = result->getUInt(1);
					room.is_private_ = (result->getUInt(2)) ? true : false;
					room.is_muted_ = (result->getUInt(3)) ? true : false;
					room.name_ = result->getString(4);
					room.owner_ = result->getUInt64(5);
					room.creator_ = result->getUInt64(7);
					room.title_ = result->getString(9);
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
					find_itr->second.banned_users_.insert(result->getUInt64(2));
				}
			}
	
			//Load moderators
			statement->getMoreResults();
			result.reset(statement->getResultSet());
			while(result->next())
			{
				auto find_itr = rooms_.find(result->getInt(1));
				if(find_itr != rooms_.end())
				{
					find_itr->second.moderators_.insert(result->getUInt64(2));
				}
			}
	
			while(statement->getMoreResults());
		}

		auto channelAdder = [&conn, this] (std::string name) 
		{
			if(GetRoomByPath_(name) == nullptr)
			{

				auto statement = std::unique_ptr<sql::PreparedStatement>(conn->prepareStatement("CALL sp_ChatRoomAdd(?,?,?,?,?);"));
				statement->setInt(1, 0);
				statement->setInt(2, 0);
				statement->setString(3, name);
				statement->setUInt64(4, 0);
				statement->setString(5, "");
	
				//If we have a result it was created!
				std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
				Room room;
				if(result->next())
				{
					room.id_ = result->getInt(1);
					room.is_private_ = false;
					room.is_muted_ = false;
					room.name_ = name;
					room.title_ = "";
					room.owner_ = 0;
					room.creator_ = 0;

					boost::lock_guard<boost::mutex> lock_(room_mutex_);
					auto insert_itr = rooms_.insert(std::make_pair(room.id_, std::move(room)));
				} while(result->next());
				
				while(statement->getMoreResults());
			}
		};

		channelAdder("SWG." + galaxy_name_ + ".Chat");

		for(auto& scene : kernel_->GetAppConfig().scenes)
		{
			boost::algorithm::to_lower(scene);
			channelAdder("SWG." + galaxy_name_ + "." + scene + ".Planet");
			channelAdder("SWG." + galaxy_name_ + "." + scene + ".Chat");
		}
	}
	catch(sql::SQLException &e) 
	{
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
		LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}
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
	connection_service->RegisterMessageHandler(&ChatService::_handleJoinRoom, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleLeaveRoom, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleCreateRoom, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleDestroyRoom, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleBanAvatar, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleUnbanAvatar, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleInviteAvatar, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleUninviteAvatar, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleModeratorAdd, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleModeratorRemove, this);
	connection_service->RegisterMessageHandler(&ChatService::_handleKickAvatar, this);
	
	//Spatial
    command_service_->AddCommandCreator<SpatialChatInternalCommand>("spatialchatinternal");
    
	//When a new player arrives we must load mail/friends for him
	kernel_->GetEventDispatcher()->Subscribe("ObjectReadyEvent",
	[this] (shared_ptr<swganh::EventInterface> incoming_event)
	{
	    auto& creo_obj = static_pointer_cast<ValueEvent<shared_ptr<Creature>>>(incoming_event)->Get();
        LoadMessageHeaders(creo_obj);

		boost::lock_guard<boost::mutex> lock(room_mutex_);
		HandleFriendsList(creo_obj, 1);
	});

	//When a player leaves we need to send messages to all friends
	kernel_->GetEventDispatcher()->Subscribe("Connection::PlayerRemoved",
	[this] (shared_ptr<swganh::EventInterface> incoming_event)
	{
	    auto& play_obj = static_pointer_cast<ValueEvent<shared_ptr<Player>>>(incoming_event)->Get();
		auto creo_obj = std::static_pointer_cast<Creature>(play_obj->GetContainer());
		
		boost::lock_guard<boost::mutex> lock(room_mutex_);
		HandleFriendsList(creo_obj, 0);
	});

	//When a player is added to a friend's list
	kernel_->GetEventDispatcher()->Subscribe("Player::AddFriend",
	[this] (shared_ptr<swganh::EventInterface> incoming_event)
	{
		boost::lock_guard<boost::mutex> lock(room_mutex_);
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
            ChatFriendsListUpdate update(galaxy_name_, real_event.second, 1);
			controller->Notify(&update);
		}
	});

	//When a player is removed from a friend's list
	kernel_->GetEventDispatcher()->Subscribe("Player::RemoveFriend",
	[this] (shared_ptr<swganh::EventInterface> incoming_event)
	{
		boost::lock_guard<boost::mutex> lock(room_mutex_);
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
 
bool ChatService::IsOnline(const std::string& name) const
{
	return IsOnline(user_provider_->GetIdFromFullName(name));
}

bool ChatService::IsOnline(uint64_t id) const
{
	boost::lock_guard<boost::mutex> lock(room_mutex_);
	return online_players_.find(id) != online_players_.end();
}

uint64_t ChatService::GetObjectIdByCustomName(const std::string& custom_name)
{
    return user_provider_->GetIdFromFullName(custom_name);
}

const std::string& ChatService::GetFullNameFromId(uint64_t creature_id) const
{
	return user_provider_->GetFullNameFromId(creature_id);
}
	
std::string ChatService::GetFirstNameFromId(uint64_t creature_id) const
{
	return user_provider_->GetFirstNameFromId(creature_id);
}
	
const std::string& ChatService::GetFullNameFromFirstName(const std::string& name) const
{
	return user_provider_->GetFullNameFromFirstName(name);
}
	
std::string ChatService::GetFirstNameFromFullName(const std::string& name) const
{
	return user_provider_->GetFirstNameFromFullName(name);
}

void ChatService::SendChatPersistentMessageToClient(
    const std::shared_ptr<ObserverInterface>& receiver, 
    const std::string& sender_name,  const std::string& sender_game, const std::string& sender_galaxy, 
    const std::wstring& subject, const std::wstring& message, 
    uint32_t message_id, uint8_t status, const std::vector<char>& attachments, uint32_t timestamp)
{
    SendChatPersistentMessageToClient(receiver, sender_name, sender_game, sender_galaxy, subject, message, message_id, status, attachments, timestamp, true);
}

void ChatService::SendChatPersistentMessageToClient(
    const std::shared_ptr<ObserverInterface>& receiver, 
    const std::string& sender_name, const std::string& sender_game, const std::string& sender_galaxy, 
    const std::wstring& subject, uint32_t message_id, uint8_t status, uint32_t timestamp)
{
    SendChatPersistentMessageToClient(receiver, sender_name, sender_game, sender_galaxy, subject, L"", message_id, status, std::vector<char>(), timestamp, true);
}

void ChatService::SendChatPersistentMessageToClient(
    const std::shared_ptr<ObserverInterface>& receiver, 
    const std::string& sender_name, const std::string& sender_game, const std::string& sender_galaxy, 
    const std::wstring& subject, const std::wstring& message, 
	uint32_t message_id, uint8_t status, const std::vector<char>& attachments, 
    uint32_t timestamp, uint8_t header_only)
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
    uint32_t message_id, uint32_t recieved_status)
{
    ChatOnSendInstantMessage response;
    response.sequence_number = message_id;
    response.success_flag = recieved_status;

    receiver->Notify(&response);
}

void ChatService::SendChatOnSendPersistentMessage(
    const std::shared_ptr<swganh::observer::ObserverInterface>& receiver,
    uint32_t message_id, uint32_t recieved_status)
{    
    ChatOnSendPersistentMessage response;
    response.sequence_number = message_id;
    response.success_flag = recieved_status;

    receiver->Notify(&response);
}

void ChatService::SendChatInstantMessageToClient(
    const std::shared_ptr<swganh::observer::ObserverInterface>& receiver, 
    const std::string& sender_name, const std::string& sender_game, const std::string& sender_galaxy, 
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
    const std::string& sender_name, const std::string& sender_game, const std::string& sender_galaxy, 
    const std::wstring& subject, const std::wstring& message, 
    const std::vector<char>& attachments, uint8_t status, uint32_t timestamp)
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
            ChatFriendsListUpdate friends_list(galaxy_name_, std::string(custom_name.begin(), custom_name.end()), operation);
			id->Notify(&friends_list);
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
                ChatFriendsListUpdate update(galaxy_name_, friend_name, 1);
				controller->Notify(&update);
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

uint32_t ChatService::CreateRoom(const std::string& room_path, const std::string& room_title,
	uint64_t owner_id_, uint64_t creator_id_, bool is_private_, bool is_muted_)
{
	try
	{
		boost::lock_guard<boost::mutex> lock_(room_mutex_);
		Room* room = GetRoomByPath_(room_path);
		if(room == nullptr)
		{
			auto conn = db_manager_->getConnection("galaxy");
			auto statement = std::unique_ptr<sql::PreparedStatement>(conn->prepareStatement("CALL sp_ChatRoomAdd(?,?,?,?,?);"));
			statement->setInt(1, is_private_);
			statement->setInt(2, is_muted_);
			statement->setString(3, room_path);
			statement->setUInt64(4, owner_id_);
			statement->setString(5, room_title);
	
			//If we have a result it was created!
			std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
			Room room;
			if(result->next())
			{
				uint32_t result_id = result->getInt(1);
				
				room.id_ = result_id;
				room.is_private_ = is_private_;
				room.is_muted_ = is_muted_;
				room.name_ = room_path;
				room.title_ = room_title;
				room.owner_ = owner_id_;
				room.creator_ = creator_id_;

				//Cleanup MYSQL due to trashy interface
				while(result->next());
				while(statement->getMoreResults());

				//Perform the inserts
				room.moderators_.insert(room.owner_);
				rooms_.insert(std::make_pair(room.id_, std::move(room)));
				
				return result_id;
			} 
		}
	}
	catch(sql::SQLException &e) 
	{
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
		LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}
	return 0;
}

void ChatService::DestroyRoom(const std::string& room_path)
{
	try
	{
		boost::lock_guard<boost::mutex> lock_(room_mutex_);
		Room* room = GetRoomByPath_(room_path);
		if(room != nullptr)
		{
			auto conn = db_manager_->getConnection("galaxy");
			auto statement = std::unique_ptr<sql::PreparedStatement>(conn->prepareStatement("CALL sp_ChatRoomRemove(?);"));
			statement->setInt(1, room->id_);
			statement->execute();

			ChatOnDestroyRoom reply;
			reply.game_name = "SWG";
			reply.server_name = galaxy_name_;
			reply.owner_name = user_provider_->GetFullNameFromId(room->owner_);
			reply.error = 0;
			reply.channel_id = room->id_;
			reply.request_id = 0;

			for(auto& user : room->users_)
			{
				user->Notify(&reply);
			}
			rooms_.erase(room->id_);
		}
	}
	catch(sql::SQLException &e) 
	{
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
		LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}
}

void ChatService::ForceJoin(const std::shared_ptr<swganh::object::Object>& actor, const std::string& room_path)
{
	auto controller = actor->GetController();

	boost::lock_guard<boost::mutex> lock_(room_mutex_);
	Room* room = GetRoomByPath_(room_path);

	if(controller == nullptr || room == nullptr)
		return;

	//Do the insert
	room->users_.insert(controller);

	//If the channel is hidden we need to tell the actor about the channel
	if(	room->owner_ == 0 && room->is_private_ &&
		room->users_.find(controller) == room->users_.end() &&
		room->invited_users_.find(controller->GetId()) == room->invited_users_.end())
	{
		ChatRoomList room_list;
		room_list.data.write<uint32_t>(1);
		room->SerializeBody(room_list.data, galaxy_name_, user_provider_);
		room_list.data.write<uint32_t>(0);
		room_list.data.write<uint32_t>(0);

		controller->Notify(&room_list);
	}


	//Send notice to all users
	ChatOnEnteredRoom reply;
	reply.game_name = "SWG";
	reply.server_name = galaxy_name_;
	reply.character_name = user_provider_->GetFirstNameFromId(actor->GetObjectId());
	reply.channel_id = room->id_;
	reply.success_bitmask = 0;

	for(auto& user : room->users_)
	{
		user->Notify(&reply);
	}
}
	
void ChatService::ForceLeave(const std::shared_ptr<swganh::object::Object>& actor, const std::string& room_path)
{
	auto controller = actor->GetController();

	boost::lock_guard<boost::mutex> lock_(room_mutex_);
	Room* room = GetRoomByPath_(room_path);

	if(controller == nullptr || room == nullptr)
		return;

	//Do the remove
	auto find_itr = room->users_.find(controller);
	if(find_itr != room->users_.end())
	{
		//Send notice to all users
		ChatOnLeaveRoom reply;
		reply.game_name = "SWG";
		reply.server_name = galaxy_name_;
		reply.character_name = user_provider_->GetFirstNameFromId(actor->GetObjectId());
		reply.channel_id = room->id_;
		reply.error = 0;

		for(auto& user : room->users_)
		{
			user->Notify(&reply);
		}

		room->users_.erase(find_itr);
	}
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
	auto controller = client->GetController();
	if(!controller)
		return;

	auto sender_id = controller->GetId();
	if(controller)
	{
		ChatRoomList room_list;
		{
			boost::unique_lock<boost::mutex> lock(room_mutex_);
			
			uint32_t count = 0, offset = room_list.data.write_position();
			room_list.data.write<uint32_t>(0xCAFEBABE);
			for(auto& room_pair : rooms_)
			{
				Room& room = room_pair.second;

				//If the channel is owned by SYSTEM
				//And we're not in it, or invited, skip it
				if(	room.owner_ == 0 && room.is_private_ &&
					room.users_.find(controller) == room.users_.end() &&
					room.invited_users_.find(sender_id) == room.invited_users_.end())
				{
					continue;
				}

				room.SerializeBody(room_list.data, galaxy_name_, user_provider_);

				//Write out empty lists to save bandwidth (only the result of the Query is displayed.)
				room_list.data.write<uint32_t>(0);
				room_list.data.write<uint32_t>(0);
				++count;
			}
			room_list.data.writeAt<uint32_t>(offset, count);
		}
        
        ChatServerStatus server_status(1);
		controller->Notify(&server_status);
		controller->Notify(&room_list);
	}
}

void ChatService::_handleQueryRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatQueryRoom* message)
{
	auto controller = client->GetController();
	if(!controller)
		return;

	uint64_t sender_id = controller->GetId();

	boost::lock_guard<boost::mutex> lock_(room_mutex_);
	if(Room* room = GetRoomByPath_(message->channel_path))
	{
		//If the channel is owned by SYSTEM
		//And we're not in it, or invited, skip it
		if(	room->owner_ == 0 && room->is_private_ &&
			room->users_.find(controller) == room->users_.end() &&
			room->invited_users_.find(sender_id) == room->invited_users_.end())
		{
			return;
		}

		ChatQueryRoomResponse reply;

		room->SerializeUsers(reply.data, galaxy_name_, user_provider_);
		Room::SerializeIdSet(reply.data, galaxy_name_, user_provider_, room->invited_users_);
		Room::SerializeIdSet(reply.data, galaxy_name_, user_provider_, room->moderators_);
		Room::SerializeIdSet(reply.data, galaxy_name_, user_provider_, room->banned_users_);

		reply.data.write<uint32_t>(message->request_id);
		
		room->SerializeBody(reply.data, galaxy_name_, user_provider_);

		//Write empty lists to save bandwidth
		reply.data.write<uint32_t>(0);
		reply.data.write<uint32_t>(0);

		client->GetController()->Notify(&reply);
	}
}

void ChatService::_handleSendToRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatSendToRoom* message)
{
	auto controller = client->GetController();
	if(!controller)
		return;

	uint64_t speaker = controller->GetId();

	boost::lock_guard<boost::mutex> lock_(room_mutex_);
	if(Room* room = GetRoomById_(message->channel_id))
	{
		uint32_t error_code = 0;
		if(room->users_.find(controller) == room->users_.end())
		{
			error_code = 1; //Not sent
		}
		else if(room->is_muted_ && (room->moderators_.find(speaker) == room->moderators_.end()))
		{
			error_code = 9;
		}
		
		//Send Status Message
        ChatOnSendRoomMessage send_room_msg(error_code, message->message_counter);
		controller->Notify(&send_room_msg);
		
		if(error_code == 0)
		{
			//Send message to all users in channel
			ChatRoomMessage msg;
			msg.server_name = galaxy_name_;
			msg.sender_character_name = user_provider_->GetFirstNameFromId(speaker);
			msg.channel_id = message->channel_id;
			msg.message = message->message;

			for(auto& user : room->users_)
			{
				user->Notify(&msg);
			}
		}
	}
}

void ChatService::_handleJoinRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatEnterRoomById* message)
{
	auto controller = client->GetController();
	if(!controller)
		return;

	uint64_t sender_id = controller->GetId();

	boost::lock_guard<boost::mutex> lock_(room_mutex_);
	if(Room* room = GetRoomById_(message->channel_id))
	{
		ChatOnEnteredRoom reply;
		reply.success_bitmask = 0;

		//If the channel is owned by SYSTEM
		//And we're not in it, or invited, skip it
		if(	room->owner_ == 0 && room->is_private_ &&
			room->users_.find(controller) == room->users_.end() &&
			room->invited_users_.find(sender_id) == room->invited_users_.end())
		{
			reply.success_bitmask = 16;
		}

		//Check if already in room
		if(room->users_.find(client->GetController()) != room->users_.end())
		{
			reply.success_bitmask = 1; //Failed for unknown reason
		}

		bool is_invited = room->invited_users_.find(sender_id) != room->invited_users_.end();
		bool is_admin = (room->moderators_.find(sender_id) != room->moderators_.end()) || (room->owner_ != sender_id);

		if(!is_admin && room->banned_users_.find(sender_id) != room->banned_users_.end())
		{
			reply.success_bitmask = 16; //Cannot join because not invited.
		}
		else if(room->is_private_ && (!(is_invited || is_admin)))
		{
			reply.success_bitmask = 16; //Cannot join because not invited.
		}

		//Send necessary result msg to sender
		reply.game_name = "SWG";
		reply.server_name = galaxy_name_;
		reply.character_name = user_provider_->GetFirstNameFromId(sender_id);
		reply.channel_id = room->id_;

		//If OK
		if(reply.success_bitmask == 0)
		{
			//Add sender to user list
			room->users_.insert(client->GetController());

			for(auto& user : room->users_)
			{
				user->Notify(&reply);
			}
		}
	}
}


void ChatService::_handleLeaveRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatLeaveRoom* message)
{
	auto controller = client->GetController();
	if(!controller)
		return;

	auto sender_id = controller->GetId();

	boost::lock_guard<boost::mutex> lock_(room_mutex_);
	if(Room* room = GetRoomByPath_(message->channel_path))
	{	
		//If the channel is owned by SYSTEM
		//And we're not in it, or invited, skip it
		if(	room->owner_ == 0 && room->is_private_ &&
			room->users_.find(controller) == room->users_.end() &&
			room->invited_users_.find(sender_id) == room->invited_users_.end())
		{
			//We cannot leave a channel controlled by system.
			return;
		}

		auto find_itr = room->users_.find(controller);

		ChatOnLeaveRoom reply;
		reply.error = (find_itr != room->users_.end()) ? 0 : 1;
		reply.game_name = "SWG";
		reply.server_name = galaxy_name_;
		reply.character_name = user_provider_->GetFirstNameFromId(controller->GetId());
		reply.channel_id = room->id_;

		for(auto& user : room->users_)
		{
			user->Notify(&reply);
		}

		if(reply.error == 0)
		{
			room->users_.erase(find_itr);
		}
	}
}

//Chat Room Management
void ChatService::_handleCreateRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatCreateRoom* message)
{
	auto controller = client->GetController();
	if(!controller)
		return;

	auto sender_id = controller->GetId();
	uint32_t id = CreateRoom(message->channel_path, message->channel_title, sender_id, sender_id, 
		(message->public_flag) ? 1 : 0, (message->moderation_flag) ? 1 : 0);
	
	boost::lock_guard<boost::mutex> lock_(room_mutex_);
	if(Room* room = GetRoomById_(id))
	{
		ChatOnCreateRoom reply;
		reply.data.write<uint32_t>(0); //Error code
				
		room->SerializeBody(reply.data, galaxy_name_, user_provider_);
	
		//Empty lists
		reply.data.write<uint32_t>(0);
		reply.data.write<uint32_t>(0);
		reply.data.write<uint32_t>(message->attempts_counter);

		controller->Notify(&reply);
	}
}

void ChatService::_handleDestroyRoom(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatDestroyRoom* message)
{
	auto controller = client->GetController();
	if(!controller)
		return;		

	try
	{
		boost::lock_guard<boost::mutex> lock_(room_mutex_);
		auto find_itr = rooms_.find(message->channel_id);
		if(find_itr != rooms_.end())
		{
			Room* room = &find_itr->second;

			uint64_t sender_id = controller->GetId();

			if(room->owner_ == sender_id)
			{
				//Valid destroy
				auto conn = db_manager_->getConnection("galaxy");
				auto statement = std::unique_ptr<sql::PreparedStatement>(conn->prepareStatement("CALL sp_ChatRoomRemove(?);"));
				statement->setInt(1, room->id_);
				statement->execute();
				
				ChatOnDestroyRoom reply;
				reply.game_name = "SWG";
				reply.server_name = galaxy_name_;
				reply.owner_name = user_provider_->GetFullNameFromId(sender_id);
				reply.error = 0;
				reply.channel_id = message->channel_id;
				reply.request_id = message->attempts_counter;
				
				controller->Notify(&reply);

				for(auto& user : room->users_)
				{
					user->Notify(&reply);
				}

				rooms_.erase(find_itr);
			}
		}
	}
	catch(sql::SQLException &e) 
	{
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
		LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}
}


//class Room User Management
void ChatService::_handleBanAvatar(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatBanAvatar* message)
{

}

void ChatService::_handleUnbanAvatar(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatUnbanAvatar* message)
{
}

void ChatService::_handleInviteAvatar(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatInviteAvatar* message)
{
}

void ChatService::_handleUninviteAvatar(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatUninviteAvatar* message)
{
}

void ChatService::_handleModeratorAdd(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatModeratorAdd* message)
{
}

void ChatService::_handleModeratorRemove(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatModeratorRemove* message)
{
}

void ChatService::_handleKickAvatar(
	const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
	swganh::messages::ChatKickAvatar* message)
{
}

const Room* ChatService::GetRoomByPath(std::string path)
{
	boost::lock_guard<boost::mutex> lock_(room_mutex_);
	return GetRoomByPath_(path);
}

Room* ChatService::GetRoomByPath_(std::string path)
{
	auto find_itr = std::find_if(rooms_.begin(), rooms_.end(), [&] (const std::pair<uint32_t, Room>& room) {
		return room.second.name_ == path;
	});
	return (find_itr != rooms_.end()) ? &(find_itr->second) : nullptr;
}

Room* ChatService::GetRoomById_(uint32_t id)
{
	auto find_itr = rooms_.find(id);
	if(find_itr != rooms_.end())
	{
		return &find_itr->second;
	}
	return nullptr;
}

void Room::SerializeBody(swganh::ByteBuffer& data, 
	const std::string& galaxy_name_, 
	std::shared_ptr<ChatUserProviderInterface>& user_provider_)
{
	data.write<uint32_t>(id_);
	data.write<uint32_t>((is_private_) ? 1 : 0);
	data.write<uint8_t>((is_muted_) ? 1 : 0);
	data.write<std::string>(name_);

	SerializeUser(data, galaxy_name_, user_provider_->GetFirstNameFromId(owner_)); 
	SerializeUser(data, galaxy_name_, user_provider_->GetFirstNameFromId(creator_)); 

	data.write<std::wstring>(std::wstring(title_.begin(), title_.end()));
}

void Room::SerializeUsers(swganh::ByteBuffer& data, 
	const std::string& galaxy_name_, 
	std::shared_ptr<ChatUserProviderInterface>& user_provider_)
{
	data.write<uint32_t>(users_.size());
	for(auto& user : users_)
	{
		SerializeUser(data, galaxy_name_, user_provider_->GetFirstNameFromId(user->GetId())); 
	}
}

void Room::SerializeIdSet(swganh::ByteBuffer& data,
	const std::string& galaxy_name_,
	std::shared_ptr<ChatUserProviderInterface>& user_provider_,
	const std::set<uint64_t>& data_set)
{
	data.write<uint32_t>(data_set.size());
	for(const auto& id : data_set)
	{
		SerializeUser(data, galaxy_name_, user_provider_->GetFirstNameFromId(id));
	}
}