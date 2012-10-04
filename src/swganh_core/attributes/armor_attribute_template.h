// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <map>

#include "base_attribute_template.h"

namespace swganh {
namespace attributes {

	/*!
	* The attribute template for armor related items.
	*/
	class ArmorAttributeTemplate : public BaseAttributeTemplate
	{
	public:

		/*!
		* Creates a the armor attribute template
		* @param dispatcher the dispatcher to associate this template with
		*/
		ArmorAttributeTemplate(swganh::EventDispatcher* dispatcher);

		/**
		* Builds an attribute list message using this template.
		* @param object the object to buid the message for
		* @Override
		*/
		swganh::messages::AttributeListMessage BuildAttributeTemplate(std::shared_ptr<swganh::object::Object> object);			
	};

}}
