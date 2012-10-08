// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <memory>
#include <string>

namespace swganh {
namespace tre {
	class VisitorInterface;
	enum VisitorType;
}
}

namespace swganh {
namespace resource {
	class ResourceManagerInterface
	{
	public:
		void LoadResourceByName(const std::string& name, std::shared_ptr<swganh::tre::VisitorInterface> visitor, bool is_cached=true);
	};

}
}
