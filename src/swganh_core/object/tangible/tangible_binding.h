// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_TANGIBLE_TANGIBLE_BINDING_H_
#define SWGANH_OBJECT_TANGIBLE_TANGIBLE_BINDING_H_

#include "swganh_core/object/object_binding.h"
#include "swganh_core/object/tangible/tangible.h"
#include "swganh_core/object/building/building.h"
#include "swganh_core/object/factory_crate/factory_crate.h"
#include "swganh_core/object/weapon/weapon.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;

void exportTangible()
{
	class_<Tangible, bases<swganh::object::Object>, std::shared_ptr<Tangible>, boost::noncopyable>("Tangible")
		.add_property("customization", &Tangible::GetCustomization, &Tangible::SetCustomization, "Gets and Sets the tangible customization")
		.def("setCustomizationFromInts", &Tangible::SetCustomizationFromInts)
		//.def("customization.__add__", &Tangible::AddCustomization, "Adds a string to the existing customization")
        //.def("componentCustomization", &Tangible::GetComponentCustomization, return_value_policy<copy_const_reference>(),"Gets the current component_customization")
        .def("addComponentCustomization", &Tangible::AddComponentCustomization, "Adds a string to the component_customization")
        .def("removeComponentCustomization", &Tangible::RemoveComponentCustomization, "Removes the specified component_customization")
		//.def("component_customization.__add__", &Tangible::AddComponentCustomization, "Adds a crc to the component customization list")
		//.def("component_customization.__sub__", &Tangible::RemoveComponentCustomization, "Removes crc from the component customization list")
		.def("clearComponentCustomization", &Tangible::ClearComponentCustomization, "Clears all crc's from the component customization list")
		.add_property("options_mask", &Tangible::GetOptionsMask, &Tangible::SetOptionsMask, "Gets and Sets the options mask")
		.def("toggleOption", &Tangible::ToggleOption, "Toggles the specified option")
		.add_property("counter", &Tangible::GetCounter, &Tangible::SetCounter, "Gets and Sets the counter of the tangible object")
		.add_property("condition_damage", &Tangible::GetCondition, &Tangible::SetConditionDamage, "Gets and Sets the current condition of the tangible")
		.add_property("max_condition", &Tangible::GetMaxCondition, &Tangible::SetMaxCondition, "Gets and Sets the max condition of the tangible")
		.add_property("static", &Tangible::IsStatic, &Tangible::SetStatic, "Gets and Sets the static property")
		.def("addDefender", &Tangible::AddDefender, "Adds a defender to the list")
		.def("removeDefender", &Tangible::RemoveDefender, "Removes a defender from the list")
		.def("defended", &Tangible::IsDefending, "Checks if the object is being defended")
		.def("resetDefenders", &Tangible::ResetDefenders, "Resets the defenders to the specified tangible")
		.def("clearDefenders", &Tangible::ClearDefenders, "Clears all defenders from the tangible object")
        .def("activateAutoAttack", &Tangible::ActivateAutoAttack, "Activates auto attack for the tangible object")
        .def("clearAutoAttack", &Tangible::ClearAutoAttack, "Clears the auto attack flag")
        .def("isAutoAttacking", &Tangible::IsAutoAttacking, "returns true if currently auto attacking")
		;

	class_<Building, bases<Tangible>, std::shared_ptr<Building>, boost::noncopyable>("Building");
	class_<FactoryCrate, bases<Tangible>, std::shared_ptr<FactoryCrate>, boost::noncopyable>("FactoryCrate");

	implicitly_convertible<std::shared_ptr<FactoryCrate>, std::shared_ptr<Tangible>>();
	implicitly_convertible<std::shared_ptr<Building>, std::shared_ptr<Tangible>>();
	implicitly_convertible<std::shared_ptr<Tangible>, std::shared_ptr<Object>>();
	implicitly_convertible<std::shared_ptr<Tangible>, std::shared_ptr<ContainerInterface>>();
}
void exportWeapon()
{
	class_<Weapon, bases<Tangible>, std::shared_ptr<Weapon>, boost::noncopyable>("Weapon")
		.add_property("weapon_type", &Weapon::GetWeaponType)
		;
	implicitly_convertible<std::shared_ptr<Weapon>, std::shared_ptr<Tangible>>();
}

#endif // SWGANH_OBJECT_TANGIBLE_TANGIBLE_BINDING_H_
