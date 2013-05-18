// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "cell_factory.h"

#include "cell.h"
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

CellFactory::CellFactory(swganh::app::SwganhKernel* kernel)
    : IntangibleFactory(kernel)
{}

void CellFactory::LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock)
{
    IntangibleFactory::LoadFromStorage(connection, object, lock);
}

void CellFactory::RegisterEventHandlers()
{
	GetEventDispatcher()->Subscribe("Cell::Cell", std::bind(&CellFactory::PersistHandler, this, std::placeholders::_1));
}


void CellFactory::PersistChangedObjects()
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

uint32_t CellFactory::PersistObject(const shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited)
{
	// Persist Intangible and Base Object First
    uint32_t counter = 1;
	
    IntangibleFactory::PersistObject(object, lock, persist_inherited);

	try 
    {
		auto conn = GetDatabaseManager()->getConnection("galaxy");
		auto statement = shared_ptr<sql::PreparedStatement>
			(conn->prepareStatement("CALL sp_PersistCell(?);"));
		
		auto cell = static_pointer_cast<Cell>(object);
		statement->setInt(counter++, cell->GetCell(lock));
		statement->executeUpdate();
	}
	catch(sql::SQLException &e)
	{
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}
	return counter;
}

void CellFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{
	ObjectFactory::DeleteObjectFromStorage(object);
}

shared_ptr<Object> CellFactory::CreateObject()
{
	return make_shared<Cell>();
}
