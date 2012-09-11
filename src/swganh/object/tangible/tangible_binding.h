// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_TANGIBLE_TANGIBLE_BINDING_H_
#define SWGANH_OBJECT_TANGIBLE_TANGIBLE_BINDING_H_

#include "swganh/object/object_binding.h"
#include "swganh_core/object/tangible/tangible.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;

void exportTangible()
{
	class_<Tangible, bases<swganh::object::Object>, std::shared_ptr<Tangible>, boost::noncopyable>("Tangible")
		.add_property("customization", &Tangible::GetCustomization, &Tangible::SetCustomization, "Gets and Sets the tangible customization")
		//.def("customization.__add__", &Tangible::AddCustomization, "Adds a string to the existing customization")
        .def("component_customization", &Tangible::GetComponentCustomization, "Gets the current component_customization")
        .def("add_component_customization", &Tangible::AddComponentCustomization, "Adds a string to the component_customization")
        .def("remove_component_customization", &Tangible::RemoveComponentCustomization, "Removes the specified component_customization")
		//.def("component_customization.__add__", &Tangible::AddComponentCustomization, "Adds a crc to the component customization list")
		//.def("component_customization.__sub__", &Tangible::RemoveComponentCustomization, "Removes crc from the component customization list")
		.def("clear_component_customization", &Tangible::ClearComponentCustomization, "Clears all crc's from the component customization list")
		.add_property("options_mask", &Tangible::GetOptionsMask, &Tangible::SetOptionsMask, "Gets and Sets the options mask")
		.def("toggle_option", &Tangible::ToggleOption, "Toggles the specified option")
		.add_property("incap_timer", &Tangible::GetIncapTimer, &Tangible::SetIncapTimer, "Gets and Sets the incapacitation timer of the tangible object")
		.add_property("condition_damage", &Tangible::GetCondition, &Tangible::SetConditionDamage, "Gets and Sets the current condition of the tangible")
		.add_property("max_condition", &Tangible::GetMaxCondition, &Tangible::SetMaxCondition, "Gets and Sets the max condition of the tangible")
		.add_property("static", &Tangible::IsStatic, &Tangible::SetStatic, "Gets and Sets the static property")
		.def("add_defender", &Tangible::AddDefender, "Adds a defender to the list")
		.def("remove_defender", &Tangible::RemoveDefender, "Removes a defender from the list")
		.def("defended", &Tangible::IsDefending, "Checks if the object is being defended")
		.def("reset_defenders", &Tangible::ResetDefenders, "Resets the defenders to the specified tangible")
		.def("clear_defenders", &Tangible::ClearDefenders, "Clears all defenders from the tangible object")
        .def("activate_auto_attack", &Tangible::ActivateAutoAttack, "Activates auto attack for the tangible object")
        .def("clear_auto_attack", &Tangible::ClearAutoAttack, "Clears the auto attack flag")
        .def("is_auto_attacking", &Tangible::IsAutoAttacking, "returns true if currently auto attacking")
		;

	implicitly_convertible<std::shared_ptr<Tangible>, std::shared_ptr<Object>>();
	implicitly_convertible<std::shared_ptr<Tangible>, std::shared_ptr<ContainerInterface>>();
}

#endif // SWGANH_OBJECT_TANGIBLE_TANGIBLE_BINDING_H_
