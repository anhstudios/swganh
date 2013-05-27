// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <functional>
#include <string>
#include <cstdint>
#include <map>
#include <set>


namespace swganh
{
namespace observer
{
	class ObserverInterface;
}

namespace chat
{

struct Room
{
	uint32_t id_;
	bool is_private_, is_muted_, is_heading_, is_hidden_;
	std::string name_, title_;
	uint64_t owner_, creator_;
	std::set<uint64_t> moderators_, invited_users_, banned_users_;
	std::set<std::shared_ptr<swganh::observer::ObserverInterface>> users_;
};

class ChatUserProviderInterface;
class ChatRoomProviderInterface
{
public:

	virtual void SendRoomList(const std::shared_ptr<swganh::observer::ObserverInterface>& client) = 0;

	virtual void SendQueryResults(const std::shared_ptr<swganh::observer::ObserverInterface>& client, const std::string& path) = 0;

	/*!
	* @param path the path to the room which is requested
	* @return a room that is associated with this path, or nullptr
	*/
	virtual const Room* GetRoomByPath(std::string path) = 0;

	/*!
	* @param name the name of the user who is requested
	* @return the rooms that the given user has entered.
	*/
	virtual std::reference_wrapper<std::set<std::reference_wrapper<const Room>>> GetUsersRooms(const std::string& name) = 0;

	//Channel Management
	/*!
	* @param room_path the path of the room to create
	* @param owner the user who owns this channel
	* @return true if the channel was created
	*/
	virtual bool CreateChannel(const std::string& room_path, uint64_t owner) = 0;
	
	/*!
	* @param room_path the path of the room to destroy
	* @param requester the person who is asking to destroy the room
	* @return true if requester is the owner and the room was destroyed
	*/
	virtual bool DestroyChannel(const std::string& room_path, uint64_t requester) = 0;

	//Add user to channel
	/*!
	* @param room_path the path of the room to enter
	* @param user the user who wants to enter
	*/
	virtual bool EnterChannel(const std::string& room_path, uint64_t user) = 0;
	
	/*!
	* @param room_path the path of the room to exit
	* @param user the user who wants to exit
	*/
	virtual void LeaveChannel(const std::string& room_path, uint64_t user) = 0;

	/*!
	* @param room_path the room to invite a user into
	* @param requester the inviting user
	* @param user the user to invite
	* @return true if the user was invited
	*/
	virtual bool InviteUser(const std::string& room_path, uint64_t requester, uint64_t user) = 0;

	/*!
	* @param room_path the room to uninvite a user from
	* @param requester the user uninviting
	* @param user the user to invite
	* @return true if the user was uninvited
	*/
	virtual bool UninviteUser(const std::string& room_path, uint64_t requester, uint64_t user) = 0;

	//Remove Moderators
	/*!
	* @param room_path the path of the room to modify
	* @param requester the user who wants to add the user
	* @param new_moderator the new user to turn into a moderator.
	* @return true if the requester is the owner and the moderator was added
	*/
	virtual bool AddModerator(const std::string& room_path, uint64_t requester, uint64_t new_moderator) = 0;
	
	/*!
	* @param room_path the path of the room to modify
	* @param requester the user who wants to remove the user
	* @param old_moderator the old user to turn into a normal user.
	*/
	virtual bool RemoveModerator(const std::string& room_path, uint64_t requester, uint64_t old_moderator) = 0;

	//Ownership methods
	/*!
	* @param room_path the path of the room to modify
	* @param requester the user who wishes to transfer ownership
	* @param new_owner the user to turn into the owner
	* @return true if the requester is the current owner and the ownership was changed
	*/
	virtual bool TransferOwnership(const std::string& room_path, uint64_t requester, uint64_t new_owner) = 0;

	//Management methods
	/*!
	* @param room_path the path of the room to kick from
	* @param kicker the user doing the kicking
	* @param kicked the user being kicked
	* @return true if the kicker is a moderator and the user was kicked
	*/
	virtual bool KickUser(const std::string& room_path, uint64_t kicker, uint64_t kicked) = 0;

	/*!
	* @param room_path the room to ban the user from
	* @param requester the user being banned
	* @param banned_user the user being banned
	* @return true if the requester is a moderator and the user was banned
	*/
	virtual bool BanUser(const std::string& room_path, uint64_t requester, uint64_t banned_user) = 0;
	
	/*!
	* @param room_path the room to unban the user from
	* @param requester the user doing the unbanning
	* @param unbanned_user the user being unbanned
	* @return true if the requester is a moderator and the user was banned
	*/
	virtual bool UnbanUser(const std::string& room_path, uint64_t requester, uint64_t unbanned_user) = 0;

	/*!
	* @param room_id the room to send the chat to
	* @param message the message to send
	* @param speaker the user doing the speaking
	* @return true if the user is in the channel, has permissions to do the speaking, and the speaking was performed
	*/
	virtual bool SendToChannel(
		uint32_t room_id, 
		const std::wstring& message, 
		const std::shared_ptr<swganh::observer::ObserverInterface>& client, 
		uint32_t request_id) = 0;

	/*!
	* @return the user provider that is associated with this room provider.
	*/
	virtual std::shared_ptr<ChatUserProviderInterface> GetUserProvider() = 0;

};

}
}