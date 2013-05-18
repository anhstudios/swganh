// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh_core/object/tangible/tangible_factory.h"
#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "swganh/logger.h"

#include "swganh/database/database_manager.h"
#include "swganh_core/object/tangible/tangible.h"
#include "swganh_core/object/exception.h"
#include "swganh_core/simulation/simulation_service_interface.h"

using namespace std;
using namespace swganh::database;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::simulation;

TangibleFactory::TangibleFactory(swganh::app::SwganhKernel* kernel)
    : ObjectFactory(kernel)
{	
}

void TangibleFactory::LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock)
{
    ObjectFactory::LoadFromStorage(connection, object, lock);

    auto tangible = std::dynamic_pointer_cast<Tangible>(object);
    if(!tangible)
    {
        throw InvalidObject("Object requested for loading is not Intangible");
    }

    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetTangible(?);"));
    
    statement->setUInt64(1, tangible->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {
        while (result->next())
        {        
            tangible->SetCustomization(result->getString("customization"), lock);
            tangible->SetOptionsMask(result->getUInt("options_bitmask"), lock);
            tangible->SetCounter(result->getUInt("incap_timer"), lock);
            tangible->SetConditionDamage(result->getUInt("condition_damage"), lock);
            tangible->SetMaxCondition(result->getUInt("max_condition"), lock);
            tangible->SetStatic(result->getBoolean("is_static"), lock);
        }
    } while(statement->getMoreResults());
}

void TangibleFactory::RegisterEventHandlers()
{
	GetEventDispatcher()->Subscribe("Tangible::Customization", std::bind(&TangibleFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Tangible::ComponentCustomization", std::bind(&TangibleFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Tangible::OptionsMask", std::bind(&TangibleFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Tangible::Counter", std::bind(&TangibleFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Tangible::ConditionDamage", std::bind(&TangibleFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Tangible::UpdateAttribute", std::bind(&TangibleFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Tangible::MaxCondition", std::bind(&TangibleFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Tangible::Static", std::bind(&TangibleFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Tangible::Defenders", std::bind(&TangibleFactory::PersistHandler, this, std::placeholders::_1));
}

 
void TangibleFactory::PersistChangedObjects()
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

uint32_t TangibleFactory::PersistObject(const shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited)
{
	uint32_t counter = 1;

	ObjectFactory::PersistObject(object, lock, persist_inherited);

    try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>
            (conn->prepareStatement("CALL sp_PersistTangible(?,?,?,?,?,?,?);"));
        // cast to tangible
        auto tangible = static_pointer_cast<Tangible>(object);
		statement->setUInt64(counter++, tangible->GetObjectId(lock));
        statement->setString(counter++, tangible->GetCustomization(lock));
        statement->setInt(counter++, tangible->GetOptionsMask(lock));
        statement->setInt(counter++, tangible->GetCounter(lock));
        statement->setInt(counter++, tangible->GetCondition(lock));
        statement->setInt(counter++, tangible->GetMaxCondition(lock));
        statement->setBoolean(counter++, tangible->IsStatic(lock));
        statement->executeUpdate();		
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
	return counter;
}

void TangibleFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{
	ObjectFactory::DeleteObjectFromStorage(object);
}

shared_ptr<Object> TangibleFactory::CreateObject()
{
	return make_shared<Tangible>();
}
