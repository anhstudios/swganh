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
		virtual std::shared_ptr<swganh::tre::VisitorInterface> getResourceByName(const std::string& name, swganh::tre::VisitorType type) = 0;
	};

}
}

#endif