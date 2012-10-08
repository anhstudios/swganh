// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <map>

#include "swganh/attributes/attribute_template_interface.h"
#include "swganh_core/messages/attribute_list_message.h"

namespace swganh {
namespace attributes {
	typedef std::map<uint64_t, swganh::messages::AttributeListMessage> ObjectMapAttributeListMessages;

	/**
	* This is the base implementation of the AttributeTemplateInterface.
	*/
	class BaseAttributeTemplate : public swganh::attributes::AttributeTemplateInterface
	{
	public:
		/*
		* Create a new instance of this base template
		* @param dispatcher the dispatcher to use for this attribute template.
		*/
		BaseAttributeTemplate(swganh::EventDispatcher* dispatcher);

		/**
		* Registers event handlers required by this template
		* @Override
		*/
		virtual void RegisterEventHandlers();

		/**
		* Builds an attribute list message using this template.
		* @param object the object to buid the message for
		* @Override
		*/
		virtual swganh::messages::AttributeListMessage BuildAttributeTemplate(std::shared_ptr<swganh::object::Object> object);		

		/**
		* @param object_id the object_id to check
		* @return true if the attribute list message for the given object_id is cached
		* @Override
		*/
		virtual bool HasAttributeListMessage(uint64_t object_id);

	protected:
		ObjectMapAttributeListMessages object_attribute_list_messages_;		
	};

}}
