
#include "waypoint_service.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/database/database_manager.h"
#include "swganh/service/service_manager.h"
#include "swganh_core/simulation/simulation_service_interface.h"

using namespace swganh::waypoint;
using swganh::object::Object;
using swganh::object::Waypoint;

WaypointService::WaypointService(swganh::app::SwganhKernel* kernel)
    : kernel_(kernel) {}

WaypointService::~WaypointService()
{}

std::shared_ptr<Waypoint> WaypointService::CreateWaypointInStorage(
    const std::shared_ptr<Object>& owner,
    const std::string& name,
    const glm::vec3& coordinates,
    const std::string& planet,
    Waypoint::WaypointColor color,
    bool activated)
{
    std::shared_ptr<Waypoint> waypoint;

    try
    {
        auto connection = kernel_->GetDatabaseManager()->getConnection("galaxy");
        auto statement = connection->prepareStatement("SELECT sf_WaypointCreate(?, ?, ?, ?, ?, ?, ?, ?)");
        
        statement->setUInt64(1, owner->GetObjectId());
        statement->setDouble(2, coordinates.x);
        statement->setDouble(3, coordinates.y);
        statement->setDouble(4, coordinates.z);
        statement->setUInt(5, activated);
        statement->setString(6, planet);
        statement->setString(7, name);
        statement->setUInt(8, color);
        
        auto result = std::shared_ptr<sql::ResultSet>(statement->executeQuery());
        
        if (result->next())
        {
            waypoint = std::make_shared<Waypoint>();
            waypoint->SetObjectId(result->getUInt64(1));
            waypoint->SetCoordinates(coordinates);
            waypoint->SetPlanet(planet);
            waypoint->SetName(std::wstring(std::begin(name), std::end(name)));
            waypoint->SetColor(color);
            (activated) ? waypoint->Activate() : waypoint->DeActivate();
        }
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return waypoint;
}
