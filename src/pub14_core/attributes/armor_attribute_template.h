// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_LIB_ARMOR_ATTRIBUTE_TEMPLATE_H_
#define PUB14_CORE_LIB_ARMOR_ATTRIBUTE_TEMPLATE_H_

#include <map>

#include "base_attribute_template.h"

namespace swganh_core {
namespace attributes {
	class ArmorAttributeTemplate : public BaseAttributeTemplate
	{
	public:
		ArmorAttributeTemplate(anh::EventDispatcher* dispatcher);
		swganh::messages::AttributeListMessage BuildAttributeTemplate(std::shared_ptr<swganh::object::Object> object);			
	};

}}


#endif //  PUB14_CORE_LIB_ARMOR_ATTRIBUTE_TEMPLATE_H_