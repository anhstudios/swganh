
#include "sui_service.h"
#include "sui_window.h"

#include <algorithm>

#include "swganh/observer/observer_interface.h"

#include "swganh_core/messages/sui_create_page_message.h"
#include "swganh_core/messages/sui_event_notification.h"
#include "swganh_core/messages/sui_update_page_message.h"
#include "swganh_core/messages/sui_force_close.h"
#include "swganh_core/messages/controllers/object_menu_request.h"
#include "swganh_core/messages/controllers/object_menu_response.h"

#include "swganh/connection/connection_client_interface.h"
#include "swganh/connection/connection_service_interface.h"
#include "swganh/sui/sui_window_interface.h"
#include "swganh/sui/radial_interface.h"
#include "swganh_core/object/object.h"
#include "swganh_core/object/player/player.h"

#include "swganh/simulation/simulation_service_interface.h"

#include "swganh/sui/python_radial_creator.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/scripting/utilities.h"
#include "swganh/service/service_manager.h"
#include "swganh/event_dispatcher.h"


using namespace swganh::service;
using namespace swganh::app;

using namespace swganh::sui;
using namespace swganh::sui;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::connection;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::simulation;

SUIService::SUIService(swganh::app::SwganhKernel* kernel)
	: kernel_(kernel)
	, script_directory_(kernel->GetAppConfig().script_directory + "/radials/")
	, window_id_counter_(0)
{
}

void SUIService::Startup()
{
	//Subscribe to EventNotifcation
	auto connection_service = kernel_->GetServiceManager()->GetService<ConnectionServiceInterface>("ConnectionService");
	connection_service->RegisterMessageHandler(&SUIService::_handleEventNotifyMessage, this);
	
	// Register Radial Events
	simulation_service_ = kernel_->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");
	simulation_service_->RegisterControllerHandler(&SUIService::_handleObjectMenuRequest, this);

	//Subscribe to player logouts
	kernel_->GetEventDispatcher()->Subscribe(
		"Connection::PlayerRemoved",
		[this] (std::shared_ptr<swganh::EventInterface> incoming_event)
	{
		//Clear all of this player's SUIs
		const auto& player = std::static_pointer_cast<swganh::ValueEvent<std::shared_ptr<Player>>>(incoming_event)->Get();
		if(player != nullptr)
		{
			WindowMapRange range = window_lookup_.equal_range(player->GetObjectId());
			window_lookup_.erase(range.first, range.second);
		}
	});
}

void SUIService::_handleEventNotifyMessage(const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, swganh::messages::SUIEventNotification* message)
{
	auto owner = client->GetController()->GetId();
	WindowMapRange range = window_lookup_.equal_range(owner);
	std::shared_ptr<SUIWindowInterface> result = nullptr;
	for(auto itr=range.first; itr != range.second; ++itr)
	{
		if(itr->second->GetWindowId() == message->window_id)
		{
			if(itr->second->GetFunctionById(message->event_type)(itr->second->GetOwner(), message->event_type, message->returnList))
			{
				window_lookup_.erase(itr);
			}
			break;
		}
	};
}

void SUIService::_handleObjectMenuRequest(
	const std::shared_ptr<swganh::object::Object>& controller,
	ObjectMenuRequest* message)
{
	PythonRadialCreator creator("radials.radial_menu", "PyRadialMenu");
	auto radial = creator();
	
	auto owner = simulation_service_->GetObjectById(message->owner_id);
	auto target = simulation_service_->GetObjectById(message->target_id);

	radial->BuildRadial(owner, target, message->radial_options);
	
	// Get the response filled in by python
	auto response = *owner->GetMenuResponse();
	response.owner_id = message->owner_id;
	response.target_id = message->target_id;
	response.response_count = message->response_count;
	// send it
	if (owner)
	{
		owner->GetController()->Notify(&response);
	}
	
}

