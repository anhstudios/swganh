// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "installation_factory.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "swganh/logger.h"

#include "swganh/database/database_manager.h"

#include "swganh_core/object/exception.h"

#include "installation.h"

using namespace std;
using namespace swganh::object;

InstallationFactory::InstallationFactory(swganh::app::SwganhKernel* kernel)
	: TangibleFactory(kernel)
{}

void InstallationFactory::LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock)
{    
    TangibleFactory::LoadFromStorage(connection, object, lock);

    auto installation = std::dynamic_pointer_cast<Installation>(object);
    if(!installation)
    {
        throw InvalidObject("Object requested for loading is not Intangible");
    }

    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetInstallation(?);"));
    
    statement->setUInt64(1, installation->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());    
    
    do
    {
        while (result->next())
        {
            installation->SetSelectedResourceId(result->getUInt64("selected_resource_id"), lock);

            if (result->getInt("is_active") == 1)
                installation->Activate(lock);

            installation->SetPowerReserve(static_cast<float>(result->getDouble("power_reserve")), lock);
            installation->SetPowerCost(static_cast<float>(result->getDouble("power_cost")), lock);
            installation->SetMaxExtractionRate(static_cast<float>(result->getDouble("max_extraction_rate")), lock);
            installation->SetCurrentExtractionRate(static_cast<float>(result->getDouble("current_extraction_rate")), lock);
            installation->SetCurrentHopperSize(static_cast<float>(result->getDouble("current_hopper_size")), lock);

            if (result->getInt("is_updating") == 1)
                installation->StartUpdating(lock);
            
            installation->SetConditionPercentage(result->getInt("condition_percentage"), lock);
        }
    } while(statement->getMoreResults());
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
		auto lock = object->AcquireLock();
		if(object->IsDatabasePersisted(lock))
		{
			PersistObject(object, lock);
		}
	}
}
uint32_t InstallationFactory::PersistObject(const shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited)
{
	uint32_t counter = 1;
	
    TangibleFactory::PersistObject(object, lock, persist_inherited);

	try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>
			(conn->prepareStatement("CALL sp_PersistInstallation(?,?,?,?,?,?,?,?,?,?);"));        

		auto installation = static_pointer_cast<Installation>(object);
		statement->setUInt64(counter++, installation->GetObjectId(lock));
		statement->setUInt64(counter++, installation->GetSelectedResourceId(lock));
		statement->setInt(counter++, installation->IsActive(lock) == true ? 1 : 0);
		statement->setDouble(counter++, installation->GetPowerReserve(lock));
		statement->setDouble(counter++, installation->GetPowerCost(lock));
		statement->setDouble(counter++, installation->GetMaxExtractionRate(lock));
		statement->setDouble(counter++, installation->GetCurrentExtractionRate(lock));
		statement->setDouble(counter++, installation->GetCurrentHopperSize(lock));
		statement->setInt(counter++, installation->IsUpdating(lock) == true ? 1 : 0);
		statement->setDouble(counter++, installation->GetConditionPercentage(lock));

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

shared_ptr<Object> InstallationFactory::CreateObject()
{
    return make_shared<Installation>();
}
