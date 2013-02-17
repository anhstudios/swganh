// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <boost/thread/mutex.hpp>
#include <map>

#include "swganh_core/sui/sui_service_interface.h"

namespace swganh
{
namespace observer
{
	class ObserverInterface;
}
}

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
namespace simulation
{
	class SimulationServiceInterface;
}

namespace sui
{
	class SUIWindowInterface;
	class PythonRadialCreator;
}

namespace messages
{
	struct SUIEventNotification;
	struct RadialMenuSelection;
	namespace controllers 
	{
		class ObjectMenuRequest;
	}
}

}

namespace swganh
{
namespace sui
{
	
	class RadialInterface;

	typedef std::multimap<uint64_t,std::shared_ptr<swganh::sui::SUIWindowInterface>> WindowMap;
	typedef std::pair<WindowMap::iterator, WindowMap::iterator> WindowMapRange;

	class SUIService : public swganh::sui::SUIServiceInterface
	{
	public:
		
		SUIService(swganh::app::SwganhKernel* kernel);

		swganh::service::ServiceDescription GetServiceDescription();

		virtual std::shared_ptr<swganh::sui::SUIWindowInterface> CreateSUIWindow(std::string script_name, std::shared_ptr<swganh::object::Object> owner, 
							std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0);

		//Creates a new SUI page and returns the id of the corresponding window id
		virtual int32_t OpenSUIWindow(std::shared_ptr<swganh::sui::SUIWindowInterface> window);

		//UpdateWindow
		virtual int32_t UpdateSUIWindow(std::shared_ptr<swganh::sui::SUIWindowInterface> window);

		//Get Window
		virtual std::shared_ptr<swganh::sui::SUIWindowInterface> GetSUIWindowById(std::shared_ptr<swganh::object::Object> owner, int32_t windowId);

		virtual std::shared_ptr<SUIWindowInterface> GetSUIWindowByScriptName(std::shared_ptr<swganh::object::Object> owner, std::string script);

		//Forcefully closes a previously opened window.
		virtual void CloseSUIWindow(std::shared_ptr<swganh::object::Object> owner, int32_t windowId);

		/// PREBUILT SUI WINDOWS ///
		virtual std::shared_ptr<swganh::sui::SUIWindowInterface> CreateMessageBox(swganh::sui::MessageBoxType msgBox_type, std::wstring title, std::wstring caption,
			std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> ranged_object = nullptr, 
			float max_distance = 0);

		virtual std::shared_ptr<swganh::sui::SUIWindowInterface> CreateListBox(swganh::sui::ListBoxType lstBox_type, std::wstring title, std::wstring prompt, 
			std::vector<std::wstring> dataList, std::shared_ptr<swganh::object::Object> owner, 
			std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0);
			
		virtual std::shared_ptr<swganh::sui::SUIWindowInterface> CreateInputBox(swganh::sui::InputBoxType iptBox_type, std::wstring title, std::wstring prompt, 
			uint32_t input_max_length, std::shared_ptr<swganh::object::Object> owner, 
			std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0);

		void Startup();

	private:
		void _handleEventNotifyMessage(const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, swganh::messages::SUIEventNotification* message);
		void _handleObjectMenuSelection(const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, swganh::messages::RadialMenuSelection* message);

		std::shared_ptr<RadialInterface> GetRadialInterfaceForObject(std::shared_ptr<swganh::object::Object> target);

		// Radials
		void _handleObjectMenuRequest(const std::shared_ptr<swganh::object::Object>& object, swganh::messages::controllers::ObjectMenuRequest* message);
		void _handlePlayerLogoutEvent();

		std::map<std::string, std::shared_ptr<PythonRadialCreator>> radial_menus_;

		swganh::app::SwganhKernel* kernel_;
		std::string script_directory_;
		swganh::simulation::SimulationServiceInterface* simulation_service_;
		WindowMap window_lookup_;
		int32_t window_id_counter_;

		boost::mutex sui_mutex_;
	};

}
}
