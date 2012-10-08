// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "base_attribute_template.h"

namespace swganh {
namespace attributes {

	/**
	* The attributes template used for droids
	*/
	class DroidAttributeTemplate : public BaseAttributeTemplate
	{
	public:
		/**
		* Creates a new instance
		*/
		DroidAttributeTemplate(swganh::EventDispatcher* dispatcher);            
		
		/**
		* Builds an attribute list message using this template.
		* @param object the object to buid the message for
		* @Override
		*/
		swganh::messages::AttributeListMessage BuildAttributeTemplate(std::shared_ptr<swganh::object::Object> object);		
	};

}}
