// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "swganh_core/chat/chat_service_interface.h"
#include "swganh_core/command/command_service_interface.h"
#include "swganh_core/simulation/simulation_service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/messages/controllers/command_queue_enqueue.h"

namespace swganh {
namespace connection {
    class ConnectionClientInterface;
}
namespace messages {
    struct ChatInstantMessageToCharacter;
    struct ChatPersistentMessageToServer;
    struct ChatRequestPersistentMessage;
    struct ChatDeletePersistentMessage;
}
namespace object {
	class Object;
}}  // namespace swganh::object

namespace swganh {
namespace chat {

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

	/**
	* @return the service description for this service
	*/
    swganh::service::ServiceDescription GetServiceDescription();

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

	/**
	* Sends a spatial chat message
	* @param actor the speaker
	* @param target the target
	* @param chat_message the message
	* @param chat_type the type of chat
	* @param mood the mood used
	*/
    void SendSpatialChat(
		const std::shared_ptr<swganh::object::Object>& actor, // creature object
		const std::shared_ptr<swganh::object::Object>& target,	// target object
        std::wstring chat_message,
        uint16_t chat_type,
        uint16_t mood);

	/**
	* Called on startup
	*/
    void Startup();

    uint64_t GetObjectIdByCustomName(const std::string& custom_name);

private:
    swganh::database::DatabaseManager* db_manager_;
	swganh::command::CommandServiceInterface* command_service_;
    swganh::simulation::SimulationServiceInterface* simulation_service_;
    swganh::app::SwganhKernel* kernel_;
    
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

    void HandleChatInstantMessageToCharacter(
        const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
        swganh::messages::ChatInstantMessageToCharacter* message);

    void HandleChatPersistentMessageToServer(
        const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
        swganh::messages::ChatPersistentMessageToServer* message);

    void HandleChatRequestPersistentMessage(
        const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
        swganh::messages::ChatRequestPersistentMessage* message);

    void HandleChatDeletePersistentMessage(
        const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
        swganh::messages::ChatDeletePersistentMessage* message);

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

    void LoadMessageHeaders(const std::shared_ptr<swganh::object::Object>& receiver);

    std::wstring FilterMessage(const std::wstring& message);
};

}}  // namespace swganh::chat
