// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_ATTRIBUTE_TEMPLATE_INTERFACE_H_
#define SWGANH_ATTRIBUTE_TEMPLATE_INTERFACE_H_

#include <cstdint>
#include <memory>
#include <string>

#include "anh/event_dispatcher.h"

namespace swganh {
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
		AttributeTemplateInterface(anh::EventDispatcher* dispatcher)
			: event_dispatcher_(dispatcher){}
        virtual void RegisterEventHandlers() = 0;
        virtual swganh::messages::AttributeListMessage BuildAttributeTemplate(std::shared_ptr<swganh::object::Object> object) = 0;		
    protected:
        anh::EventDispatcher* event_dispatcher_; 
    };

}}


#endif // SWGANH_ATTRIBUTE_TEMPLATE_INTERFACE_H_