#ifndef SWGNAH_RESOURCE_MANAGER_H_
#define SWGANH_RESOURCE_MANAGER_H_

#include <map>

#include "swganh/tre/tre_archive.h"
#include "visitors/visitor_interface.h"

namespace swganh {
namespace tre {
	
	class ResourceManager
	{
	public:
		ResourceManager(std::shared_ptr<swganh::tre::TreArchive> archive);

		std::shared_ptr<swganh::tre::VisitorInterface> getResourceByName(const std::string& name, swganh::tre::VisitorType type, bool is_cached=true);

	private:
		std::map<std::string, std::shared_ptr<swganh::tre::VisitorInterface>> loadedResources_;
		std::shared_ptr<swganh::tre::TreArchive> archive_;
	};

}
}

#endif