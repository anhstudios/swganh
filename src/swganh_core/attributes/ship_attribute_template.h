// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "base_attribute_template.h"

namespace swganh {
namespace attributes {
	class ShipAttributeTemplate : public BaseAttributeTemplate
	{
	public:
		ShipAttributeTemplate(swganh::EventDispatcher* dispatcher);            
		swganh::messages::AttributeListMessage BuildAttributeTemplate(std::shared_ptr<swganh::object::Object> object);		
	};

}}
