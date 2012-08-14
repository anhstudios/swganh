#ifndef SWGANH_UI_SERVICE_H_
#define SWGNAH_UI_SERVICE_H_

#include <map>

#include "swganh/sui/sui_service_interface.h"

namespace swganh
{
namespace app
{
	class SwganhKernel;
}

namespace connection
{	
	class ConnectionClientInterface;
}

namespace object
{
	class Object;
}

namespace sui
{
	class SUIWindowInterface;
}

namespace messages
{
	struct SUIEventNotification;
}

}

namespace swganh_core
{
namespace sui
{
	
	typedef std::multimap<uint64_t,std::shared_ptr<swganh::sui::SUIWindowInterface>> WindowMap;
	typedef std::pair<WindowMap::iterator, WindowMap::iterator> WindowMapRange;

	class SUIService : public swganh::sui::SUIServiceInterface
	{
	public:
		
		SUIService(swganh::app::SwganhKernel* kernel);

		anh::service::ServiceDescription GetServiceDescription();

		virtual std::shared_ptr<swganh::sui::SUIWindowInterface> GetNewSUIWindow(std::string script_name, std::shared_ptr<swganh::object::Object> owner, 
							std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0);

		//Creates a new SUI page and returns the id of the corresponding window id
		virtual int32_t CreateSUIWindow(std::shared_ptr<swganh::sui::SUIWindowInterface> window);

		//UpdateWindow
		virtual int32_t UpdateSUIWindow(std::shared_ptr<swganh::sui::SUIWindowInterface> window);

		//Get Window
		virtual std::shared_ptr<swganh::sui::SUIWindowInterface> GetSUIWindowById(std::shared_ptr<swganh::object::Object> owner, int32_t windowId);

		//Forcefully closes a previously opened window.
		virtual void CloseSUIWindow(std::shared_ptr<swganh::object::Object> owner, int32_t windowId);

	private:

		void _handleEventNotifyMessage(const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, swganh::messages::SUIEventNotification message);
		void _handlePlayerLogoutEvent();

		WindowMap window_lookup_;
		int32_t window_id_counter_;
	};

}
}

#endif