#include "myql_chat_user_provider.h"
#include <algorithm>

MysqlChatUserProvider::MysqlChatUserProvider(swganh::app::SwganhKernel* kernel);
{
	//Grab the current user data from the DB
	{
		boost::lock_guard<boost::mutex> lock_(mutex_);
		auto database_manager = kernel_->GetDatabaseManager();
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
			nameToId.insert(std::make_pair<std::string, uint64_t>(name, id));
			IdToName.insert(std::make_pair<uint64_t, std::string>(id, name));
		}
	}

	//Hook into the Event Dispatcher for creation of new players
	kernel->GetEventDispatcher()->Subscribe("Character::NewCharacter", [] (const std::shared_ptr<EventInterface>& event_interface) {
		auto real_event = std::static_pointer_cast<ValueEvent<std::pair<uint64_t, std::string>>>(event_interface);
		auto& data = real_event->Get();

		boost::lock_guard<boost::mutex> lock_(mutex_);
		nameToId.insert(std::make_pair<std::string, uint64_t>(data.second, data.first));
		IdToName.insert(std::make_pair<uint64_t, std::string>(data.first, data.second));
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
	return "UNKNOWN";
}
	
const std::string& MysqlChatUserProvider::GetFirstNameFromId(uint64_t creature_id) const
{
	boost::lock_guard<boost::mutex> lock_(mutex_);
	return GetFirstNameFromFullName(GetFullNameFromId(creature_id));
}
	
std::string MysqlChatUserProvider::GetUsernamePathFromId(uint64_t creature_id) const
{
	return "SWG.username."+GetFirstNameFromId(creature_id);
}
	
std::string MysqlChatUserProvider::GetFirstNameFromFullName(const std::string& name) const
{
	std::string first(name.substr(0, name.find(' ')));
	std::transform(first.begin(), first.end(), first.begin(), ::tolower);
	return first;
}