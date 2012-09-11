// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "swganh/service/service_interface.h"
#include "attribute_template_interface.h"

namespace swganh {
namespace object {
	class Object;
}}

namespace swganh {
namespace attributes {

	enum AttributeTemplateId : uint8_t
	{
		ARMOR = 0,
		CRAFTING_TOOL,
		DEED,
		DROID,
		FACTORY_CRATE,
		FOOD,
		FURNITURE,
		MEDS,
		SHIP,
		SURVEY_TOOL,
		WEAPON,
		WEARABLE 
	};

    class AttributesServiceInterface: public swganh::service::ServiceInterface
    {
    public:        
        virtual std::shared_ptr<AttributeTemplateInterface> GetAttributeTemplate(AttributeTemplateId template_id) = 0;
        virtual void SetAttributeTemplate(const std::shared_ptr<AttributeTemplateInterface> template_, AttributeTemplateId template_id) = 0;        
		virtual bool HasAttributeTemplate(AttributeTemplateId template_id) = 0;
		virtual void SendAttributesMessage(const std::shared_ptr<swganh::object::Object> object, const std::shared_ptr<swganh::object::Object> actor) = 0;
    };

}}  // namespace swganh::attributes
