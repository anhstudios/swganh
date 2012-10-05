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
	
	/*!
	* Service that pushes attribute listings to the clients
	* when requested.
	*/
    class AttributesService: public swganh::attributes::AttributesServiceInterface
    {
    public:

		/*!
		* Creates a new instance
		*/
        explicit AttributesService(swganh::app::SwganhKernel* kernel);
        
		/*!
		* Returns the service description for this service
		*/
		swganh::service::ServiceDescription GetServiceDescription();
		
		/*!
		* @param template_id the attribute to fetch
		* @return the attribute template associated with a particular id
		*/
		std::shared_ptr<swganh::attributes::AttributeTemplateInterface> GetAttributeTemplate(swganh::attributes::AttributeTemplateId template_id);
        
		/*!
		* Adds an attribute to the lookup with the given id
		* @param template_ the new template
		* @param template_id_ the new template's id
		*/
		void SetAttributeTemplate(const std::shared_ptr<swganh::attributes::AttributeTemplateInterface> template_, swganh::attributes::AttributeTemplateId template_id); 
		
		/*!
		* Returns true if the given template id is bound to a template
		* @param template_id the id to check
		*/
		bool HasAttributeTemplate(swganh::attributes::AttributeTemplateId template_id);
        
		/*!
		* Send the attributes list message for object to actor
		* @param object the object to build the attribute list from
		* @param actor the object to send the attribute list to
		*/
		void SendAttributesMessage(const std::shared_ptr<swganh::object::Object> object, const std::shared_ptr<swganh::object::Object> actor);
		
		/*!
		* Method called on the service at start up.
		*/
		void Startup();
		
    private:		

		/*!
		* Loads a static list of attribute templates
		*/
		void LoadAttributeTemplates_();
		
		AttributeTemplates attribute_templates_;
		
		swganh::simulation::SimulationServiceInterface* simulation_service_;
		swganh::app::SwganhKernel* kernel_;
    };

}}  // namespace swganh::attributes
