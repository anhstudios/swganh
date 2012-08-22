// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "attributes_service.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include "anh/logger.h"

#include "anh/service/service_manager.h"
#include "anh/database/database_manager.h"

#include "swganh/app/swganh_kernel.h"

using swganh::app::SwganhKernel;
using namespace swganh::app;
using namespace swganh_core::attributes;


using namespace sql;
using namespace std;
using namespace anh::database;

AttributesService::AttributesService(SwganhKernel* kernel)
    : kernel_(kernel)
{
}
void AttributesService::Startup()
{
}

AttributesService::~AttributesService()
{    
	attribute_templates_.clear();
}

std::shared_ptr<swganh::attributes::AttributeTemplateInterface> AttributesService::GetAttributeTemplate(const std::string& name)
{
	auto found = find_if(begin(attribute_templates_), end(attribute_templates_), [&name](AttributeTemplates::value_type entry)
	{
		return entry.first == name;
	});
	if (found != attribute_templates_.end())
	{
		return found->second;
	}
	LOG(warning) << "Attribute template not found with name " << name;
	return nullptr;
}
void AttributesService::SetAttributeTemplate(const std::shared_ptr<swganh::attributes::AttributeTemplateInterface> template_, const std::string& name)
{
	auto found = find_if(begin(attribute_templates_), end(attribute_templates_), [&name](AttributeTemplates::value_type entry)
	{
		return entry.first == name;
	});
	if (found == end(attribute_templates_))
	{
		attribute_templates_[name] = template_;
	}
	else
	{
		LOG(warning) << "Error Attribute Template already exists for template name " << name;
	}
}

void AttributesService::SendAttributesMessage(const std::shared_ptr<swganh::object::Object> object)
{

}

//void AttributesService::LoadAttributes_()
//{
//	try {
//
//	auto db_manager = kernel_->GetDatabaseManager();
//	auto static_db = db_manager->getConnection("swganh_static");
//	auto statement = unique_ptr<PreparedStatement>(
//		static_db->prepareStatement("CALL sp_GetAttributes()"));
//
//	auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery());
//	// TODO: Get Data Here
//	}
//	catch(SQLException &e)
//    {
//        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
//        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
//    }
//}

