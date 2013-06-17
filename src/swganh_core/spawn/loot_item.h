// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <functional>

namespace swganh
{
namespace app
{
	class SwganhKernel;
}

namespace object
{
	class Object;
}

namespace spawn
{

typedef std::function<std::shared_ptr<swganh::object::Object>(swganh::app::SwganhKernel* kernel)> LootFactory;

class LootItem
{
public:

	LootItem(float drop_percent, LootFactory factory);

	std::shared_ptr<swganh::object::Object> AttemptDrop(swganh::app::SwganhKernel* kernel, float attempt);
	
private:

	LootFactory factory_;

};

}
}