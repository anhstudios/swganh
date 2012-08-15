#ifndef SWGANH_UI_SERVICE_INTERFACE_H_
#define SWGNAH_UI_SERVICE_INTERFACE_H_

#include <memory>
#include <anh/service/service_interface.h>

namespace swganh
{
namespace object
{
	class Object;
}
namespace sui
{
	class SUIWindowInterface;

	class SUIServiceInterface : public anh::service::ServiceInterface
	{
	public:

		virtual std::shared_ptr<SUIWindowInterface> CreateSUIWindow(std::string script_name, std::shared_ptr<swganh::object::Object> owner, 
							std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0) = 0;

		//Creates a new SUI page and returns the id of the corresponding window id
		virtual int32_t OpenSUIWindow(std::shared_ptr<SUIWindowInterface> window) = 0;

		//UpdateWindow
		virtual int32_t UpdateSUIWindow(std::shared_ptr<SUIWindowInterface> window) = 0;

		//Get Window
		virtual std::shared_ptr<SUIWindowInterface> GetSUIWindowById(std::shared_ptr<swganh::object::Object> owner, int32_t windowId) = 0;

		//Forcefully closes a previously opened window.
		virtual void CloseSUIWindow(std::shared_ptr<swganh::object::Object> owner, int32_t windowId) = 0;

	};

}
}

#endif