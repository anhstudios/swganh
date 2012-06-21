// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef ANH_RESOURCE_RESOURCE_ARCHIVE_INTERFACE_H_
#define ANH_RESOURCE_RESOURCE_ARCHIVE_INTERFACE_H_

#include <cstdint>
#include <string>
#include <vector>

namespace anh {
namespace resource {

	class ResourceArchiveInterface
	{
	public:
		virtual ~ResourceArchiveInterface() {}

		virtual bool Open() = 0;
		virtual uint32_t GetResourceSize(const std::string& resource_name) const = 0;
		virtual void GetResource(const std::string& resource_name, std::vector<char>& buffer) = 0;
	};

}}  // namespace anh::resource

#endif  // ANH_RESOURCE_RESOURCE_ARCHIVE_INTERFACE_H_
