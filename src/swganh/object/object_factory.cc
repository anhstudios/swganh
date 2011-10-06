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

using namespace std;
using namespace anh::database;
using namespace swganh::object;

ObjectFactory::ObjectFactory(const shared_ptr<DatabaseManagerInterface>& db_manager)
    : db_manager_(db_manager)
{
}

void ObjectFactory::CreateBaseObjectFromStorage(const std::shared_ptr<Object>& object)
{
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetObject(?);");
        statement->setUInt64(1, object->GetObjectId());
        auto result = shared_ptr<sql::ResultSet>(statement->executeQuery());
        while (result->next())
        {
            object->SetPosition(glm::vec3(result->getDouble("x_position"),result->getDouble("y_position"), result->getDouble("z_position")));
            object->SetOrientation(glm::quat(result->getDouble("x_orientation"),result->getDouble("y_orientation"), result->getDouble("z_orientation"), result->getDouble("w_orientation")));
            object->SetComplexity(result->getDouble("complexity"));
            object->SetStfNameFile(result->getString("stf_name_file"));
            object->SetStfNameString(result->getString("stf_name_string"));
            string custom_string = result->getString("custom_name");
            object->SetCustomName(wstring(begin(custom_string), end(custom_string)));
            object->SetVolume(result->getUInt("volume"));
            object->SetTemplate(result->getString("discr"));
        }
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}