#ifndef SWGNAH_RESOURCE_MANAGER_H_
#define SWGANH_RESOURCE_MANAGER_H_

#include <anh/resource/resource_manager_interface.h>
#include <map>

#include "swganh/tre/tre_archive.h"

namespace swganh {
namespace tre {
	
	class ResourceManager : public anh::resource::ResourceManagerInterface
	{
	public:
		ResourceManager(std::shared_ptr<swganh::tre::TreArchive> archive);

		std::shared_ptr<swganh::tre::VisitorInterface> getResourceByName(const std::string& name, swganh::tre::VisitorType type);

	private:
		std::map<std::string, std::shared_ptr<swganh::tre::VisitorInterface>> loadedResources_;
		std::shared_ptr<swganh::tre::TreArchive> archive_;
	};

}
}

#endif