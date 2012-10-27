
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "character_create.h"

#include <boost/python.hpp>

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

bool PyCharacterCreate::CreateCharacter(
	uint64_t character_id, const std::wstring& full_name, const std::string& profession, 
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
			// kernel, id, scale, profession, base_model, appearance_customization, hair_model, hair_customization, start_city
			auto return_creature = createFunc(bp::ptr(kernel_), character_id, height, iff_template, full_name, profession, hair_iff, location);			
			swganh::object::Creature* created_creature = bp::extract<swganh::object::Creature*>(return_creature);

			// Temp
			created_creature->SetCustomization(customization);
			auto simulation = kernel_->GetServiceManager()->GetService<swganh::simulation::SimulationService>("SimulationService");
			simulation->PersistObject(created_creature->GetObjectId());
			// Set customization, etc...
			return true;
		}
	}
	catch(bp::error_already_set& /*e*/)
    {
        PyErr_Print();
    }
	return false;
}
