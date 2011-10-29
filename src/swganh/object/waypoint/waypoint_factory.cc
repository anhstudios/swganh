
#include "swganh/object/waypoint/waypoint_factory.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <glog/logging.h>

#include "anh/database/database_manager.h"
#include "swganh/object/waypoint/waypoint.h"
#include "swganh/object/exception.h"
#include "swganh/simulation/simulation_service.h"

using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::object::waypoint;
using namespace swganh::simulation;

WaypointFactory::WaypointFactory(const shared_ptr<DatabaseManagerInterface>& db_manager,
                             SimulationService* simulation_service)
    : ObjectFactory(db_manager, simulation_service)
{
}

void WaypointFactory::LoadTemplates()
{
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetWaypointTemplates();");
        auto result = statement->executeQuery();

        while (result->next())
        {
            auto waypoint = make_shared<Waypoint>();
            // position orientation not used in waypoints
            waypoint->SetComplexity(result->getDouble(1));
            waypoint->SetStfNameFile(result->getString(2));
            waypoint->SetStfNameString(result->getString(3));
            string custom_string = result->getString(4);
            waypoint->SetCustomName(wstring(begin(custom_string), end(custom_string)));
            waypoint->SetVolume(result->getUInt(5));
            waypoint->SetCoordinates(result->getDouble(6),result->getDouble(7),result->getDouble(8));
            bool active = result->getUInt(9) == 1;
            if (active)
                waypoint->Activate();
            else
                waypoint->DeActivate();

            waypoint->SetPlanet(result->getString(10));
            string name = result->getString(11);
            waypoint->SetName(wstring(begin(name), end(name)));
            waypoint->SetColor(result->getString(12));
            waypoint->SetTemplate(result->getString(13));

            waypoint_templates_.insert(make_pair(waypoint->GetTemplate(), move(waypoint)));
        } while (statement->getMoreResults());
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
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
            statement->setString(4, waypoint->GetCustomNameStandard());
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
            DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
            DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
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
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

shared_ptr<Object> WaypointFactory::CreateObjectFromStorage(uint64_t object_id)
{
    auto waypoint = make_shared<Waypoint>();
    waypoint->SetObjectId(object_id);
    try{
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetWaypoint(?);");
        statement->setUInt64(1, object_id);
        auto result = statement->executeQuery();
        while (result->next())
        {
            bool active = result->getUInt("is_active") == 1;
            if (active)
                waypoint->Activate();
            else
                waypoint->DeActivate();

            string name = result->getString("name");
            waypoint->SetName(wstring(begin(name), end(name)));
            waypoint->SetColor(result->getString("color"));
        }
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
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
