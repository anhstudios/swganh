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
namespace messages {
	struct AttributeListMessage;
}
namespace object {
	class Object; 
}
namespace attributes {

    class AttributeTemplateInterface 
    {
    public:
        virtual ~AttributeTemplateInterface() {}

        virtual swganh::messages::AttributeListMessage BuildAttributeTemplate(std::shared_ptr<swganh::object::Object> object) = 0;
        virtual void SetKernel(swganh::app::SwganhKernel* kernel) = 0;
		virtual swganh::app::SwganhKernel* GetKernel() = 0;
    };

    class BaseAttributeTemplate : public AttributeTemplateInterface
    {
    public:
        virtual ~BaseAttributeTemplate() {}

        virtual void SetKernel(swganh::app::SwganhKernel* kernel) { kernel_ = kernel; }
        virtual swganh::app::SwganhKernel* GetKernel() { return kernel_; }

    private:
        swganh::app::SwganhKernel* kernel_;
    };

}}
