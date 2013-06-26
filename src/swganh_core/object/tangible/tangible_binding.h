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
		.add_property("customization", (std::string(Tangible::*)()) &Tangible::GetCustomization, (void(Creature::*)(const std::string&)) &Tangible::SetCustomization, "Gets and Sets the tangible customization")
		.def("setCustomizationFromInts", (void(Tangible::*)(std::vector<int>)) &Tangible::SetCustomizationFromInts)
		.def("addComponentCustomization", (void(Tangible::*)(uint32_t)) &Tangible::AddComponentCustomization, "Adds a string to the component_customization")
        .def("removeComponentCustomization", (void(Tangible::*)(uint32_t)) &Tangible::RemoveComponentCustomization, "Removes the specified component_customization")
		.def("clearComponentCustomization", (void(Tangible::*)()) &Tangible::ClearComponentCustomization, "Clears all crc's from the component customization list")
		.add_property("options_mask", (uint32_t(Tangible::*)()) &Tangible::GetOptionsMask, (void(Tangible::*)(uint32_t)) &Tangible::SetOptionsMask, "Gets and Sets the options mask")
		.def("toggleOption", (void(Tangible::*)(uint32_t)) &Tangible::ToggleOption, "Toggles the specified option")
		.add_property("counter", (uint32_t(Tangible::*)()) &Tangible::GetCounter, (void(Tangible::*)(uint32_t)) &Tangible::SetCounter, "Gets and Sets the counter of the tangible object")
		.add_property("condition_damage", (uint32_t(Tangible::*)()) &Tangible::GetCondition, (void(Tangible::*)(uint32_t)) &Tangible::SetConditionDamage, "Gets and Sets the current condition of the tangible")
		.add_property("max_condition", (uint32_t(Tangible::*)()) &Tangible::GetMaxCondition, (void(Tangible::*)(uint32_t)) &Tangible::SetMaxCondition, "Gets and Sets the max condition of the tangible")
		.add_property("static", (bool(Tangible::*)()) &Tangible::IsStatic, (void(Tangible::*)(bool)) &Tangible::SetStatic, "Gets and Sets the static property")
		.def("addDefender", (void(Tangible::*)(uint64_t)) &Tangible::AddDefender, "Adds a defender to the list")
		.def("removeDefender", (void(Tangible::*)(uint64_t)) &Tangible::RemoveDefender, "Removes a defender from the list")
		.def("defended", (bool(Tangible::*)(uint64_t)) &Tangible::IsDefending, "Checks if the object is being defended")
		.def("resetDefenders", (void(Tangible::*)(std::vector<uint64_t>)) &Tangible::ResetDefenders, "Resets the defenders to the specified tangible")
		.def("clearDefenders", (void(Tangible::*)()) &Tangible::ClearDefenders, "Clears all defenders from the tangible object")
		.def("activateAutoAttack", (void(Tangible::*)()) &Tangible::ActivateAutoAttack, "Activates auto attack for the tangible object")
        .def("clearAutoAttack", (void(Tangible::*)()) &Tangible::ClearAutoAttack, "Clears the auto attack flag")
        .def("isAutoAttacking", (bool(Tangible::*)()) &Tangible::IsAutoAttacking, "returns true if currently auto attacking")
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
