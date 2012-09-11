// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "attributes_service.h"
// Templates
#include "armor_attribute_template.h"
#include "crafting_tool_attribute_template.h"
#include "deed_attribute_template.h"
#include "droid_attribute_template.h"
#include "factory_crate_attribute_template.h"
#include "food_attribute_template.h"
#include "furniture_attribute_template.h"
#include "meds_attribute_template.h"
#include "ship_attribute_template.h"
#include "weapon_attribute_template.h"
#include "wearable_attribute_template.h"
//
#include "attributes_get_batch_command.h"

#include "swganh/logger.h"

#include "swganh/service/service_manager.h"
#include "swganh/database/database_manager.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/object/creature/creature.h"
#include "swganh/connection/connection_service_interface.h"
#include "swganh/connection/connection_client_interface.h"

#include "swganh/simulation/simulation_service_interface.h"
#include "swganh/command/command_service_interface.h"

#include "swganh_core/connection/connection_client.h"
#include "swganh_core/messages/attribute_list_message.h"


using swganh::app::SwganhKernel;
using namespace swganh::app;
using namespace swganh::attributes;
using namespace swganh::connection;
using namespace swganh::attributes;
using namespace sql;
using namespace std;
using namespace swganh::database;

AttributesService::AttributesService(SwganhKernel* kernel)
    : kernel_(kernel)
{
}

swganh::service::ServiceDescription AttributesService::GetServiceDescription()
{
	swganh::service::ServiceDescription service_description(
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
	auto command_service = kernel_->GetServiceManager()->GetService<swganh::command::CommandServiceInterface>("CommandService");

    command_service->AddCommandCreator("getattributesbatch", [] (
        swganh::app::SwganhKernel* kernel,
        const swganh::command::CommandProperties& properties)
    {
        return std::make_shared<GetAttributesBatchCommand>(kernel, properties);
    });	

	LoadAttributeTemplates_();
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
	DLOG(warning) << "Attribute template not found with id " << template_id;
	return nullptr;
}
void AttributesService::SetAttributeTemplate(const std::shared_ptr<swganh::attributes::AttributeTemplateInterface> template_, swganh::attributes::AttributeTemplateId template_id)
{
	if (!HasAttributeTemplate(template_id))
	{
		attribute_templates_[template_id] = template_;
	}
	else
	{
		DLOG(warning) << "Error Attribute Template already exists for template id " << template_id;
	}
}

void AttributesService::SendAttributesMessage(const std::shared_ptr<swganh::object::Object> object, const std::shared_ptr<swganh::object::Object> actor)
{
	AttributeTemplateId template_id = static_cast<AttributeTemplateId>(object->GetAttributeTemplateId());
	if (HasAttributeTemplate(template_id))
	{
		auto message = attribute_templates_[template_id]->BuildAttributeTemplate(object);
		// Append Pups
		// Append Slicing
		actor->NotifyObservers(&message);
	}
}
void AttributesService::LoadAttributeTemplates_()
{
	SetAttributeTemplate(make_shared<ArmorAttributeTemplate>(kernel_->GetEventDispatcher()), ARMOR);
	SetAttributeTemplate(make_shared<CraftingToolAttributeTemplate>(kernel_->GetEventDispatcher()), CRAFTING_TOOL);
	SetAttributeTemplate(make_shared<DeedAttributeTemplate>(kernel_->GetEventDispatcher()), DEED);
	SetAttributeTemplate(make_shared<DroidAttributeTemplate>(kernel_->GetEventDispatcher()), DROID);
	SetAttributeTemplate(make_shared<FactoryCrateAttributeTemplate>(kernel_->GetEventDispatcher()), FACTORY_CRATE);
	SetAttributeTemplate(make_shared<FoodAttributeTemplate>(kernel_->GetEventDispatcher()), FOOD);
	SetAttributeTemplate(make_shared<FurnitureAttributeTemplate>(kernel_->GetEventDispatcher()), FURNITURE);
	SetAttributeTemplate(make_shared<MedsAttributeTemplate>(kernel_->GetEventDispatcher()), MEDS);
	SetAttributeTemplate(make_shared<ShipAttributeTemplate>(kernel_->GetEventDispatcher()), SHIP);
	SetAttributeTemplate(make_shared<WeaponAttributeTemplate>(kernel_->GetEventDispatcher()), WEAPON);
	SetAttributeTemplate(make_shared<WearableAttributeTemplate>(kernel_->GetEventDispatcher()), WEARABLE);
}

void AttributesService::HandleGetAttributesBatch(const std::shared_ptr<swganh::object::Object> object, const std::shared_ptr<swganh::object::Object> actor)
{
	SendAttributesMessage(object, actor);
}

