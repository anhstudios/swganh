
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "character_create.h"

#include <boost/python.hpp>

#include "swganh/logger.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/service/service_manager.h"
#include "swganh/scripting/utilities.h"
#include "swganh_core/simulation/simulation_service.h"
#include "swganh_core/object/creature/creature.h"

namespace bp = boost::python;
using namespace swganh::character;
using namespace swganh::scripting;

PyCharacterCreate::PyCharacterCreate(swganh::app::KernelInterface* kernel)
	: kernel_(kernel)
{}

uint64_t PyCharacterCreate::CreateCharacter(
	const std::wstring& full_name, const std::string& profession, 
	const std::string& location, float height, const std::string& bio, const std::string& customization, 
	const std::string& hair_iff, const std::string& hair_customization, const std::string& iff_template)
{
	ScopedGilLock lock;
    try 
    {		
		auto start_mod = bp::import("character_create.create_starting_character");
		if (!start_mod.is_none())
		{
			auto createFunc = start_mod.attr("CreateStartingCharacter");
			// kernel, scale, profession, base_model, appearance_customization, hair_model, hair_customization, start_city
			std::vector<int> char_customization(customization.begin(), customization.end());
			std::vector<int> hair_custom(hair_customization.begin(), hair_customization.end());
			auto return_creature = createFunc(bp::ptr(kernel_), height, iff_template, char_customization, full_name, profession, hair_iff, hair_custom, location);			
			swganh::object::Creature* created_creature = bp::extract<swganh::object::Creature*>(return_creature);
			auto simulation = kernel_->GetServiceManager()->GetService<swganh::simulation::SimulationService>("SimulationService");
			simulation->PersistRelatedObjects(created_creature->GetObjectId(), true);
			return created_creature->GetObjectId();
		}
	}
	catch(bp::error_already_set& /*e*/)
    {
        PyErr_Print();
    }
	return 0;
}
