// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <boost/thread/mutex.hpp>

#include "swganh_core/chat/chat_service_interface.h"
#include "swganh_core/command/command_service_interface.h"
#include "swganh_core/simulation/simulation_service_interface.h"
#include "swganh_core/equipment/equipment_service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/messages/controllers/command_queue_enqueue.h"

#include "room.h"

namespace swganh {
namespace connection {
    class ConnectionClientInterface;
}

namespace object {
	class Object;
}

namespace messages
{
	//Tell Related
	struct ChatInstantMessageToCharacter;
	
	//Mail Related
	struct ChatPersistentMessageToServer;
	struct ChatRequestPersistentMessage;
	struct ChatDeletePersistentMessage;

	//Chat Room Related
	struct ChatRequestRoomList;
	struct ChatQueryRoom;
	struct ChatSendToRoom;
	struct ChatEnterRoomById;
	struct ChatLeaveRoom;

	//Chat Room Management
	struct ChatCreateRoom;
	struct ChatDestroyRoom;

	//class Room User Management
	struct ChatBanAvatar;
	struct ChatUnbanAvatar;
	struct ChatInviteAvatar;
	struct ChatUninviteAvatar;
	struct ChatModeratorAdd;
	struct ChatModeratorRemove;
	struct ChatKickAvatar;

	//Chat Room Ownership Management
}

namespace chat {

class ChatUserProviderInterface;

/*
* The chat service handles chat related functions
*/
class ChatService: public swganh::chat::ChatServiceInterface
{
public:
	/**
	* Creates a new instance
	*/
    explicit ChatService(swganh::app::SwganhKernel* kernel);
    virtual ~ChatService();

    bool SendPersistentMessage(
        const std::string& recipient,
        const std::string& sender, 
        const std::string& sender_game, 
        const std::string& sender_galaxy, 
        const std::wstring& subject, 
        const std::wstring& message, 
        const std::vector<char>& attachments);
    
    bool SendPersistentMessage(
        const std::string& recipient,
        const std::string& sender,
        const std::wstring& subject, 
        const std::wstring& message);
		
    void SendSpatialChat(
		const std::shared_ptr<swganh::object::Object>& actor,
		const std::shared_ptr<swganh::object::Object>& target,
        std::wstring chat_message,
        uint16_t chat_type,
        uint16_t mood);

	uint32_t CreateRoom(const std::string& room_path, const std::string& room_title,
		uint64_t owner_id_, uint64_t creator_id_,
		bool is_private_, bool is_muted_);

	void DestroyRoom(const std::string& room_path);

	void ForceJoin(const std::shared_ptr<swganh::object::Object>& actor, const std::string& room_path);
	
	void ForceLeave(const std::shared_ptr<swganh::object::Object>& actor, const std::string& room_path);

	uint64_t GetObjectIdByCustomName(const std::string& custom_name);

	const std::string& GetFullNameFromId(uint64_t creature_id) const;
	
	std::string GetFirstNameFromId(uint64_t creature_id) const;
	
	const std::string& GetFullNameFromFirstName(const std::string& name) const;
	
	std::string GetFirstNameFromFullName(const std::string& name) const;

	bool IsOnline(const std::string& name) const;
	bool IsOnline(uint64_t id) const;

	void Initialize();
    void Startup();

    

private:

    void SendChatPersistentMessageToClient(
        const std::shared_ptr<swganh::observer::ObserverInterface>& receiver, 
        const std::string& sender_name, 
        const std::string& sender_game, 
        const std::string& sender_galaxy, 
        const std::wstring& subject, 
        const std::wstring& message, 
        uint32_t message_id,
        uint8_t status,
        const std::vector<char>& attachments, 
        uint32_t timestamp);

    void SendChatPersistentMessageToClient(
        const std::shared_ptr<swganh::observer::ObserverInterface>& receiver, 
        const std::string& sender_name, 
        const std::string& sender_game, 
        const std::string& sender_galaxy, 
        const std::wstring& subject,
        uint32_t message_id,
        uint8_t status,
        uint32_t timestamp);
    
