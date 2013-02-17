// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "swganh/event_dispatcher.h"

namespace swganh {
namespace app {
	class SwganhKernel;
}
namespace object {
	class Object; 

    class TemplateInterface 
    {
    public:
		virtual std::shared_ptr<swganh::object::Object> CreateTemplate(swganh::app::SwganhKernel* kernel, std::map<std::string, std::string> parameters) = 0;		
    };

}}
