// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "waypoint_factory.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "swganh/logger.h"

#include "swganh/database/database_manager.h"
#include "swganh_core/object/waypoint/waypoint.h"
#include "swganh_core/object/player/player_events.h"
#include "swganh_core/object/player/player.h"
#include "swganh_core/object/exception.h"
#include "swganh_core/messages/containers/network_map.h"

using namespace std;
using namespace swganh;
using namespace swganh::database;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::containers;

WaypointFactory::WaypointFactory(swganh::app::SwganhKernel* kernel)
    : IntangibleFactory(kernel)
{
    RegisterEventHandlers();
}

void WaypointFactory::LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock)
{
    IntangibleFactory::LoadFromStorage(connection, object, lock);

    auto waypoint = std::dynamic_pointer_cast<Waypoint>(object);
    if(!waypoint)
    {
        throw InvalidObject("Object requested for loading is not Waypoint");
    }

    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetWaypoint(?);"));
    
    statement->setUInt64(1, waypoint->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {
        while (result->next())
        {
            waypoint->SetCoordinates( glm::vec3(
                result->getDouble("x_coordinate"),
                result->getDouble("y_coordinate"),
                result->getDouble("z_coordinate")), lock);
            //waypoint->SetLocationNetworkId(result->getUInt("scene_id"));
            string custom_string = result->getString("name");
            waypoint->SetName(wstring(begin(custom_string), end(custom_string)), lock);
            waypoint->SetPlanet(result->getString("planet"), lock);

            result->getUInt("is_active") == 0 ? waypoint->DeActivate(lock) : waypoint->Activate(lock);
            waypoint->SetColor(Waypoint::WaypointColor(result->getUInt("color")), lock);
        }
    } while(statement->getMoreResults());
}

void WaypointFactory::RegisterEventHandlers()
{
    auto event_dispatcher = GetEventDispatcher();
    event_dispatcher->Subscribe("Waypoint::Activated", std::bind(&WaypointFactory::PersistHandler, this, std::placeholders::_1));
    event_dispatcher->Subscribe("Waypoint::DeActivated", std::bind(&WaypointFactory::PersistHandler, this, std::placeholders::_1));
    event_dispatcher->Subscribe("Waypoint::Coordinates", std::bind(&WaypointFactory::PersistHandler, this, std::placeholders::_1));
    event_dispatcher->Subscribe("Waypoint::Name", std::bind(&WaypointFactory::PersistHandler, this, std::placeholders::_1));
}

void WaypointFactory::PersistChangedObjects()
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

uint32_t WaypointFactory::PersistObject(const shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited)
{
	uint32_t counter = 1;
	
    if (object) 
    {
        IntangibleFactory::PersistObject(object, lock, persist_inherited);

        try 
        {			
            auto waypoint = static_pointer_cast<Waypoint>(object);
            auto conn = GetDatabaseManager()->getConnection("galaxy");
            auto statement = conn->prepareStatement("CALL sp_PersistWaypoint(?,?,?,?,?,?,?,?,?);");

            auto container = waypoint->GetContainer(lock);

            statement->setUInt64(counter++, waypoint->GetObjectId(lock));
            statement->setUInt64(counter++, container->GetObjectId());
            auto coords = waypoint->GetCoordinates(lock);
            statement->setDouble(counter++, coords.x);
            statement->setDouble(counter++, coords.y);
            statement->setDouble(counter++, coords.z);
            statement->setUInt(counter++, waypoint->GetActiveFlag(lock));
            statement->setString(counter++, waypoint->GetPlanet(lock));
            statement->setString(counter++, waypoint->GetNameStandard(lock));
            statement->setUInt(counter++, waypoint->GetColor(lock));
            statement->execute();						
        }
            catch(sql::SQLException &e)
        {
            LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
            LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
        }		
    }
	return counter;
}

void WaypointFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{
	ObjectFactory::DeleteObjectFromStorage(object);
}

shared_ptr<Object> WaypointFactory::CreateObject()
{
	return make_shared<Waypoint>();
}

