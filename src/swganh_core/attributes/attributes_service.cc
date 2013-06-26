// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

#include "attributes_service.h"

#include <boost/python.hpp>

#include "swganh/scripting/utilities.h"
//
#include "attributes_get_batch_command.h"

#include "swganh/logger.h"

#include "swganh/service/service_manager.h"
#include "swganh/database/database_manager.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/object/creature/creature.h"
#include "swganh_core/connection/connection_service_interface.h"
#include "swganh_core/connection/connection_client_interface.h"

#include "swganh_core/simulation/simulation_service_interface.h"
#include "swganh_core/command/command_service_interface.h"

#include "swganh_core/connection/connection_client.h"
#include "swganh_core/messages/attribute_list_message.h"

#include "swganh/scripting/python_script.h"


namespace bp = boost::python;
using swganh::scripting::ScopedGilLock;
using swganh::app::SwganhKernel;
using namespace swganh::app;
using namespace swganh::attributes;
using namespace swganh::connection;
using namespace swganh::database;
using namespace sql;
using namespace std;

const std::string python_init = "AttributeTemplateInit.py";

AttributesService::AttributesService(SwganhKernel* kernel)
    : kernel_(kernel)	
{
    SetServiceDescription(swganh::service::ServiceDescription(
        "Attributes Service",
        "attributes",
        "0.1",
        "127.0.0.1",
        0,
        0,
        0));
}

AttributesService::~AttributesService()
{}

void AttributesService::Initialize()
{
	simulation_service_ = kernel_->GetServiceManager()->GetService<swganh::simulation::SimulationServiceInterface>("SimulationService");
}

void AttributesService::Startup()
{
	auto command_service = kernel_->GetServiceManager()->GetService<swganh::command::CommandServiceInterface>("CommandService");
    command_service->AddCommandCreator<GetAttributesBatchCommand>("getattributesbatch");	

	LoadAttributeTemplates_();
}

bool AttributesService::HasAttributeTemplate(int8_t template_id)
{
// if in debug we want to reload the template each time so it's not stale...
#ifdef _DEBUG
	return python_attribute_templates_.size() >= static_cast<uint16_t>(template_id);
#endif
	auto found = find_if(begin(attribute_templates_), end(attribute_templates_), [&template_id](AttributeTemplateMap::value_type entry)
	{
		return entry.first == template_id;
	});
	if (found != attribute_templates_.end())
	{
		return true;
	}
	return false;
}

std::shared_ptr<swganh::attributes::AttributeTemplateInterface> AttributesService::GetAttributeTemplate(int8_t template_id)
{
	if (HasAttributeTemplate(template_id))
		return attribute_templates_[template_id];
	DLOG(warning) << "Attribute template not found with id " << template_id;
	return nullptr;
}
void AttributesService::SetAttributeTemplate(const std::shared_ptr<swganh::attributes::AttributeTemplateInterface> template_, int8_t template_id)
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
	if (HasAttributeTemplate(object->GetAttributeTemplateId()))
	{
		swganh::messages::AttributeListMessage message;
// if in debug we want to reload the template each time so it's not stale...
#ifdef _DEBUG
		auto debug_template = GetPythonAttributeTemplate(python_attribute_templates_[object->GetAttributeTemplateId()]);
		message = debug_template->BuildAttributeTemplate(object);
#else
		message = attribute_templates_[object->GetAttributeTemplateId()]->BuildAttributeTemplate(object);
#endif
		
		// Append Pups
		// Append Slicing
		actor->NotifyObservers(&message);
	}
	else
	{
		// Send a message to the client saying this object doesn't have an attribute template id
		swganh::messages::AttributeListMessage attribute_list_message;
	
		std::vector<swganh::messages::Attribute> attribute_list;
		attribute_list_message.object_id = object->GetObjectId();
		std::string template_str = object->GetTemplate();
		std::wstring val = std::wstring(template_str.begin(), template_str.end());
		std::wstring error = L"Object " + val + L" doesn't have attribute_template_id value, database needs updating...";
		attribute_list.push_back(swganh::messages::Attribute("Error:", error));
		attribute_list_message.attributes = attribute_list;
		actor->NotifyObservers(&attribute_list_message);
	}
}
void AttributesService::LoadAttributeTemplates_()
{
    swganh::scripting::PythonScript script(kernel_->GetAppConfig().script_directory + "/attributes/AttributeTemplateInit.py");

    python_attribute_templates_ = script.GetGlobalAs<std::vector<std::string>>("attributeTemplates");

	// If Not debug load these up here and not again
#ifndef _DEBUG
	int template_id = 0;
	for (auto& attr_template :  python_attribute_templates_)
	{
		attribute_templates_[template_id++] = GetPythonAttributeTemplate(attr_template);
	}
#endif
}
std::shared_ptr<AttributeTemplateInterface> AttributesService::GetPythonAttributeTemplate(std::string filename)
{
    swganh::scripting::PythonScript script(kernel_->GetAppConfig().script_directory + "/attributes/" + filename + ".py");

    auto attribute_template_creator = script.CreateInstance<AttributeTemplateInterface>(filename);
    attribute_template_creator->SetKernel(kernel_);

	return attribute_template_creator;
}
