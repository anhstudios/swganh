#include <boost/algorithm/string.hpp>
#include <boost/thread/locks.hpp>
#include "mysql_chat_room_provider.h"
#include "chat_user_provider_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/plugin/plugin_manager.h"
#include "swganh/database/database_manager.h"
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include "swganh/observer/observer_interface.h"

#include "swganh_core/messages/chat/chat_room_list.h"
#include "swganh_core/messages/chat/chat_on_send_room_message.h"
#include "swganh_core/messages/chat/chat_room_message.h"

using swganh::chat::MysqlChatRoomProvider;
using swganh::chat::Room;
using namespace swganh::messages;

MysqlChatRoomProvider::MysqlChatRoomProvider(swganh::app::SwganhKernel* kernel)
	: user_provider_(kernel->GetPluginManager()->CreateObject<ChatUserProviderInterface>("Chat::UserProvider"))
	, galaxy_name_(kernel->GetAppConfig().galaxy_name)
{
	//Use kernel/db to load default chat room information
	auto database_manager = kernel->GetDatabaseManager();
	auto conn = database_manager->getConnection("galaxy");
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
			room.is_heading_ = false;
			room.is_hidden_ = false;
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
}

const Room* MysqlChatRoomProvider::GetRoomByPath(std::string path)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	return GetRoomByPath_(path);
}

Room* MysqlChatRoomProvider::GetRoomByPath_(std::string path)
{
	auto find_itr = std::find_if(rooms_.begin(), rooms_.end(), [&] (const std::pair<uint32_t, Room>& room) {
		return room.second.name_ == path;
	});
	return (find_itr != rooms_.end()) ? &(find_itr->second) : nullptr;
}

void MysqlChatRoomProvider::SendRoomList(const std::shared_ptr<swganh::observer::ObserverInterface>& client)
{
	ChatRoomList room_list;
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		room_list.data.write<uint32_t>(rooms_.size());
		for(auto& room_pair : rooms_)
		{
			Room& room = room_pair.second;

			if(room.is_hidden_)
				continue;

			room_list.data.write<uint32_t>(room_pair.first);
			room_list.data.write<uint32_t>((room.is_private_) ? 1 : 0);
			room_list.data.write<uint8_t>((room.is_muted_) ? 1 : 0);
			room_list.data.write<std::string>(room.name_);
			
			room_list.data.write<std::string>("SWG");
			room_list.data.write<std::string>(galaxy_name_);
			room_list.data.write<std::string>(user_provider_->GetFullNameFromId(room.owner_));
			
			room_list.data.write<std::string>("SWG");
			room_list.data.write<std::string>(galaxy_name_);
			room_list.data.write<std::string>(user_provider_->GetFullNameFromId(room.creator_));
			
			room_list.data.write<std::wstring>(std::wstring(room.title_.begin(), room.title_.end()));
		
			//Write out empty lists to save bandwidth (only the result of the Query is displayed.)
			room_list.data.write<uint32_t>(0);
			room_list.data.write<uint32_t>(0);
		}
	}
	client->Notify(&room_list);
}

void MysqlChatRoomProvider::SendQueryResults(const std::shared_ptr<swganh::observer::ObserverInterface>& client, const std::string& path)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomByPath_(path))
	{

	}
}

std::reference_wrapper<std::set<std::reference_wrapper<const Room>>> MysqlChatRoomProvider::GetUsersRooms(const std::string& name)
{
	uint64_t id = user_provider_->GetIdFromFullName(name);

	boost::lock_guard<boost::mutex> lock_(mutex_);
	return std::ref(user_rooms_.find(id)->second);
}

bool MysqlChatRoomProvider::CreateChannel(const std::string& room_path, uint64_t owner)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(GetRoomByPath_(room_path) == nullptr)
	{
	}
	return false;
}
	
bool MysqlChatRoomProvider::DestroyChannel(const std::string& room_path, uint64_t requester)
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomByPath_(room_path))
	{
	}
	return false;
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
	if(Room* room = GetRoomByPath_(room_path))
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

bool MysqlChatRoomProvider::SendToChannel(uint32_t room_id, const std::wstring& message, 
	const std::shared_ptr<swganh::observer::ObserverInterface>& client, uint32_t request_id)
{
	uint64_t speaker = client->GetId();

	boost::lock_guard<boost::mutex> lock_(mutex_);
	if(Room* room = GetRoomById_(room_id))
	{
		uint32_t error_code = 0;
		if(room->users_.find(client) == room->users_.end())
		{
			error_code = 1; //Not sent
		}
		else if(room->is_muted_ && (room->moderators_.find(speaker) == room->moderators_.end()))
		{
			error_code = 9;
		}
		
		//Send Status Message
		client->Notify(&ChatOnSendRoomMessage(error_code, request_id));
		
		if(error_code == 0)
		{
			//Send message to all users in channel
			ChatRoomMessage msg;
			msg.server_name = galaxy_name_;
			msg.sender_character_name = user_provider_->GetFullNameFromId(speaker);
			msg.channel_id = room_id;
			msg.message = message;

			for(auto& user : room->users_)
			{
				user->Notify(&msg);
			}
			return true;
		}
	}
	return false;
}

std::shared_ptr<swganh::chat::ChatUserProviderInterface> MysqlChatRoomProvider::GetUserProvider()
{
	return user_provider_;
}