// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <vector>

namespace swganh {
namespace app
{
	class SwganhKernel;
}

namespace object{
class Object;
}

namespace messages {
namespace controllers {
	struct RadialOptions;
}}

namespace sui {

class RadialInterface
{
public:
	RadialInterface(swganh::app::SwganhKernel* kernel)
		: kernel_(kernel)
	{
	}

	virtual std::vector<swganh::messages::controllers::RadialOptions> BuildRadial(std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> target, std::vector<swganh::messages::controllers::RadialOptions> radials) = 0;
	virtual void HandleRadial(std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> target, uint8_t action) = 0;
	virtual swganh::app::SwganhKernel* GetKernel() { return kernel_; }
protected:

	swganh::app::SwganhKernel* kernel_;
};

}}
