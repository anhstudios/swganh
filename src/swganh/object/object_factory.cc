#include "object_factory.h"


#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <glog/logging.h>

#include "anh/database/database_manager.h"
#include "swganh/object/object.h"
#include "swganh/object/exception.h"
#include "swganh/simulation/simulation_service.h"

using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::simulation;

ObjectFactory::ObjectFactory(const shared_ptr<DatabaseManagerInterface>& db_manager,
                             SimulationService* simulation_service)
    : db_manager_(db_manager)
    , simulation_service_(simulation_service)
{
}

void ObjectFactory::CreateBaseObjectFromStorage(const shared_ptr<Object>& object, const shared_ptr<sql::ResultSet>& result)
{
    try {
        result->next();
        object->SetSceneId(result->getUInt("scene_id"));
        object->SetPosition(glm::vec3(result->getDouble("x_position"),result->getDouble("y_position"), result->getDouble("z_position")));
        object->SetOrientation(glm::quat(result->getDouble("x_orientation"),result->getDouble("y_orientation"), result->getDouble("z_orientation"), result->getDouble("w_orientation")));
        object->SetComplexity(result->getDouble("complexity"));
        object->SetStfNameFile(result->getString("stf_name_file"));
        object->SetStfNameString(result->getString("stf_name_string"));
        string custom_string = result->getString("custom_name");
        object->SetCustomName(wstring(begin(custom_string), end(custom_string)));
        object->SetVolume(result->getUInt("volume"));
        object->SetTemplate(result->getString("iff_template"));
        
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

uint32_t ObjectFactory::LookupType(uint64_t object_id) const
{
    uint32_t type = 0;
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetType(?);");
        statement->setUInt64(1, object_id);
        auto result = statement->executeQuery();        
        while (result->next())
        {
            type = result->getUInt("description");
        }
        return type;
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
        return type;
    }
}