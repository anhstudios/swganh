// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <map>

#include "base_attribute_template.h"

namespace swganh {
namespace attributes {
	class ArmorAttributeTemplate : public BaseAttributeTemplate
	{
	public:
		ArmorAttributeTemplate(swganh::EventDispatcher* dispatcher);
		swganh::messages::AttributeListMessage BuildAttributeTemplate(std::shared_ptr<swganh::object::Object> object);			
	};

}}
