
#include "swganh/object/intangible/intangible_factory.h"

#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <glog/logging.h>

#include "anh/database/database_manager.h"
#include "swganh/object/intangible/intangible.h"
#include "swganh/object/exception.h"
#include "swganh/simulation/simulation_service.h"

using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::object::intangible;
using namespace swganh::simulation;

IntangibleFactory::IntangibleFactory(const shared_ptr<DatabaseManagerInterface>& db_manager,
                             SimulationService* simulation_service)
    : ObjectFactory(db_manager, simulation_service)
{
}
void IntangibleFactory::LoadTemplates()
{
    try {

        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetIntangibleTemplates();");
        auto result = statement->executeQuery();

        while (result->next())
        {
            auto intangible = make_shared<Intangible>();
            intangible->SetPosition(glm::vec3(result->getDouble(1),result->getDouble(2), result->getDouble(3)));
            intangible->SetOrientation(glm::quat(result->getDouble(4),result->getDouble(5), result->getDouble(6), result->getDouble(7)));
            intangible->SetComplexity(result->getDouble(8));
            intangible->SetStfNameFile(result->getString(9));
            intangible->SetStfNameString(result->getString(10));
            string custom_string = result->getString(11);
            intangible->SetCustomName(wstring(begin(custom_string), end(custom_string)));
            intangible->SetVolume(result->getUInt(12));
            intangible->SetStfDetailFile(result->getString(13));
            intangible->SetStfDetailString(result->getString(14));
            intangible->SetTemplate(result->getString(15));
            
            intangible_templates_.insert(make_pair(intangible->GetTemplate(), move(intangible)));
        } while (statement->getMoreResults());
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
unordered_map<string, shared_ptr<Intangible>>::iterator IntangibleFactory::GetTemplateIter_(const string& template_name)
{
    auto found = find_if(begin(intangible_templates_), end(intangible_templates_), [&template_name] (pair<string, shared_ptr<Intangible>> template_pair) {
        return template_name == template_pair.first;
    });
    return found;
}
bool IntangibleFactory::HasTemplate(const string& template_name)
{
    return GetTemplateIter_(template_name) != end(intangible_templates_);
}

void IntangibleFactory::PersistObject(const shared_ptr<Object>& object)
{
    if (object->IsDirty())
    {
        try 
        {
            auto conn = db_manager_->getConnection("galaxy");
            auto statement = conn->prepareStatement("CALL sp_PersistIntangible(?,?,?,?,?,?,?,?);");
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

void IntangibleFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_DeleteIntangible(?);");
        statement->setUInt64(1, object->GetObjectId());
        statement->execute();
    }
        catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

shared_ptr<Object> IntangibleFactory::CreateObjectFromStorage(uint64_t object_id)
{
    auto intangible = make_shared<Intangible>();
    intangible->SetObjectId(object_id);
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::Statement>(conn->createStatement());
        
        stringstream ss;
        ss << "CALL sp_GetIntangible(" << object_id << ");";

        auto result = shared_ptr<sql::ResultSet>(statement->executeQuery(ss.str()));
        CreateBaseObjectFromStorage(intangible, result);
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
                intangible->SetStfDetailFile(result->getString("stf_detail_file"));
                intangible->SetStfDetailString(result->getString("stf_detail_string"));
            }
        }
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return intangible;
}

shared_ptr<Object> IntangibleFactory::CreateObjectFromTemplate(const string& template_name)
{
    auto object = make_shared<Intangible>();
    auto found = GetTemplateIter_(template_name);
    if (found != end(intangible_templates_))
    {
        object = found->second;
    }
    else
    {
        throw swganh::object::InvalidObjectTemplate("Template Not Found: " + template_name);
    }
    
    return object;
}
