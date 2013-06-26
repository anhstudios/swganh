// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <set>

#include "swganh/byte_buffer.h"

namespace swganh
{
namespace observer
{
	class ObserverInterface;
}

namespace chat
{

class ChatUserProviderInterface;

struct Room
{
	uint32_t id_;
	bool is_private_, is_muted_;
	std::string name_, title_;
	uint64_t owner_, creator_;
	std::set<uint64_t> moderators_, invited_users_, banned_users_;
	std::set<std::shared_ptr<swganh::observer::ObserverInterface>> users_;

	inline static void SerializeUser(swganh::ByteBuffer& data, const std::string& galaxy_name_, const std::string& name)
	{
		data.write<std::string>("SWG");
		data.write<std::string>(galaxy_name_);
		data.write<std::string>(name);
	}

	void SerializeBody(swganh::ByteBuffer& data, 
		const std::string& galaxy_name_, 
		std::shared_ptr<ChatUserProviderInterface>& user_provider_);

	void SerializeUsers(swganh::ByteBuffer& data, 
		const std::string& galaxy_name_, 
		std::shared_ptr<ChatUserProviderInterface>& user_provider_);

	static void SerializeIdSet(swganh::ByteBuffer& data,
		const std::string& galaxy_name_,
		std::shared_ptr<ChatUserProviderInterface>& user_provider_,
		const std::set<uint64_t>& data_set);

};

}
}