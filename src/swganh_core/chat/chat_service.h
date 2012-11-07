// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "swganh/chat/chat_service_interface.h"
#include "swganh/command/command_service_interface.h"
#include "swganh/simulation/simulation_service_interface.h"

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

    uint64_t GetReceiverIdByCustomName(std::string custom_name);

private:
    swganh::database::DatabaseManagerInterface* db_manager_;
	swganh::command::CommandServiceInterface* command_service_;
    swganh::simulation::SimulationServiceInterface* simulation_service_;
    swganh::app::SwganhKernel* kernel_;

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

    void PersistMessage(std::shared_ptr<swganh::object::Object> receiver, std::string sender_name, std::string sender_game, std::string sender_galaxy, 
        std::wstring subject, std::wstring message, std::vector<char> attachments, uint32_t timestamp);
    
    uint32_t PersistMessage(uint64_t receiver_id, std::string sender_name, std::string sender_game, std::string sender_galaxy, 
        std::wstring subject, std::wstring message, std::vector<char> attachments, uint32_t timestamp);

    void LoadMessageHeaders(std::shared_ptr<swganh::object::Object> receiver);
};

}}  // namespace swganh::chat
