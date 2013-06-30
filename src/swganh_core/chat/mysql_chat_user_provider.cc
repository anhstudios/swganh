#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/thread/locks.hpp>
#include "mysql_chat_user_provider.h"

#include "swganh/event_dispatcher.h"
#include "swganh/app/swganh_kernel.h"

#include "swganh/database/database_manager.h"
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

using swganh::chat::MysqlChatUserProvider;

const std::string MysqlChatUserProvider::unk_string_ = "SYSTEM";

MysqlChatUserProvider::MysqlChatUserProvider(swganh::app::SwganhKernel* kernel)
{
	//Grab the current user data from the DB
	{
		try
		{
			boost::lock_guard<boost::mutex> lock_(mutex_);
			auto database_manager = kernel->GetDatabaseManager();
			auto conn = database_manager->getConnection("galaxy");
			auto statement = std::shared_ptr<sql::Statement>(conn->createStatement());
			statement->execute("CALL sp_LoadCharacterNames();");

			std::unique_ptr<sql::ResultSet> result(statement->getResultSet());
			while(result->next())
			{
				//Get Data
				uint64_t id = result->getUInt64(1);
				std::string name = result->getString(2);

				//Insert into the proper datastructures
				nameToId.insert(std::make_pair(GetFirstNameFromFullName(name), id));
				IdToName.insert(std::make_pair(id, std::move(name)));
			} while(statement->getMoreResults());
		}
		catch(sql::SQLException &e) 
		{
			LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
			LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
		}
	}

	//Hook into the Event Dispatcher for creation of new players
	kernel->GetEventDispatcher()->Subscribe("Character::NewCharacter", [this] (const std::shared_ptr<EventInterface>& event_interface) {
		auto real_event = std::static_pointer_cast<ValueEvent<std::pair<uint64_t, std::string>>>(event_interface);
		auto& data = real_event->Get();

		boost::lock_guard<boost::mutex> lock_(mutex_);
		nameToId.insert(std::make_pair(GetFirstNameFromFullName(data.second), data.first));
		IdToName.insert(std::make_pair(data.first, std::move(data.second)));
	});
}

uint64_t MysqlChatUserProvider::GetIdFromFullName(const std::string& name) const
{
	return GetIdFromFirstName(GetFirstNameFromFullName(name));
}
	
uint64_t MysqlChatUserProvider::GetIdFromFirstName(const std::string& name) const
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	auto find_itr = nameToId.find(name);
	if(find_itr != nameToId.end())
	{
		return find_itr->second;
	}
	return 0;
}

const std::string& MysqlChatUserProvider::GetFullNameFromId(uint64_t creature_id) const
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	auto find_itr = IdToName.find(creature_id);
	if(find_itr != IdToName.end())
	{
		return find_itr->second;
	}
	return unk_string_;
}
	
const std::string& MysqlChatUserProvider::GetFullNameFromFirstName(const std::string& name) const
{
	return GetFullNameFromId(GetIdFromFirstName(name));
}

std::string MysqlChatUserProvider::GetFirstNameFromId(uint64_t creature_id) const
{
	if(creature_id)
	{
		return GetFirstNameFromFullName(GetFullNameFromId(creature_id));
	}
	else
	{
		return "SYSTEM";
	}
}
	
std::string MysqlChatUserProvider::GetFirstNameFromFullName(const std::string& name) const
{
	std::string first(name.substr(0, name.find(' ')));
	boost::algorithm::to_lower(first);
	return first;
}