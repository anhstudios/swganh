/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_OBJECT_TANGIBLE_TANGIBLE_BINDING_H_
#define SWGANH_OBJECT_TANGIBLE_TANGIBLE_BINDING_H_

#include "swganh/object/object_binding.h"
#include "swganh/object/tangible/tangible.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object::tangible;

namespace swganh { namespace object { namespace tangible {
struct TangibleWrapper : Tangible,  wrapper<Tangible>
{
};
}}}

void exportTangible()
{
	class_<TangibleWrapper, bases<swganh::object::Object>, std::shared_ptr<Tangible>, boost::noncopyable>("Tangible")
		.add_property("customization", &TangibleWrapper::GetCustomization, &TangibleWrapper::SetCustomization, "Gets and Sets the tangible customization")
		//.def("customization.__add__", &TangibleWrapper::AddCustomization, "Adds a string to the existing customization")
        .def("component_customization", &TangibleWrapper::GetComponentCustomization, "Gets the current component_customization")
        .def("add_component_customization", &TangibleWrapper::AddComponentCustomization, "Adds a string to the component_customization")
        .def("remove_component_customization", &TangibleWrapper::RemoveComponentCustomization, "Removes the specified component_customization")
		//.def("component_customization.__add__", &TangibleWrapper::AddComponentCustomization, "Adds a crc to the component customization list")
		//.def("component_customization.__sub__", &TangibleWrapper::RemoveComponentCustomization, "Removes crc from the component customization list")
		.def("clear_component_customization", &TangibleWrapper::ClearComponentCustomization, "Clears all crc's from the component customization list")
		.add_property("options_mask", &TangibleWrapper::GetOptionsMask, &TangibleWrapper::SetOptionsMask, "Gets and Sets the options mask")
		.def("toggle_option", &TangibleWrapper::ToggleOption, "Toggles the specified option")
		.add_property("incap_timer", &TangibleWrapper::GetIncapTimer, &TangibleWrapper::SetIncapTimer, "Gets and Sets the incapacitation timer of the tangible object")
		.add_property("condition", &TangibleWrapper::GetCondition, &TangibleWrapper::SetConditionDamage, "Gets and Sets the current condition of the tangible")
		.add_property("max_condition", &TangibleWrapper::GetMaxCondition, &TangibleWrapper::SetMaxCondition, "Gets and Sets the max condition of the tangible")
		.add_property("static", &TangibleWrapper::IsStatic, &TangibleWrapper::SetStatic, "Gets and Sets the static property")
		.def("add_defender", &TangibleWrapper::AddDefender, "Adds a defender to the list")
		.def("remove_defender", &TangibleWrapper::RemoveDefender, "Removes a defender from the list")
		.def("defended", &TangibleWrapper::IsDefending, "Checks if the object is being defended")
		.def("reset_defenders", &TangibleWrapper::ResetDefenders, "Resets the defenders to the specified tangible")
		.def("clear_defenders", &TangibleWrapper::ClearDefenders, "Clears all defenders from the tangible object")
        .def("activate_auto_attack", &TangibleWrapper::ActivateAutoAttack, "Activates auto attack for the tangible object")
        .def("clear_auto_attack", &TangibleWrapper::ClearAutoAttack, "Clears the auto attack flag")
        .def("is_auto_attacking", &TangibleWrapper::IsAutoAttacking, "returns true if currently auto attacking")
		;
}

#endif // SWGANH_OBJECT_TANGIBLE_TANGIBLE_BINDING_H_
