#ifndef SWGANH_UI_SERVICE_INTERFACE_H_
#define SWGNAH_UI_SERVICE_INTERFACE_H_

#include <memory>
#include <anh/observer/observable_interface.h>
#include <anh/service/service_interface.h>

namespace swganh
{
namespace ui
{

class Page;

class UiService : public anh::service::ServiceInterface
{
public:
	
	//Creates a new SUI page and returns the id of the corresponding window
	int32_t CreatePage(std::shared_ptr<ObservableInterface> controller, Page page_info);
	
	//Creates a new SUI page that will automatically close when the ranged_object is max_distance away
	int32_t CreatePage(std::shared_ptr<ObservableInterface> controller, Page page_info, std::shared_ptr<Object> ranged_object, float max_distance);
	
	//Forcefully closes a previously opened page.
	void ForceClose(int32_t windowId);
};

}
}

#endif