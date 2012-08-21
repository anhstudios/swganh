// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_ATTRIBUTES_ATTRIBUTES_SERVICE_H_
#define SWGANH_ATTRIBUTES_ATTRIBUTES_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>

#include "anh/service/service_interface.h"
#include "attribute_template_interface.h"

namespace swganh {
namespace object {
	class Object;
}}

namespace swganh {
namespace attributes {

    class AttributesServiceInterface: public anh::service::ServiceInterface
    {
    public:        
        virtual std::shared_ptr<AttributeTemplateInterface> GetAttributeTemplate(const std::string& name) = 0;
        virtual void SetAttributeTemplate(const std::shared_ptr<AttributeTemplateInterface> template_, const std::string& name) = 0;        
		virtual void SendAttributesMessage(const std::string& template_name, const std::shared_ptr<swganh::object::Object> object) = 0;
    };

}}  // namespace swganh::attributes

#endif  // SWGANH_ATTRIBUTES_ATTRIBUTES_SERVICE_H_
