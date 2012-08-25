#ifndef ANH_RESOURCE_MANAGER_H_
#define ANH_RESOURCE_MANAGER_H_

#include <memory>
#include <string>

namespace swganh {
namespace tre {
	class VisitorInterface;
	enum VisitorType;
}
}

namespace anh {
namespace resource {
	class ResourceManagerInterface
	{
	public:
		void LoadResourceByName(const std::string& name, std::shared_ptr<swganh::tre::VisitorInterface> visitor, bool is_cached=true);
	};

}
}

#endif