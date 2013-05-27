#include <boost/algorithm/string.hpp>
#include "mysql_chat_room_provider.h"

MysqlChatRoomProvider::MysqlChatRoomProvider(swganh::app::SwganhKernel* kernel)
	: user_provider_(kernel->GetPluginManager()->CreateObject<ChatUserProviderInterface>("Chat::UserProvider"))
{
	//Use kernel/db to load default chat room information
	auto database_manager = kernel_->GetDatabaseManager();
	auto conn = database_manager->getConnection("galaxy");
}

const Room* MysqlChatRoomProvider::GetRoomByPath(std::string path)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	return GetRoomByPath(path);
}

Room* GetRoomByPath_(std::string path)
{
	//Cleanup the input
	std::string first(path);
	std::transform(path.begin(), path.end(), path.begin(), ::tolower);

	//Do the path
	std::vector<std::string> pieces;
	boost::split(pieces, path, boost::is_any_of("."));
	if(pieces.size() >= 1) 
	{
		auto find_itr = top_level_rooms_.find(pieces[0]);
		if(find_itr != top_level_rooms_.end())
		{
			return GetRoomByPathHelper_(find_itr->second, pieces, 1);
		}
	}
	return nullptr;
}

Room* GetRoomByPathHelper_(Room* room, const vector<std::string>& path, size_t current)
{
	//Base case
	if(pieces.size() == current) 
	{
		return room;
	}

	//Recursive case
	auto find_itr = room->sub_rooms_.find(pieces[current]);
	if(find_itr != room->sub_rooms_.end())
	{
		return GetRoomByPathHelper_(find_itr->second, pieces, ++current);
	}
	return nullptr;
}

std::reference_wrapper<std::set<std::reference_wrapper<const Room>>> MysqlChatRoomProvider::GetUsersRooms(const std::string& name)
{
	uint64_t id = user_provider_.GetIdFromFullName(name);

	boost::lock_guard<boost::mutex> lock_(mutex_);
	
}

bool MysqlChatRoomProvider::CreateChannel(const std::string& room_path, uint64_t owner)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
}
	
bool MysqlChatRoomProvider::DestroyChannel(const std::string& room_path, uint64_t requester)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);

}

bool MysqlChatRoomProvider::EnterChannel(const std::string& room_path, uint64_t user)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomByPath_(room_path))
	{
	}
	return false;
}

void MysqlChatRoomProvider::LeaveChannel(const std::string& room_path, uint64_t user)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomByPath_(room_path))
	{
	}
	return false;
}

bool MysqlChatRoomProvider::InviteUser(const std::string& room_path, uint64_t requester, uint64_t user)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomByPath_(room_path))
	{
	}
	return false;
}

bool MysqlChatRoomProvider::UninviteUser(const std::string& room_path, uint64_t requester, uint64_t user)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomByPath_(room_path))
	{
	}
	return false;
}

bool MysqlChatRoomProvider::AddModerator(const std::string& room_path, uint64_t requester, uint64_t new_moderator)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomByPath_(room_path))
	{
	}
	return false;
}
	
bool MysqlChatRoomProvider::RemoveModerator(const std::string& room_path, uint64_t requester, uint64_t old_moderator)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomByPath_(room_path))
	{
	}
	return false;
}

bool MysqlChatRoomProvider::TransferOwnership(const std::string& room_path, uint64_t requester, uint64_t new_owner)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomByPath_(room_path))
	{
	}
	return false;
}

bool MysqlChatRoomProvider::KickUser(const std::string& room_path, uint64_t kicker, uint64_t kicked)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomByPath_(room_path)
	{
	}
	return false;
}

bool MysqlChatRoomProvider::BanUser(const std::string& room_path, uint64_t requester, uint64_t banned_user)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomByPath_(room_path))
	{
	}
	return false;
}
	
bool MysqlChatRoomProvider::UnbanUser(const std::string& room_path, uint64_t requester, uint64_t unbanned_user)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomByPath_(room_path))
	{
	}
	return false;
}

bool MysqlChatRoomProvider::SendToChannel(const std::string& room_path, const std::wstring& message, uint64_t speaker)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomByPath_(room_path))
	{
	}
	return false;
}

std::shared_ptr<ChatUserProviderInterface> MysqlChatRoomProvider::GetUserProvider()
{
	return user_provider_;
}