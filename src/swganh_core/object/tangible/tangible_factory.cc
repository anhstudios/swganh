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
		if(object->IsDatabasePersisted())
			PersistObject(object);
	}
}

uint32_t TangibleFactory::PersistObject(const shared_ptr<Object>& object, bool persist_inherited)
{
	uint32_t counter = 1;
	if (persist_inherited)
		ObjectFactory::PersistObject(object, persist_inherited);
    try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>
            (conn->prepareStatement("CALL sp_PersistTangible(?,?,?,?,?,?,?);"));
        // cast to tangible
        auto tangible = static_pointer_cast<Tangible>(object);
		statement->setUInt64(counter++, tangible->GetObjectId());
        statement->setString(counter++, tangible->GetCustomization());
        statement->setInt(counter++, tangible->GetOptionsMask());
        statement->setInt(counter++, tangible->GetCounter());
        statement->setInt(counter++, tangible->GetCondition());
        statement->setInt(counter++, tangible->GetMaxCondition());
        statement->setBoolean(counter++, tangible->IsStatic());
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
void TangibleFactory::CreateTangible(const shared_ptr<Tangible>& tangible, const std::shared_ptr<sql::Statement>& statement)
{
    try {
        auto result = shared_ptr<sql::ResultSet>(statement->getResultSet());
        CreateBaseObjectFromStorage(tangible, result);
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
                tangible->SetCustomization(result->getString("customization"));
                tangible->SetOptionsMask(result->getUInt("options_bitmask"));
                tangible->SetCounter(result->getUInt("incap_timer"));
                tangible->SetConditionDamage(result->getUInt("condition_damage"));
                tangible->SetMaxCondition(result->getUInt("max_condition"));
                tangible->SetStatic(result->getBoolean("is_static"));
            }
        }

		//Clear us from the db persist update queue.
		boost::lock_guard<boost::mutex> lock(persisted_objects_mutex_);
		auto find_itr = persisted_objects_.find(tangible);
		if(find_itr != persisted_objects_.end())
			persisted_objects_.erase(find_itr);
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
shared_ptr<Object> TangibleFactory::CreateObjectFromStorage(uint64_t object_id)
{
   auto tangible = make_shared<Tangible>();
   tangible->SetObjectId(object_id);
   CreateTangibleFromStorage(tangible);
   return tangible;
}

void TangibleFactory::CreateTangibleFromStorage(shared_ptr<Tangible> tangible)
{
	try {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::Statement>(conn->createStatement());
        
        stringstream ss;
        ss << "CALL sp_GetTangible(" << tangible->GetObjectId() << ");";   

        statement->execute(ss.str());
        CreateTangible(tangible, statement);  
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

shared_ptr<Object> TangibleFactory::CreateObject()
{
	return make_shared<Tangible>();
}
