// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "swganh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/messages/controllers/command_queue_enqueue.h"

namespace swganh {
namespace object {
    class Object;
}}  // namespace swganh::object

namespace swganh {
namespace chat {

    class ChatServiceInterface: public swganh::service::BaseService
    {
    public:
        virtual ~ChatServiceInterface() {}

        virtual bool SendPersistentMessage(
            const std::string& recipient,
            const std::string& sender,
            const std::string& sender_game,
            const std::string& sender_galaxy,
            const std::wstring& subject,
            const std::wstring& message,
            const std::vector<char>& attachments) = 0;
        
        virtual bool SendPersistentMessage(
            const std::string& recipient,
            const std::string& sender,
            const std::wstring& subject, 
            const std::wstring& message) = 0;

		/*!
		* Sends a spatial chat message
		* @param actor the speaker
		* @param target the target
		* @param chat_message the message
		* @param chat_type the type of chat
		* @param mood the mood used
		*/
        virtual void SendSpatialChat(
		    const std::shared_ptr<swganh::object::Object>& actor, // creature object
		    const std::shared_ptr<swganh::object::Object>& target,	// target object
            std::wstring chat_message,
            uint16_t chat_type,
            uint16_t mood) = 0;

		/*!
		* Creates a new room with the given information, if possible.
		* @param room_path the full path to the new room, e.g. "SWG.[GALAXY NAME].Chat.myNewChatChannel"
		* @param room_title the title of the new room
		* @param owner_id_ the id of the room's owner, or 0 if SYSTEM
		* @param creator_id_ the id of the room's creator, or 0 if SYSTEM
		* @param is_private_ true if the channel requires an invitation to enter
		* @param is_muted_ true if only moderators can speak on the channel.
		* @return the id of the new channel if the channel was created, or 0
		*/
		virtual uint32_t CreateRoom(const std::string& room_path, const std::string& room_title,
			uint64_t owner_id_, uint64_t creator_id_,
			bool is_private_, bool is_muted_) = 0;

		/*!
		* Destroys any room with the given path
		* @param room_path the path to the channel to destroy, e.g. "SWG.[GALAXY NAME].Chat.myOldChatChannel"
		*/
		virtual void DestroyRoom(const std::string& room_path) = 0;

		/*!
		* Forces a user to join a channel.
		* @param actor the user to force join
		* @param room_path the room to join the user into
		*/
		virtual void ForceJoin(const std::shared_ptr<swganh::object::Object>& actor, const std::string& room_path) = 0;
	
		/*!
		* Forces a user to leave the channel. In other words, a "silent kick"
		* @param actor the user to kick
		* @param room_path the room to kick the user from
		*/
		virtual void ForceLeave(const std::shared_ptr<swganh::object::Object>& actor, const std::string& room_path) = 0;

		/*!
		* Looks up a player with the given custom name and returns his creature's object id.
		* @param custom_name the name of the player to lookup
		* @return the id of the player or 0 if he does not exist
		*/
		virtual uint64_t GetObjectIdByCustomName(const std::string& custom_name) = 0;

		/*!
		* @param creature_id the id of the user's creature
		* @return the full name of the user
		*/
		virtual const std::string& GetFullNameFromId(uint64_t creature_id) const = 0;
	
		/*!
		* @param creature_id the id of the user's creature
		* @return the first name of the user
		*/
		virtual std::string GetFirstNameFromId(uint64_t creature_id) const = 0;
	
		/*!
		* @param name the first name of the player requested
		* @return the full name of the player.
		*/
		virtual const std::string& GetFullNameFromFirstName(const std::string& name) const = 0;
	
		/*!
		* @param name the full name of the user
		* @return the first name of the user
		*/
		virtual std::string GetFirstNameFromFullName(const std::string& name) const = 0;

		/*!
		* Determines player status
		* @param name the name of the player to check
		* @return true if the player is online, false otherwise.
		*/
		virtual bool IsOnline(const std::string& name) const = 0;

		/*!
		* Determines player status
		* @param id the id of the player to check
		* @return true if the player is online, false otherwise.
		*/
		virtual bool IsOnline(uint64_t id) const = 0;

    };

}}  // namespace swganh::chat
