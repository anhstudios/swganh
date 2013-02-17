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
#include "swganh_core/simulation/simulation_service_interface.h"
#include "swganh_core/messages/containers/network_map.h"

using namespace std;
using namespace swganh;
using namespace swganh::database;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::simulation;
using namespace swganh::messages::containers;

WaypointFactory::WaypointFactory(swganh::app::SwganhKernel* kernel)
    : IntangibleFactory(kernel)
{
    RegisterEventHandlers();
}
void WaypointFactory::RegisterEventHandlers()
{
	GetEventDispatcher()->Subscribe("PersistWaypoints", std::bind(&WaypointFactory::PersistHandler, this, std::placeholders::_1));
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
		if(object->IsDatabasePersisted())
			PersistObject(object);
	}
}

void WaypointFactory::LoadWaypoints(const shared_ptr<Player>& player, const shared_ptr<sql::ResultSet> result_set)
{
    while (result_set->next())
    {
        auto waypoint = make_shared<Waypoint>();
        waypoint->SetObjectId(result_set->getUInt64("id"));
        waypoint->SetCoordinates( glm::vec3(
            result_set->getDouble("x_position"),
            result_set->getDouble("y_position"), 
            result_set->getDouble("z_position"))
            );
        waypoint->SetLocationNetworkId(result_set->getUInt("scene_id"));
        string custom_string = result_set->getString("custom_name");
        waypoint->SetName(wstring(begin(custom_string), end(custom_string)));
        uint16_t activated = result_set->getUInt("active");
        activated == 0 ? waypoint->DeActivate() : waypoint->Activate();
        waypoint->SetColorByte(result_set->getUInt("color"));
            
        player->AddWaypoint(move(PlayerWaypointSerializer(waypoint)));
    }
}

uint32_t WaypointFactory::PersistObject(const shared_ptr<Object>& object, bool persist_inherited)
{
	uint32_t counter = 1;
	if (persist_inherited)
		IntangibleFactory::PersistObject(object, persist_inherited);
    if (object)
    {
        try 
        {			
            auto waypoint = static_pointer_cast<Waypoint>(object);
            auto conn = GetDatabaseManager()->getConnection("galaxy");
            auto statement = conn->prepareStatement("CALL sp_PersistWaypoint(?,?,?,?,?,?,?,?,?,?,?,?,?);");
            statement->setDouble(counter++,waypoint->GetComplexity());
            statement->setString(counter++, waypoint->GetStfNameFile());
            statement->setString(counter++, waypoint->GetStfNameString());

            auto custom_name = waypoint->GetCustomName();
            statement->setString(counter++, string(begin(custom_name), end(custom_name)));

            statement->setUInt(counter++, waypoint->GetVolume());

            auto coords = waypoint->GetCoordinates();
            statement->setDouble(counter++, coords.x);
            statement->setDouble(counter++, coords.y);
            statement->setDouble(counter++, coords.z);
            statement->setUInt(counter++, waypoint->GetActiveFlag());
            statement->setString(counter++, waypoint->GetPlanet());
            statement->setString(counter++, waypoint->GetNameStandard());
            statement->setString(counter++, waypoint->GetColor());
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

shared_ptr<Object> WaypointFactory::CreateObjectFromStorage(uint64_t object_id)
{
    auto waypoint = make_shared<Waypoint>();
    waypoint->SetObjectId(object_id);
    try{
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::Statement>(conn->createStatement());
        stringstream ss;
        ss << "CALL sp_GetWaypoint(" << object_id << ");";
        auto result = shared_ptr<sql::ResultSet>(statement->executeQuery(ss.str()));
        CreateBaseObjectFromStorage(waypoint, result);
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
			    uint16_t activated = result->getUInt("active");
                activated == 0 ? waypoint->DeActivate() : waypoint->Activate();
                waypoint->SetColor(result->getString("color"));
            }
        }

		//Clear us from the db persist update queue.
		boost::lock_guard<boost::mutex> lock(persisted_objects_mutex_);
		auto find_itr = persisted_objects_.find(waypoint);
		if(find_itr != persisted_objects_.end())
			persisted_objects_.erase(find_itr);
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return waypoint;
}

shared_ptr<Object> WaypointFactory::CreateObject()
{
	return make_shared<Waypoint>();
}
