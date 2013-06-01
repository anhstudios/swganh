// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <unordered_map>
#include <boost/thread/mutex.hpp>
#include "chat_user_provider_interface.h"

namespace swganh
{
namespace app
{
	class SwganhKernel;
}

namespace chat
{

class MysqlChatUserProvider : public ChatUserProviderInterface
{
public:

	MysqlChatUserProvider(swganh::app::SwganhKernel* kernel);

	uint64_t GetIdFromFullName(const std::string& name) const;
	
	uint64_t GetIdFromFirstName(const std::string& name) const;

	const std::string& GetFullNameFromId(uint64_t creature_id) const;
	
	std::string GetFirstNameFromId(uint64_t creature_id) const;
	
	const std::string& GetFullNameFromFirstName(const std::string& name) const;

	std::string GetFirstNameFromFullName(const std::string& name) const;
	
private:

	static const std::string unk_string_;

	mutable boost::mutex mutex_;
	std::unordered_map<std::string, uint64_t> nameToId;
	std::unordered_map<uint64_t, std::string> IdToName;

};

}
}