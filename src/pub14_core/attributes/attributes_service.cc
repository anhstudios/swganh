// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "attributes_service.h"
#include "armor_attribute_template.h"

#include "anh/logger.h"

#include "anh/service/service_manager.h"
#include "anh/database/database_manager.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/object/object.h"
#include "swganh/connection/connection_service_interface.h"
#include "swganh/connection/connection_client_interface.h"

#include "swganh/simulation/simulation_service_interface.h"

#include "pub14_core/connection/connection_client.h"
#include "pub14_core/messages/attribute_list_message.h"

using swganh::app::SwganhKernel;
using namespace swganh::app;
using namespace swganh_core::attributes;
using namespace swganh_core::connection;
using namespace swganh::attributes;


using namespace sql;
using namespace std;
using namespace anh::database;

AttributesService::AttributesService(SwganhKernel* kernel)
    : kernel_(kernel)
{
}

anh::service::ServiceDescription AttributesService::GetServiceDescription()
{
	anh::service::ServiceDescription service_description(
        "Attributes Service",
        "attributes",
        "0.1",
        "127.0.0.1",
        0,
        0,
        0);

    return service_description;
}
void AttributesService::Startup()
{
	simulation_service_ = kernel_->GetServiceManager()->GetService<swganh::simulation::SimulationServiceInterface>("SimulationService");
	simulation_service_->RegisterControllerHandler(&AttributesService::HandleGetAttributesBatch_, this);
}

AttributesService::~AttributesService()
{    
	attribute_templates_.clear();
}

bool AttributesService::HasAttributeTemplate(AttributeTemplateId template_id)
{
	auto found = find_if(begin(attribute_templates_), end(attribute_templates_), [&template_id](AttributeTemplates::value_type entry)
	{
		return entry.first == template_id;
	});
	if (found != attribute_templates_.end())
	{
		return true;
	}
	return false;
}

std::shared_ptr<swganh::attributes::AttributeTemplateInterface> AttributesService::GetAttributeTemplate(swganh::attributes::AttributeTemplateId template_id)
{
	if (HasAttributeTemplate(template_id))
		return attribute_templates_[template_id];
	LOG(warning) << "Attribute template not found with id " << template_id;
	return nullptr;
}
void AttributesService::SetAttributeTemplate(const std::shared_ptr<swganh::attributes::AttributeTemplateInterface> template_, swganh::attributes::AttributeTemplateId template_id)
{
	if (HasAttributeTemplate(template_id))
	{
		attribute_templates_[template_id] = template_;
	}
	else
	{
		LOG(warning) << "Error Attribute Template already exists for template id " << template_id;
	}
}

void AttributesService::SendAttributesMessage(const std::shared_ptr<swganh::object::Object> object)
{
	AttributeTemplateId template_id = static_cast<AttributeTemplateId>(object->GetAttributeTemplateId());
	if (HasAttributeTemplate(template_id))
	{
		auto message = attribute_templates_[template_id]->BuildAttributeTemplate(object);
		// Append Pups
		// Append Slicing
		object->NotifyObservers(message);
	}
}
void AttributesService::LoadAttributeTemplates_()
{
	SetAttributeTemplate(make_shared<ArmorAttributeTemplate>(kernel_->GetEventDispatcher()), ARMOR);
}

void AttributesService::HandleGetAttributesBatch_(
	const shared_ptr<swganh::object::ObjectController>& controller,
	swganh::messages::controllers::GetAttributesBatchMessage message)
{
	SendAttributesMessage(controller->GetObject());
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

