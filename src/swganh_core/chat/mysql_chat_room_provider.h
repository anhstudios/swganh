// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <map>
#include <vector>
#include <boost/thread/mutex.hpp>

#include "chat_room_provider_interface.h"


namespace swganh
{

namespace app
{
	class SwganhKernel;
}

namespace chat
{

class ChatUserProviderInterface;

class MysqlChatRoomProvider
{
public:

	MysqlChatRoomProvider(swganh::app::SwganhKernel* kernel);

	void SendRoomList(const std::shared_ptr<swganh::observer::ObserverInterface>& client);

	void SendQueryResults(const std::shared_ptr<swganh::observer::ObserverInterface>& client, const std::string& path);

	const Room* GetRoomByPath(std::string path);

	std::reference_wrapper<std::set<std::reference_wrapper<const Room>>> GetUsersRooms(const std::string& name);

	bool CreateChannel(const std::string& room_path, uint64_t owner);
	
	bool DestroyChannel(const std::string& room_path, uint64_t requester);

	bool EnterChannel(const std::string& room_path, uint64_t user);

	void LeaveChannel(const std::string& room_path, uint64_t user);

	bool InviteUser(const std::string& room_path, uint64_t requester, uint64_t user);

	bool UninviteUser(const std::string& room_path, uint64_t requester, uint64_t user);

	bool AddModerator(const std::string& room_path, uint64_t requester, uint64_t new_moderator);
	
	bool RemoveModerator(const std::string& room_path, uint64_t requester, uint64_t old_moderator);

	bool TransferOwnership(const std::string& room_path, uint64_t requester, uint64_t new_owner);

	bool KickUser(const std::string& room_path, uint64_t kicker, uint64_t kicked);

	bool BanUser(const std::string& room_path, uint64_t requester, uint64_t banned_user);
	
	bool UnbanUser(const std::string& room_path, uint64_t requester, uint64_t unbanned_user);

	bool SendToChannel(
		uint32_t room_id, 
		const std::wstring& message, 
		const std::shared_ptr<swganh::observer::ObserverInterface>& client, 
		uint32_t request_id);

	std::shared_ptr<swganh::chat::ChatUserProviderInterface> GetUserProvider();

private:

	Room* GetRoomByPath_(std::string path);
	Room* GetRoomById_(uint32_t);

	mutable boost::mutex mutex_;

	std::string galaxy_name_;
	std::shared_ptr<swganh::chat::ChatUserProviderInterface> user_provider_;
	std::map<uint32_t, Room> rooms_;
	std::map<uint64_t, std::set<std::reference_wrapper<const Room>>> user_rooms_;
};

}
}