    void SendChatPersistentMessageToClient(
        const std::shared_ptr<swganh::observer::ObserverInterface>& receiver, 
        const std::string& sender_name, 
        const std::string& sender_game, 
        const std::string& sender_galaxy, 
        const std::wstring& subject, 
        const std::wstring& message, 
        uint32_t message_id,
        uint8_t status,
        const std::vector<char>& attachments, 
        uint32_t timestamp,
        uint8_t header_only);

    void SendChatOnSendInstantMessage(
        const std::shared_ptr<swganh::observer::ObserverInterface>& receiver,
        uint32_t message_id,
        uint32_t recieved_status);

    void SendChatOnSendPersistentMessage(
        const std::shared_ptr<swganh::observer::ObserverInterface>& receiver,
        uint32_t message_id,
        uint32_t recieved_status);

    void SendChatInstantMessageToClient(
        const std::shared_ptr<swganh::observer::ObserverInterface>& receiver, 
        const std::string& sender_name, 
        const std::string& sender_game, 
        const std::string& sender_galaxy, 
        const std::wstring& message);
		
    uint32_t StorePersistentMessage(
        uint64_t recipient_id,
        const std::string& sender_name, 
        const std::string& sender_game, 
        const std::string& sender_galaxy, 
        const std::wstring& subject, 
        const std::wstring& message, 
        const std::vector<char>& attachments,
        uint8_t status,
        uint32_t timestamp);

    void LoadMessageHeaders(const std::shared_ptr<swganh::object::Creature>& receiver);
	void HandleFriendsList(const std::shared_ptr<swganh::object::Creature>& receiver, uint8_t operation);
    std::wstring FilterMessage(const std::wstring& message);

	//Tell Related
	void _handleInstantMessageToCharacter(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatInstantMessageToCharacter* message);
	
	//Mail Related
	void _handlePersistentMessageToServer(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatPersistentMessageToServer* message);

	void _handleRequestPersistentMessage(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatRequestPersistentMessage* message);

	void _handleDeletePersistentMessage(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatDeletePersistentMessage* message);

	//Chat Room Related
	void _handleRequestRoomList(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatRequestRoomList* message);

	void _handleQueryRoom(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatQueryRoom* message);

	void _handleSendToRoom(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatSendToRoom* message);

	void _handleJoinRoom(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatEnterRoomById* message);

	void _handleLeaveRoom(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatLeaveRoom* message);

	//Chat Room Management
	void _handleCreateRoom(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatCreateRoom* message);

	void _handleDestroyRoom(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatDestroyRoom* message);


	//class Room User Management
	void _handleBanAvatar(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatBanAvatar* message);

	void _handleUnbanAvatar(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatUnbanAvatar* message);

	void _handleInviteAvatar(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatInviteAvatar* message);

	void _handleUninviteAvatar(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatUninviteAvatar* message);

	void _handleModeratorAdd(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatModeratorAdd* message);

	void _handleModeratorRemove(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatModeratorRemove* message);

	void _handleKickAvatar(
		const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
		swganh::messages::ChatKickAvatar* message);

	const Room* GetRoomByPath(std::string path);
	Room* GetRoomByPath_(std::string path);
	Room* GetRoomById_(uint32_t id);

	swganh::database::DatabaseManager* db_manager_;
	swganh::command::CommandServiceInterface* command_service_;
    swganh::simulation::SimulationServiceInterface* simulation_service_;
	swganh::equipment::EquipmentServiceInterface* equipment_service_;

    swganh::app::SwganhKernel* kernel_;
	std::shared_ptr<ChatUserProviderInterface> user_provider_;

	mutable boost::mutex room_mutex_;
	std::string galaxy_name_;
	std::map<uint32_t, Room> rooms_;

	std::map<uint64_t, std::set<std::shared_ptr<swganh::observer::ObserverInterface>>> player_observers_;
	std::map<uint64_t, std::shared_ptr<swganh::observer::ObserverInterface>> online_players_;
};

}}  // namespace swganh::chat
