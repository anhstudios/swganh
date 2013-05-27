// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "chat_room_provider_interface.h"
#include <boost/thread/mutex.hpp>

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

	MysqlChatRoomProvider(swganh::app::SwganhKernel* kernel, std::shared_ptr<ChatUserProviderInterface> user_provider);

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

	bool SendToChannel(const std::string& room_path, const std::wstring& message, uint64_t speaker);

	std::shared_ptr<ChatUserProviderInterface> GetUserProvider();

private:

	Room* GetRoomByPath_(std::string path);
	Room* GetRoomByPathHelper_(Room* room, const std::string& path, size_t current);

	boost::mutex mutex_;
	std::shared_ptr<ChatUserProviderInterface> user_provider_;
	RoomMap top_level_rooms_;
	std::map<uint64_t, std::set<std::reference_wrapper<const Room>>> user_rooms_;
};

}
}