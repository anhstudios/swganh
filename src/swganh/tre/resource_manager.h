#ifndef SWGNAH_RESOURCE_MANAGER_H_
#define SWGANH_RESOURCE_MANAGER_H_

#include <map>

#include "swganh/tre/tre_archive.h"
#include "visitors/visitor_interface.h"

namespace swganh {
namespace tre {
	
	typedef std::map<std::string, std::shared_ptr<swganh::tre::VisitorInterface>> ResourceCache;

	class ResourceManager
	{
	public:
		ResourceManager(std::shared_ptr<swganh::tre::TreArchive> archive);

		void LoadResourceByName(const std::string& name, std::shared_ptr<VisitorInterface> visitor, bool is_cached=true);

		template<class ValueType>
		std::shared_ptr<ValueType> GetResourceByName(const std::string& name, bool is_cached=true)
		{
			auto itr = loadedResources_.find(name);
			if(itr != loadedResources_.end())
			{
				return std::static_pointer_cast<ValueType>(itr->second);
			}
			else
			{
				std::shared_ptr<ValueType> visitor = std::make_shared<ValueType>();
				LoadResourceByName(name, visitor, is_cached);
				return visitor;
			}
		}

	private:
		ResourceCache loadedResources_;
		std::shared_ptr<swganh::tre::TreArchive> archive_;
	};

}
}

#endif