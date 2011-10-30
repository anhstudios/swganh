
#include "swganh/object/tangible/tangible_factory.h"
#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <glog/logging.h>

#include "anh/database/database_manager.h"
#include "swganh/object/tangible/tangible.h"
#include "swganh/object/exception.h"
#include "swganh/simulation/simulation_service.h"

using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::object::tangible;
using namespace swganh::simulation;

uint32_t TangibleFactory::GetType() const { return Tangible::type; }

 TangibleFactory::TangibleFactory(const shared_ptr<DatabaseManagerInterface>& db_manager,
                             SimulationService* simulation_service)
    : ObjectFactory(db_manager, simulation_service)
{
}
void TangibleFactory::LoadTemplates()
{
    try {

        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetTangibleTemplates();");
        auto result = statement->executeQuery();

        while (result->next())
        {
            auto tangible = make_shared<Tangible>();
            tangible->SetPosition(glm::vec3(result->getDouble(1),result->getDouble(2), result->getDouble(3)));
            tangible->SetOrientation(glm::quat(result->getDouble(4),result->getDouble(5), result->getDouble(6), result->getDouble(7)));
            tangible->SetComplexity(result->getDouble(8));
            tangible->SetStfNameFile(result->getString(9));
            tangible->SetStfNameString(result->getString(10));
            string custom_string = result->getString(11);
            tangible->SetCustomName(wstring(begin(custom_string), end(custom_string)));
            tangible->SetVolume(result->getUInt(12));
            tangible->SetCustomization(result->getString(13));
            tangible->SetOptionsMask(result->getUInt(14));
            tangible->SetIncapTimer(result->getUInt(15));
            tangible->SetConditionDamage(result->getUInt(16));
            uint8_t is_static = result->getInt(17);
            tangible->SetStatic(is_static == 1);
            tangible->SetTemplate(result->getString(18));
            
            tangible_templates_.insert(make_pair(tangible->GetTemplate(), move(tangible)));
        } while (statement->getMoreResults());
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

bool TangibleFactory::HasTemplate(const string& template_name)
{
    auto found = GetTemplateIter_(template_name);
    return found != end(tangible_templates_);
}
unordered_map<string, shared_ptr<Tangible>>::iterator TangibleFactory::GetTemplateIter_(const string& template_name)
{
    auto found = find_if(begin(tangible_templates_), end(tangible_templates_), [&template_name] (pair<string, shared_ptr<Tangible>> template_pair) {
        return template_name == template_pair.first;
    });
    return found;
}
void TangibleFactory::PersistObject(const shared_ptr<Object>& object)
{
    if (object->IsDirty())
    {
        try 
        {
            auto conn = db_manager_->getConnection("galaxy");
            auto statement = conn->prepareStatement("CALL sp_PersistTangible(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);");
            // TODO: Add in values to persist
            statement->execute();
        }
            catch(sql::SQLException &e)
        {
            DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
            DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
        }
    }
}

void TangibleFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_DeleteTangible(?);");
        statement->setUInt64(1, object->GetObjectId());
        statement->execute();
    }
        catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void TangibleFactory::CreateBaseTangible(const shared_ptr<BaseTangible>& tangible, const std::shared_ptr<sql::Statement>& statement)
{
    try {
        auto result = shared_ptr<sql::ResultSet>(statement->getResultSet());
        CreateBaseObjectFromStorage(tangible, result);
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
                tangible->SetCustomization(result->getString("customization"));
                tangible->SetOptionsMask(result->getUInt("options_bitmask"));
                tangible->SetIncapTimer(result->getUInt("incap_timer"));
                tangible->SetConditionDamage(result->getUInt("condition_damage"));
                tangible->SetMaxCondition(result->getUInt("max_condition"));
                uint8_t is_static = result->getInt("is_moveable");
                tangible->SetStatic(is_static == 0);
            }
        }
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
shared_ptr<Object> TangibleFactory::CreateObjectFromStorage(uint64_t object_id)
{
    auto tangible = make_shared<Tangible>();
    tangible->SetObjectId(object_id);
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::Statement>(conn->createStatement());
        
        stringstream ss;
        ss << "CALL sp_GetTangible(" << object_id << ");";   

        statement->execute(ss.str());
        CreateBaseTangible(tangible, statement);  
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return tangible;
}

shared_ptr<Object> TangibleFactory::CreateObjectFromTemplate(const string& template_name)
{
    auto object = make_shared<Tangible>();
    auto found = GetTemplateIter_(template_name);
    if (found != end(tangible_templates_))
    {
        object = found->second;
    }
    else
    {
        throw swganh::object::InvalidObjectTemplate("Template Not Found: " + template_name);
    }
    
    return object;
}