ServiceDescription SUIService::GetServiceDescription()
{
	ServiceDescription service_description(
		"SuiService",
		"sui",
		"0.1",
		"127.0.0.1",
		0,
		0,
		0);

	return service_description;
}

std::shared_ptr<SUIWindowInterface> SUIService::CreateSUIWindow(std::string script_name, std::shared_ptr<swganh::object::Object> owner, 
							std::shared_ptr<swganh::object::Object> ranged_object, float max_distance)
{
	return std::shared_ptr<SUIWindowInterface>(new SUIWindow(script_name, owner, ranged_object, max_distance));
}

//Creates a new SUI page and returns the id of the corresponding window id
int32_t SUIService::OpenSUIWindow(std::shared_ptr<SUIWindowInterface> window)
{
	int32_t window_id = -1;
	auto owner = window->GetOwner()->GetController();
	if(owner != nullptr)
	{
		window_id = window_id_counter_++;
		window->SetWindowId(window_id);
		window_lookup_.insert(WindowMap::value_type(window->GetOwner()->GetObjectId(), window));
		
		//Send Create to controller
		SUICreatePageMessage create_page;
		create_page.window_id = window_id;
		create_page.script_name = window->GetScriptName();
		create_page.components = std::move(window->GetComponents());
		if(window->GetRangedObject())
		{
			create_page.ranged_object = window->GetRangedObject()->GetObjectId();
			create_page.range = window->GetMaxDistance();
		}
		else
		{
			create_page.ranged_object = 0;
			create_page.range = 0.0f;
		}
		owner->Notify(&create_page);
	}
	return window_id;
}

//UpdateWindow
int32_t SUIService::UpdateSUIWindow(std::shared_ptr<SUIWindowInterface> window)
{
	int32_t window_id = -1;
	auto owner = window->GetOwner()->GetController();
	if(owner != nullptr)
	{
		window_id = window->GetWindowId();

		//Send Update to controller
		SUIUpdatePageMessage update_page;
		update_page.window_id = window_id;
		update_page.script_name = window->GetScriptName();
		update_page.components = std::move(window->GetComponents());
		if(window->GetRangedObject())
		{
			update_page.ranged_object = window->GetRangedObject()->GetObjectId();
			update_page.range = window->GetMaxDistance();
		}
		else
		{
			update_page.ranged_object = 0;
			update_page.range = 0.0f;
		}
		owner->Notify(&update_page);
	}
	return window_id;
}

//Get Window
std::shared_ptr<SUIWindowInterface> SUIService::GetSUIWindowById(std::shared_ptr<swganh::object::Object> owner, int32_t windowId)
{
	WindowMapRange range = window_lookup_.equal_range(owner->GetObjectId());
	std::shared_ptr<SUIWindowInterface> result = nullptr;
	std::find_if(range.first, range.second, [&] (WindowMap::value_type& element) -> bool {
		if(element.second->GetWindowId() == windowId)
		{
			result = element.second;
			return true;
		}
		return false;
	});
	return result;
}

//Forcefully closes a previously opened page.
void SUIService::CloseSUIWindow(std::shared_ptr<swganh::object::Object> owner, int32_t windowId)
{
	WindowMapRange range = window_lookup_.equal_range(owner->GetObjectId());
	std::shared_ptr<SUIWindowInterface> result = nullptr;
	for(auto itr=range.first; itr != range.second; ++itr)
	{
		if(itr->second->GetWindowId() == windowId)
		{
			result = itr->second;
			window_lookup_.erase(itr);
			break;
		}
	};

	if(result != nullptr)
	{
		//Send Window Force Close
		
		auto controller = owner->GetController();
		if(controller)
		{
			SUIForceClose force_close;
			force_close.window_id = windowId;
			controller->Notify(&force_close);
		}
	}
}

