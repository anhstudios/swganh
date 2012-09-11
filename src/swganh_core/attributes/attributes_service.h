// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "swganh/hash_string.h"
#include "swganh/attributes/attributes_service_interface.h"
#include "swganh/attributes/attribute_template_interface.h"

#include "swganh_core/messages/controllers/get_attributes_batch.h"

namespace swganh {
namespace app {
	class SwganhKernel;
}
namespace command {
	class CommandInterface;
	class CommandServiceInterface;
	struct CommandProperties;
}
namespace simulation {
	class SimulationServiceInterface; 
}
namespace object {
	class ObjectController;
	class Creature;
}}

namespace swganh {
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
        swganh::service::ServiceDescription GetServiceDescription();
		
		std::shared_ptr<swganh::attributes::AttributeTemplateInterface> GetAttributeTemplate(swganh::attributes::AttributeTemplateId template_id);
        void SetAttributeTemplate(const std::shared_ptr<swganh::attributes::AttributeTemplateInterface> template_, swganh::attributes::AttributeTemplateId template_id); 
		bool HasAttributeTemplate(swganh::attributes::AttributeTemplateId template_id);
        void SendAttributesMessage(const std::shared_ptr<swganh::object::Object> object, const std::shared_ptr<swganh::object::Object> actor);
		void Startup();
		void HandleGetAttributesBatch(const std::shared_ptr<swganh::object::Object> object, const std::shared_ptr<swganh::object::Object> actor);
    private:		
		void LoadAttributeTemplates_();
		
		AttributeTemplates attribute_templates_;
		
		swganh::simulation::SimulationServiceInterface* simulation_service_;
		swganh::app::SwganhKernel* kernel_;
    };

}}  // namespace swganh::attributes
