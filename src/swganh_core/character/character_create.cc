
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

#include "character_create.h"

#include <boost/python.hpp>

#include "swganh/logger.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/service/service_manager.h"
#include "swganh/scripting/utilities.h"
#include "swganh/scripting/python_script.h"
#include "swganh_core/simulation/simulation_service.h"
#include "swganh_core/object/creature/creature.h"

namespace bp = boost::python;
using namespace swganh::character;
using namespace swganh::scripting;
using swganh::object::Creature;

PyCharacterCreate::PyCharacterCreate(swganh::app::SwganhKernel* kernel)
	: kernel_(kernel)
{
    simulation_ = kernel_->GetServiceManager()->GetService<swganh::simulation::SimulationService>("SimulationService");
}

uint64_t PyCharacterCreate::CreateCharacter(
	const std::wstring& full_name, const std::string& profession, 
	const std::string& location, float height, const std::string& bio, const std::string& customization, 
	const std::string& hair_iff, const std::string& hair_customization, const std::string& iff_template)
{
    uint64_t character_id = 0;
    std::vector<int> char_customization(customization.begin(), customization.end());
	std::vector<int> hair_custom(hair_customization.begin(), hair_customization.end());

    PythonScript script(kernel_->GetAppConfig().script_directory + "/character_create/create_starting_character.py");

    auto create_func = script.GetGlobal("CreateStartingCharacter");
    
    {
        ScopedGilLock lock;
        try
        {
            auto py_character = (*create_func)(bp::ptr(kernel_), height, iff_template, char_customization, full_name, profession, hair_iff, hair_custom, location);
            Creature* character = bp::extract<Creature*>(py_character);
            character_id = character->GetObjectId();

            simulation_->PersistRelatedObjects(character_id, true);
        }
        catch (bp::error_already_set&)
	    {
		    logPythonException();
	    }
    }

	return character_id;
}
