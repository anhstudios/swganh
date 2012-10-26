
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "py_character_create.h"

#include <boost/python.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh/scripting/utilities.h"

namespace bp = boost::python;
using namespace swganh::character;
using namespace swganh::scripting;

bool PyCharacterCreate::CreateCharacter(
	uint64_t character_id, const std::wstring& full_name, const std::string& profession, 
	const std::string& location, float height, const std::string& bio, const std::string& customization, 
	const std::string& hair_iff, const std::string& hair_customization, const std::string& iff_template)
{
	ScopedGilLock lock;
    try 
    {
		auto start_mod = bp::import("templates.create_starting_character");
		if (!start_mod.is_none())
		{
			auto createFunc = start_mod.attr("CreateStartingCharacter");
			// kernel, id, scale, profession, base_model, appearance_customization, hair_model, hair_customization, start_city
			auto return_creature = createFunc(bp::ptr(kernel_), character_id, height, iff_template, full_name, profession, bio, customization, hair_iff, hair_customization, location);			
			return true;
		}
	}
	catch(bp::error_already_set& /*e*/)
    {
        PyErr_Print();
    }
	return false;
}
