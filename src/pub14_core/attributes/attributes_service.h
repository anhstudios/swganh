// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SWGANH_ATTRIBUTES_SERVICE_H_
#define SWGANH_SWGANH_ATTRIBUTES_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>

#include "swganh/attributes/attributes_service_interface.h"
#include "swganh/attributes/attribute_template_interface.h"

#include "swganh/app/swganh_kernel.h"


namespace swganh_core {
namespace attributes {
	typedef std::map<
		std::string, 
		std::shared_ptr<swganh::attributes::AttributeTemplateInterface>
	> AttributeTemplates;

    class AttributesService: public swganh::attributes::AttributesServiceInterface
    {
    public:
        explicit AttributesService(swganh::app::SwganhKernel* kernel);
        virtual ~AttributesService();
        anh::service::ServiceDescription GetServiceDescription();
		
		std::shared_ptr<swganh::attributes::AttributeTemplateInterface> GetAttributeTemplate(const std::string& name);
        void SetAttributeTemplate(const std::shared_ptr<swganh::attributes::AttributeTemplateInterface> template_, const std::string& name); 
        void SendAttributesMessage(const std::string& template_name, const std::shared_ptr<swganh::object::Object> object);
        void Startup();

    private:
		AttributeTemplates attribute_templates_;

		swganh::app::SwganhKernel* kernel_;
    };

}}  // namespace swganh_core::attributes

#endif  // SWGANH_SWGANH_ATTRIBUTES_SERVICE_H_
