// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "intangible_factory.h"

#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "swganh/logger.h"

#include "swganh/database/database_manager.h"
#include "swganh_core/object/intangible/intangible.h"
#include "swganh_core/object/exception.h"
#include "swganh_core/simulation/simulation_service_interface.h"

using namespace std;
using namespace swganh::database;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::simulation;

IntangibleFactory::IntangibleFactory(swganh::app::SwganhKernel* kernel)
    : ObjectFactory(kernel)
{
}

void IntangibleFactory::LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object)
{
    ObjectFactory::LoadFromStorage(connection, object);

    auto intangible = std::dynamic_pointer_cast<Intangible>(object);
    if(!intangible)
    {
        throw InvalidObject("Object requested for loading is not Intangible");
    }

    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetIntangible(?);"));
    
    statement->setUInt64(1, intangible->GetObjectId());

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());

    do
    { 
        while (result->next())
        {
            intangible->SetGenericInt(result->getInt("generic_int"));
        }
    } while(statement->getMoreResults());
}

uint32_t IntangibleFactory::PersistObject(const shared_ptr<Object>& object, bool persist_inherited)
{
	// Persist Intangible
    uint32_t counter = 1;
	if (persist_inherited)
		ObjectFactory::PersistObject(object, persist_inherited);
	try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>
            (conn->prepareStatement("CALL sp_PersistIntangible(?,?,?,?);"));
        auto tangible = static_pointer_cast<Intangible>(object);
		statement->setUInt64(counter++, tangible->GetObjectId());
		statement->setString(counter++, tangible->GetStfNameFile());
		statement->setString(counter++, tangible->GetStfNameString());
		statement->setInt(counter++, tangible->GetGenericInt());
		statement->executeUpdate();
	}
	catch(sql::SQLException &e)
	{
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}
	return counter;
}

void IntangibleFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{
	ObjectFactory::DeleteObjectFromStorage(object);
}

shared_ptr<Object> IntangibleFactory::CreateObject()
{
	return make_shared<Intangible>();
}
