// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "swganh/hash_string.h"
#include "swganh/scripting/python_script.h"
#include "swganh_core/attributes/attributes_service_interface.h"
#include "swganh_core/attributes/attribute_template_interface.h"

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
	
	/*!
	* Service that pushes attribute listings to the clients
	* when requested.
	*/
    class AttributesService: public swganh::attributes::AttributesServiceInterface
    {
    public:
		typedef std::map<int8_t, std::shared_ptr<AttributeTemplateInterface>> AttributeTemplateMap;
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
		std::shared_ptr<swganh::attributes::AttributeTemplateInterface> GetAttributeTemplate(int8_t template_id);
        
		/*!
		* Adds an attribute to the lookup with the given id
		* @param template_ the new template
		* @param template_id_ the new template's id
		*/
		void SetAttributeTemplate(const std::shared_ptr<swganh::attributes::AttributeTemplateInterface> template_, int8_t template_id); 
		
		/*!
		* Returns true if the given template id is bound to a template
		* @param template_id the id to check
		*/
		bool HasAttributeTemplate(int8_t template_id);
        
		/*!
		* Send the attributes list message for object to actor
		* @param object the object to build the attribute list from
		* @param actor the object to send the attribute list to
		*/
		void SendAttributesMessage(const std::shared_ptr<swganh::object::Object> object, const std::shared_ptr<swganh::object::Object> actor);

		std::shared_ptr<AttributeTemplateInterface> GetPythonAttributeTemplate(std::string filename);
		
		/*!
		* Method called on the service at start up.
		*/
		void Startup();
		
    private:		

		/*!
		* Loads a static list of attribute templates from python
		*/
		void LoadAttributeTemplates_();
		AttributeTemplateMap attribute_templates_;
		std::vector<std::string> python_attribute_templates_;
		swganh::simulation::SimulationServiceInterface* simulation_service_;
		swganh::app::SwganhKernel* kernel_;	
    };

}}  // namespace swganh::attributes
