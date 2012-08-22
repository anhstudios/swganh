// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SWGANH_ATTRIBUTES_SERVICE_H_
#define SWGANH_SWGANH_ATTRIBUTES_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "anh/hash_string.h"
#include "swganh/attributes/attributes_service_interface.h"
#include "swganh/attributes/attribute_template_interface.h"

#include "pub14_core/messages/controllers/get_attributes_batch.h"

namespace swganh {
namespace app {
	class SwganhKernel;
}
namespace simulation {
	class SimulationServiceInterface; 
}
namespace object {
	class ObjectController;
}
}

namespace swganh_core {
namespace attributes {
	typedef std::map<
		swganh::attributes::AttributeTemplateId, 
		std::shared_ptr<swganh::attributes::AttributeTemplateInterface>
	> AttributeTemplates;
	
    class AttributesService: public swganh::attributes::AttributesServiceInterface
    {
    public:
        explicit AttributesService(swganh::app::SwganhKernel* kernel);
        virtual ~AttributesService();
        anh::service::ServiceDescription GetServiceDescription();
		
		std::shared_ptr<swganh::attributes::AttributeTemplateInterface> GetAttributeTemplate(swganh::attributes::AttributeTemplateId template_id);
        void SetAttributeTemplate(const std::shared_ptr<swganh::attributes::AttributeTemplateInterface> template_, swganh::attributes::AttributeTemplateId template_id); 
		bool HasAttributeTemplate(swganh::attributes::AttributeTemplateId template_id);
        void SendAttributesMessage(const std::shared_ptr<swganh::object::Object> object);
		void Startup();

    private:		
		void LoadAttributeTemplates_();
		void HandleGetAttributesBatch_(const std::shared_ptr<swganh::object::ObjectController>& controller, swganh::messages::controllers::GetAttributesBatchMessage message);
		AttributeTemplates attribute_templates_;
		
		swganh::simulation::SimulationServiceInterface* simulation_service_;
		swganh::app::SwganhKernel* kernel_;
    };

}}  // namespace swganh_core::attributes

#endif  // SWGANH_SWGANH_ATTRIBUTES_SERVICE_H_
