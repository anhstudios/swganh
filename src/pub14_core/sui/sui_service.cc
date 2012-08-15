
#include "sui_service.h"

#include <algorithm>

#include "pub14_core/messages/sui_create_page_message.h"
#include "pub14_core/messages/sui_event_notification.h"
#include "pub14_core/messages/sui_update_page_message.h"
#include "pub14_core/messages/sui_force_close.h"

#include "swganh/connection/connection_client_interface.h"
#include "swganh/connection/connection_service_interface.h"
#include "swganh/sui/sui_window_interface.h"
#include "swganh/object/object.h"

#include "sui_window.h"

#include "swganh/app/swganh_kernel.h"
#include "anh/service/service_manager.h"
#include "anh/event_dispatcher.h"


using namespace anh::service;
using namespace swganh::app;

using namespace swganh_core::sui;
using namespace swganh::sui;
using namespace swganh::connection;
using namespace swganh::messages;

SUIService::SUIService(swganh::app::SwganhKernel* kernel)
	: window_id_counter_(0)
{
	//Subscribe to EventNotifcation
	auto connection_service = kernel->GetServiceManager()->GetService<ConnectionServiceInterface>("ConnectionService");
	connection_service->RegisterMessageHandler(&SUIService::_handleEventNotifyMessage, this);

	//Subscribe to player logouts
	kernel->GetEventDispatcher()->Subscribe(
		"Connection::PlayerRemoved",
		[this] (std::shared_ptr<anh::EventInterface> incoming_event)
	{
		
	});

}

void SUIService::_handleEventNotifyMessage(const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, swganh::messages::SUIEventNotification message)
{
	auto owner = client->GetPlayerId();
	WindowMapRange range = window_lookup_.equal_range(owner);
	std::shared_ptr<SUIWindowInterface> result = nullptr;
	for(auto itr=range.first; itr != range.second; ++itr)
	{
		if(itr->second->GetWindowId() == message.window_id)
		{
			itr->second->GetFunctionById(message.event_type)(itr->second->GetOwner(), message.event_type, message.returnList);
			break;
		}
	};

}

ServiceDescription SUIService::GetServiceDescription()
{
	ServiceDescription service_description(
		"SUIService",
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
		owner->Notify(create_page);
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
		owner->Notify(update_page);
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
			controller->Notify(force_close);
		}
	}
}