std::shared_ptr<SUIWindowInterface> SUIService::CreateMessageBox(MessageBoxType msgBox_type, std::wstring title, std::wstring caption,
			std::shared_ptr<Object> owner, std::shared_ptr<Object> ranged_object, float max_distance)
{
	std::shared_ptr<SUIWindowInterface> result = CreateSUIWindow("Script.messageBox", owner, ranged_object, max_distance);
	
	//Write out the basic properties
	result->SetProperty("bg.caption.lblTitle:Text", title)->SetProperty("Prompt.lblPrompt:Text", caption);
	
	//Write out the buttons
	result->SetProperty("btnRevert:visible", L"False");
	switch(msgBox_type)
	{
	case MESSAGE_BOX_OK:
		result->SetProperty("btnOk:visible", L"True")->SetProperty("btnOk:Text", L"@ok");
		result->SetProperty("btnCancel:visible", L"False");
		break;
	case MESSAGE_BOX_OK_CANCEL:
		result->SetProperty("btnOk:visible", L"True")->SetProperty("btnOk:Text", L"@ok");
		result->SetProperty("btnCancel:visible", L"True")->SetProperty("btnCancel:Text", L"@cancel");
		break;
	case MESSAGE_BOX_YES_NO:
		result->SetProperty("btnOk:visible", L"True")->SetProperty("btnOk:Text", L"@yes");
		result->SetProperty("btnCancel:visible", L"True")->SetProperty("btnCancel:Text", L"@no");
		break;
	}
	return result;
}

std::shared_ptr<SUIWindowInterface> SUIService::CreateListBox(ListBoxType lstBox_type, std::wstring title, std::wstring prompt, 
	std::vector<std::wstring> dataList, std::shared_ptr<Object> owner, std::shared_ptr<Object> ranged_object, float max_distance)
{
	std::shared_ptr<SUIWindowInterface> result = CreateSUIWindow("Script.listBox", owner, ranged_object, max_distance);
	
	//Write out the basic properties
	result->SetProperty("bg.caption.lblTitle:Text", title)->SetProperty("Prompt.lblPrompt:Text", prompt);

	//Write out the buttons
	switch(lstBox_type) 
	{
	case LIST_BOX_OKCANCEL:
		result->SetProperty("btnOk:visible", L"True")->SetProperty("btnOk:Text", L"@ok");
		result->SetProperty("btnCancel:visible", L"True")->SetProperty("btnCancel:Text", L"@cancel");
		break;
	case LIST_BOX_OK:
		result->SetProperty("btnOk:visible", L"True");
		result->SetProperty("btnCancel:visible", L"False");
		break;
	}

	//Clear out the list
	result->ClearDataSource("List.dataList");

	//Write out the list
	size_t index = 0;
	std::wstringstream wss;
	std::stringstream ss;
	for(auto& string : dataList)
	{
		wss << index;
		ss << "List.dataList." << index << ":Text";

		//Now for each entry, we add it to the list with the id as the name
		result->AddProperty("List.dataList:Name", wss.str());

		//And Then set it's text property to the given string
		result->SetProperty(ss.str(), string);

		//Increment and clear the streams
		++index;
		wss.str(L"");
		ss.str("");
	}

	return result;
}
			
std::shared_ptr<SUIWindowInterface> SUIService::CreateInputBox(InputBoxType iptBox_type, std::wstring title, std::wstring prompt, 
	uint32_t input_max_length, std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<Object> ranged_object, float max_distance)
{
	std::shared_ptr<SUIWindowInterface> result = CreateSUIWindow("Script.inputBox", owner, ranged_object, max_distance);
	result->SetProperty("bg.caption.lblTitle:Text", title)->SetProperty("Prompt.lblPrompt:Text", prompt);

	switch(iptBox_type)
	{
	case INPUT_BOX_OK:
		break;
	case INPUT_BOX_OKCANCEL:	
		break;
	}

	result->SetProperty("cmbInput:visible", L"False");
	return result;
}