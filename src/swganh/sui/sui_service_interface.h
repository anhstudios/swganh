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

class UIElement;

class SuiServiceInterface : public anh::service::ServiceInterface
{
public:
	
	virtual std::shared_ptr<UIElement> FindPageByName(const std::string& name) = 0;

	//Creates a new SUI page and returns the id of the corresponding window id
	virtual int32_t CreatePage(std::shared_ptr<UIElement> page_info, 
								std::shared_ptr<swganh::object::Object> owner, 
								std::shared_ptr<swganh::object::Object> ranged_object = nullptr, 
								float max_distance = 0) = 0;
	
	//Forcefully closes a previously opened page.
	virtual void ForceClose(int32_t windowId) = 0;
};

}
}

#endif