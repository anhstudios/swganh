// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_LIB_BASE_ATTRIBUTE_TEMPLATE_H_
#define PUB14_CORE_LIB_BASE_ATTRIBUTE_TEMPLATE_H_

#include <map>

#include "swganh/attributes/attribute_template_interface.h"
#include "pub14_core/messages/attribute_list_message.h"

namespace swganh_core {
namespace attributes {
	typedef std::map<uint64_t,
		swganh::messages::AttributeListMessage
	> ObjectMapAttributeListMessages;
	class BaseAttributeTemplate : public swganh::attributes::AttributeTemplateInterface
	{
	public:
		BaseAttributeTemplate(anh::EventDispatcher* dispatcher);
		virtual void RegisterEventHandlers();
		virtual swganh::messages::AttributeListMessage BuildAttributeTemplate(std::shared_ptr<swganh::object::Object> object);		

		virtual bool HasAttributeListMessage(uint64_t object_id);
	protected:
		ObjectMapAttributeListMessages object_attribute_list_messages_;		
	};

}}


#endif //  PUB14_CORE_LIB_BASE_ATTRIBUTE_TEMPLATE_H_