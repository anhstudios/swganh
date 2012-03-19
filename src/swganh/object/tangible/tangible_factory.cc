
#include "swganh/object/tangible/tangible_factory.h"
#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "anh/logger.h"

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

 TangibleFactory::TangibleFactory(
    DatabaseManagerInterface* db_manager,
    SimulationService* simulation_service)
    : ObjectFactory(db_manager, simulation_service)
{
}
void TangibleFactory::LoadTemplates()
{
    try {

        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetTangibleTemplates();");
        auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());

        while (result->next())
        {
            auto tangible = make_shared<Tangible>();
            
            
            tangible_templates_.insert(make_pair(tangible->GetTemplate(), move(tangible)));
        } while (statement->getMoreResults());
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
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
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>
            (conn->prepareStatement("CALL sp_PersistTangible(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);"));
        ObjectFactory::PersistObject(object, statement);
        // cast to tangible
        auto tangible = static_pointer_cast<Tangible>(object);
        statement->setString(17, tangible->customization_);
        statement->setInt(18, tangible->options_bitmask_);
        statement->setInt(19, tangible->incap_timer_);
        statement->setInt(20, tangible->condition_damage_);
        statement->setInt(21, tangible->max_condition_);
        statement->setBoolean(22, tangible->is_static_);
        statement->executeUpdate();
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
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
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void TangibleFactory::CreateTangible(const shared_ptr<Tangible>& tangible, const std::shared_ptr<sql::Statement>& statement)
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
                tangible->SetStatic(result->getBoolean("is_static"));
            }
        }
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
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
        CreateTangible(tangible, statement);  
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
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
