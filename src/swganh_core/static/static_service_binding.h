// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/scripting/python_shared_ptr.h"
#include "static_service_interface.h"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/indexing_suite.hpp>

#include "swganh_core/static/skill.h"
#include "swganh_core/static/skill_info.h"

#include "swganh/scripting/set_wrap.h"

using namespace swganh::statics;
using namespace boost::python;
using namespace std;


void exportStaticService()
{
	set_wrapper<std::string>::wrap("StringSet");

	class_<ElevatorData, shared_ptr<ElevatorData>, boost::noncopyable>("ElevatorData", "A type to represent elevator data")
		.def_readwrite("dst_cell", &ElevatorData::dst_cell)
		.def_readwrite("dst_orientation", &ElevatorData::dst_orientation)
		.def_readwrite("dst_position", &ElevatorData::dst_position)
		.def_readwrite("effect_id", &ElevatorData::effect_id)
		.def_readwrite("going_down", &ElevatorData::going_down)
		;

	class_<SkillInfo, shared_ptr<SkillInfo>, boost::noncopyable>("SkillInfo", "information about skills.")
		.def_readonly("isTitle", &SkillInfo::is_title)
		.def_readonly("isProfession", &SkillInfo::is_profession)
		.def_readonly("moneyRequired", &SkillInfo::money_required)
		.def_readonly("skillPointsConsumed", &SkillInfo::skill_points_consumed)
		.def_readonly("xpType",&SkillInfo::xp_type)
		.def_readonly("xpCost", &SkillInfo::xp_cost)
		.def_readonly("xpCap", &SkillInfo::xp_cap)
		.def_readonly("requiredSkills", &SkillInfo::required_skills_)
		.def_readonly("requiredSpecies", &SkillInfo::required_species_)
		.def_readonly("commands", &SkillInfo::commands_)
		;

    class_<StaticServiceInterface, shared_ptr<StaticServiceInterface>, boost::noncopyable>("StaticService", "The static service loads and holds data that never changes.", no_init)
		.def("getElevatorDataForObject", &StaticServiceInterface::GetElevatorDataForObject, "Returns elevator data for a particular terminal id.")
        .def("getSkillInfo", &StaticServiceInterface::GetSkillInfo, "Returns information about a particular skill.")
		;


}
