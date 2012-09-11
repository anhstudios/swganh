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
#include "swganh/simulation/simulation_service_interface.h"
#include "swganh_core/messages/containers/network_map.h"

using namespace std;
using namespace swganh;
using namespace swganh::database;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::simulation;
using namespace swganh::messages::containers;

WaypointFactory::WaypointFactory(DatabaseManagerInterface* db_manager,
                                 EventDispatcher* event_dispatcher)
    : IntangibleFactory(db_manager, event_dispatcher)
{
    RegisterEventHandlers();
}
void WaypointFactory::RegisterEventHandlers()
{
    event_dispatcher_->Subscribe("PersistWaypoints", [this] (shared_ptr<swganh::EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ValueEvent<NetworkMap<uint64_t, PlayerWaypointSerializer>>>(incoming_event);
        auto waypoints = value_event->Get();
        for(auto& waypoint_pair : waypoints)
        {
            PersistObject(waypoint_pair.second.waypoint);
        };
    });
    event_dispatcher_->Subscribe("LoadWaypoints", [this] (shared_ptr<swganh::EventInterface> incoming_event)
    {
        auto waypoint_event = static_pointer_cast<WaypointEvent>(incoming_event);

    });
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

uint32_t WaypointFactory::PersistObject(const shared_ptr<Object>& object)
{
	uint32_t counter = 1;
    if (object->IsDirty() && object->GetType() ==  Waypoint::type)
    {
        try 
        {			
            auto waypoint = static_pointer_cast<Waypoint>(object);
            auto conn = db_manager_->getConnection("galaxy");
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
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_DeleteWaypoint(?);");
        statement->setUInt64(1, object->GetObjectId());
        statement->execute();
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

shared_ptr<Object> WaypointFactory::CreateObjectFromStorage(uint64_t object_id)
{
    auto waypoint = make_shared<Waypoint>();
    waypoint->SetObjectId(object_id);
    try{
        auto conn = db_manager_->getConnection("galaxy");
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
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return waypoint;
}

shared_ptr<Object> WaypointFactory::CreateObjectFromTemplate(const string& template_name, bool db_persisted, bool db_initialized)
{
	//@TODO: Create me with help from db
	return make_shared<Waypoint>();
}
