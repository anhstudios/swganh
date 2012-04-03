
#include "swganh/object/waypoint/waypoint_factory.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "anh/logger.h"

#include "anh/database/database_manager.h"
#include "swganh/object/waypoint/waypoint.h"
#include "swganh/object/player/player_events.h"
#include "swganh/object/player/player.h"
#include "swganh/object/exception.h"
#include "swganh/simulation/simulation_service.h"
#include "swganh/messages/containers/network_map.h"

using namespace std;
using namespace anh;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::object::waypoint;
using namespace swganh::simulation;
using namespace swganh::messages::containers;

WaypointFactory::WaypointFactory(DatabaseManagerInterface* db_manager,
                             SimulationService* simulation_service, EventDispatcher* event_dispatcher)
    : ObjectFactory(db_manager, simulation_service, event_dispatcher)
{
    RegisterEventHandlers();
}
void WaypointFactory::RegisterEventHandlers()
{
    event_dispatcher_->Subscribe("PersistWaypoints", [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ValueEvent<NetworkMap<uint64_t, player::PlayerWaypointSerializer>>>(incoming_event);
        auto waypoints = value_event->Get();
        for(auto& waypoint_pair : waypoints)
        {
            PersistObject(waypoint_pair.second.waypoint);
        };
    });
    event_dispatcher_->Subscribe("LoadWaypoints", [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        auto waypoint_event = static_pointer_cast<player::WaypointEvent>(incoming_event);

    });
}

void WaypointFactory::LoadWaypoints(const shared_ptr<player::Player>& player, const shared_ptr<sql::ResultSet> result_set)
{
    while (result_set->next())
    {
        auto waypoint = make_shared<Waypoint>();
        waypoint->object_id_ = result_set->getUInt64("id");
        waypoint->coordinates_ = glm::vec3(
            result_set->getDouble("x_position"),
            result_set->getDouble("y_position"), 
            result_set->getDouble("z_position"));
        waypoint->location_network_id_ = result_set->getUInt("scene_id");
        string custom_string = result_set->getString("custom_name");
        waypoint->name_ = wstring(begin(custom_string), end(custom_string));
        waypoint->activated_flag_ = result_set->getUInt("is_active");
        waypoint->color_ = result_set->getUInt("color");
            
        player->AddWaypoint(move(player::PlayerWaypointSerializer(waypoint)));
    }
}

void WaypointFactory::LoadTemplates()
{
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetWaypointTemplates();");
        auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());

		while (result->next())
        {
            auto waypoint = make_shared<Waypoint>();
            // position orientation not used in waypoints
            
            waypoint->SetCoordinates(
                static_cast<int16_t>(result->getDouble("coord_x")),
                static_cast<int16_t>(result->getDouble("coord_y")),
                static_cast<int16_t>(result->getDouble("coord_z")));
            waypoint->activated_flag_ = result->getUInt("active");

            waypoint->planet_name_ = result->getString("planet");
            waypoint->color_ = result->getString("color");
            
            waypoint_templates_.insert(make_pair(waypoint->GetTemplate(), move(waypoint)));
        } while (statement->getMoreResults());
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

bool WaypointFactory::HasTemplate(const string& template_name)
{
    return GetTemplateIter_(template_name) != end(waypoint_templates_);
}
unordered_map<string,shared_ptr<Waypoint>>::iterator WaypointFactory::GetTemplateIter_(const string& template_name)
{
    auto iter = find_if(begin(waypoint_templates_), end(waypoint_templates_), [&template_name] (pair<string, shared_ptr<Waypoint>> template_pair) {
        return template_name == template_pair.first;
    });

    return iter;
}

void WaypointFactory::PersistObject(const shared_ptr<Object>& object)
{
    if (object->IsDirty() && object->GetType() ==  Waypoint::type)
    {
        try 
        {
            auto waypoint = static_pointer_cast<Waypoint>(object);
            auto conn = db_manager_->getConnection("galaxy");
            auto statement = conn->prepareStatement("CALL sp_PersistWaypoint(?,?,?,?,?,?,?,?,?,?,?,?,?);");
            statement->setDouble(1,waypoint->GetComplexity());
            statement->setString(2, waypoint->GetStfNameFile());
            statement->setString(3, waypoint->GetStfNameString());

            auto custom_name = waypoint->GetCustomName();
            statement->setString(4, string(begin(custom_name), end(custom_name)));

            statement->setUInt(5, waypoint->GetVolume());

            auto coords = waypoint->GetCoordinates();
            statement->setDouble(6, coords.x);
            statement->setDouble(7, coords.y);
            statement->setDouble(8, coords.z);
            statement->setUInt(9, waypoint->GetActiveFlag());
            statement->setString(10, waypoint->GetPlanet());
            statement->setString(11, waypoint->GetNameStandard());
            statement->setString(12, waypoint->GetColor());
            statement->execute();
        }
            catch(sql::SQLException &e)
        {
            LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
            LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
        }
    }
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
    waypoint->object_id_ = object_id;
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
			    waypoint->activated_flag_ = result->getUInt("is_active");
                waypoint->color_ = result->getString("color");
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

shared_ptr<Object> WaypointFactory::CreateObjectFromTemplate(const string& template_name)
{
    auto object = make_shared<Waypoint>();
    auto found = GetTemplateIter_(template_name);
    if (found != end(waypoint_templates_))
    {
        object = found->second;
    }
    else
    {
        throw swganh::object::InvalidObjectTemplate("Template Not Found: " + template_name);
    }
    
    return object;
}
