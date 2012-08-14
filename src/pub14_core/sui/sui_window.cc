#include <anh/logger.h>

#include "sui_window.h"
#include <string.h>

using namespace swganh::object;
using namespace swganh::sui;
using namespace swganh_core::sui;

SUIWindow::SUIWindow(std::string script_name, std::shared_ptr<Object> owner, std::shared_ptr<Object> ranged_object, float max_distance)
	: SUIWindowInterface(script_name, owner, ranged_object, max_distance)
{
}

std::shared_ptr<SUIWindowInterface> SUIWindow::ClearDataSource(std::string name)
{
	SUI_WINDOW_COMPONENT component;
	component.type = 1;
	
	//Add in narrows
	component.narrow_params.push_back(name);

	//No wides to add

	components_.push_back(component);

	return shared_from_this();
}

std::shared_ptr<SUIWindowInterface> SUIWindow::SetProperty(std::string location_string, std::wstring value)
{
	SUI_WINDOW_COMPONENT component;
	component.type = 3;

	//Add in narrows
	char *dup = _strdup(location_string.c_str());
	component.narrow_params.push_back(strtok(dup, ":"));
	component.narrow_params.push_back(strtok(nullptr, ":"));
	strtok(nullptr, ":");
	free(dup);

	//Add in wides
	component.wide_params.push_back(value);

	components_.push_back(component);

	return shared_from_this();
}

std::shared_ptr<SUIWindowInterface> SUIWindow::AddProperty(std::string location_string, std::wstring value)
{
	SUI_WINDOW_COMPONENT component;
	component.type = 4;

	//Add in narrows
	char *dup = _strdup(location_string.c_str());
	component.narrow_params.push_back(strtok(dup, ":"));
	component.narrow_params.push_back(strtok(nullptr, ":"));
	strtok(nullptr, ":");
	free(dup);

	//Add in wides
	component.wide_params.push_back(value);

	components_.push_back(component);

	return shared_from_this();
}

std::shared_ptr<SUIWindowInterface> SUIWindow::SubscribeToEventCallback(
	uint32_t event_id,
	std::string event_source,
	InputTrigger trigger, 
	std::vector<std::string> returned_properties, 
	WindowCallbackFunction callbackFunction)
{
	SUI_WINDOW_COMPONENT component;
	component.type = 5;

	//Add narrows
	component.narrow_params.push_back(event_source);
	component.narrow_params.push_back(std::string((char*)trigger, 1));
	component.narrow_params.push_back("STRING_NOT_USED");

	for(auto& returned : returned_properties)
	{
		char *dup = _strdup(returned.c_str());
		component.narrow_params.push_back(strtok(dup, ":"));
		component.narrow_params.push_back(strtok(nullptr, ":"));
		strtok(nullptr, ":");
		free(dup);
	}

	//No wides to add
	
	//Add callback
	callbacks_.insert(CallbackMap::value_type(event_id, callbackFunction));

	components_.push_back(component);

	return shared_from_this();
}

std::shared_ptr<SUIWindowInterface> SUIWindow::AddDataSourceContainer(std::string location_string, std::wstring value)
{
	SUI_WINDOW_COMPONENT component;
	component.type = 6;

	char *dup = _strdup(location_string.c_str());
	component.narrow_params.push_back(strtok(dup, ":"));
	component.narrow_params.push_back(strtok(nullptr, ":"));
	strtok(nullptr, ":");
	free(dup);

	component.wide_params.push_back(value);

	components_.push_back(component);

	return shared_from_this();
}

WindowCallbackFunction SUIWindow::GetFunctionById(uint32_t event_id)
{
	auto itr = callbacks_.find(event_id);
	if(itr != callbacks_.end())
	{
		return itr->second;
	}

	return [] (std::shared_ptr<swganh::object::Object>,uint32_t, std::vector<std::wstring>)
	{
		LOG(error) << "Client attempted to fetch an unknown callback";
	};
}