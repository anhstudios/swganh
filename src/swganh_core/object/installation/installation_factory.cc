// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "installation_factory.h"

#include "installation.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "swganh/logger.h"

#include "swganh/database/database_manager.h"

using namespace std;
using namespace swganh::object;

InstallationFactory::InstallationFactory(swganh::app::SwganhKernel* kernel)
	: TangibleFactory(kernel)
{		
}
void InstallationFactory::RegisterEventHandlers()
{
	GetEventDispatcher()->Subscribe("Installation::Active", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Installation::PowerReserve", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Installation::PowerCost", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Installation::AvailableResource", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Installation::DisplayedMaxExtraction", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Installation::MaxExtraction", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Installation::CurrentExtraction", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Installation::CurrentHopperSize", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Installation::MaxHopperSize", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Installation::IsUpdating", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Installation::Hopper", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Installation::IsUpdating", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Installation::ConditionPercent", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Installation::SelectedResource", std::bind(&InstallationFactory::PersistHandler, this, std::placeholders::_1));	
}
void InstallationFactory::PersistChangedObjects()
{
	std::set<shared_ptr<Object>> persisted;
	{
		boost::lock_guard<boost::mutex> lg(persisted_objects_mutex_);
		persisted = move(persisted_objects_);
	}
	for (auto& object : persisted)
	{
		PersistObject(object);
	}
}
uint32_t InstallationFactory::PersistObject(const shared_ptr<Object>& object)
{
	uint32_t counter = 1;
	try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>
			(conn->prepareStatement("CALL sp_PersistInstallation(?,?,?,?,?,?,?,?,?,?);"));        

		auto installation = static_pointer_cast<Installation>(object);
		statement->setUInt64(counter++, installation->GetObjectId());
		statement->setUInt64(counter++, installation->GetSelectedResourceId());
		statement->setInt(counter++, installation->IsActive() == true ? 1 : 0);
		statement->setDouble(counter++, installation->GetPowerReserve());
		statement->setDouble(counter++, installation->GetPowerCost());
		statement->setDouble(counter++, installation->GetMaxExtractionRate());
		statement->setDouble(counter++, installation->GetCurrentExtractionRate());
		statement->setDouble(counter++, installation->GetCurrentHopperSize());
		statement->setInt(counter++, installation->IsUpdating() == true ? 1 : 0);
		statement->setDouble(counter++, installation->GetConditionPercentage());

		statement->executeUpdate();
	}
	catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

	return counter;
}

void InstallationFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{
	ObjectFactory::DeleteObjectFromStorage(object);
}

shared_ptr<Object> InstallationFactory::CreateObjectFromStorage(uint64_t object_id)
{

	auto installation = make_shared<Installation>();
    installation->SetObjectId(object_id);
    try {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::Statement>(conn->createStatement());
        unique_ptr<sql::ResultSet> result;
        stringstream ss;
        ss << "CALL sp_GetInstallation(" << object_id << ");" ;
        statement->execute(ss.str());
        CreateTangible(installation, statement);
        
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
				installation->SetSelectedResourceId(result->getUInt64("selected_resource_id"));
				int active = result->getInt("is_active");
				if (active == 1)
					installation->Activate();
				installation->SetPowerReserve(static_cast<float>(result->getDouble("power_reserve")));
				installation->SetPowerCost(static_cast<float>(result->getDouble("power_cost")));
				installation->SetMaxExtractionRate(static_cast<float>(result->getDouble("max_extraction_rate")));
				installation->SetCurrentExtractionRate(static_cast<float>(result->getDouble("current_extraction_rate")));
				installation->SetCurrentHopperSize(static_cast<float>(result->getDouble("current_hopper_size")));
				int updating = result->getInt("is_updating");
				if (updating == 1)
					installation->StartUpdating();

				installation->SetConditionPercentage(result->getInt("condition_percentage"));
			}
		}

		//Clear us from the db persist update queue.
		boost::lock_guard<boost::mutex> lock(persisted_objects_mutex_);
		auto find_itr = persisted_objects_.find(installation);
		if(find_itr != persisted_objects_.end())
			persisted_objects_.erase(find_itr);
	}
	catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return make_shared<Installation>();
}

shared_ptr<Object> InstallationFactory::CreateObject()
{
    return make_shared<Installation>();
}
