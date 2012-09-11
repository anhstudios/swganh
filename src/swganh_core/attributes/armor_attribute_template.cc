// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "armor_attribute_template.h"
#include "attributes_helper.h"
#include <sstream>

#include "swganh/event_dispatcher.h"
#include "swganh_core/object/tangible/tangible.h"

using namespace std;
using namespace swganh;
using namespace swganh::object;
using namespace swganh::messages;
using namespace swganh::attributes;

ArmorAttributeTemplate::ArmorAttributeTemplate(swganh::EventDispatcher* dispatcher)
	: BaseAttributeTemplate(dispatcher)
{
	RegisterEventHandlers();
}

swganh::messages::AttributeListMessage ArmorAttributeTemplate::BuildAttributeTemplate(shared_ptr<Object> object)
{
	uint64_t object_id = object->GetObjectId();
	if (HasAttributeListMessage(object_id))
		return object_attribute_list_messages_[object_id];

	swganh::messages::AttributeListMessage attribute_list_message;
	
	std::vector<Attribute> attribute_list;
	attribute_list_message.object_id = object->GetObjectId();

	// condition is special
	auto tano = static_pointer_cast<Tangible>(object);
	std::wstring condition = object->GetAttributeAsString("condition");
	wstringstream ss;
	if (tano)
	{
		int32_t max_condition = tano->GetMaxCondition();
		ss << max_condition - tano->GetCondition() << L"/" << max_condition;
		condition = ss.str();
		object->SetAttribute<wstring>("condition", condition);
	}

	attribute_list.push_back(Attribute("@obj_attr_n:original_name", object->GetAttributeAsString("original_name")));
	attribute_list.push_back(Attribute("@obj_attr_n:condition", condition));
	attribute_list.push_back(Attribute("@obj_attr_n:volume", object->GetAttributeAsString("volume")));
	AttributesHelper::SetOptionalAttribute(attribute_list, "obj_attr_n:sockets", "sockets", object, true);
	attribute_list.push_back(Attribute("@obj_attr_n:armor_rating", object->GetAttributeAsString("armor_rating")));
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_special_protection.@obj_attr_n:armor_eff_kinetic", "cat_armor_special_protection.armor_eff_kinetic", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_special_protection.@obj_attr_n:armor_eff_energy", "cat_armor_special_protection.armor_eff_energy", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_special_protection.@obj_attr_n:armor_eff_blast", "cat_armor_special_protection.armor_eff_blast", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_special_protection.@obj_attr_n:armor_eff_stun", "cat_armor_special_protection.armor_eff_stun", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_special_protection.@obj_attr_n:armor_eff_elemental_heat", "cat_armor_special_protection.armor_eff_elemental_heat", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_special_protection.@obj_attr_n:armor_eff_elemental_cold", "cat_armor_special_protection.armor_eff_elemental_cold", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_special_protection.@obj_attr_n:armor_eff_elemental_acid", "cat_armor_special_protection.armor_eff_elemental_acid", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_special_protection.@obj_attr_n:armor_eff_elemental_electrical", "cat_armor_special_protection.armor_eff_elemental_electrical", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_special_protection.@obj_attr_n:armor_eff_restraint", "cat_armor_special_protection.armor_eff_restraint", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_effectiveness.@obj_attr_n:armor_eff_kinetic", "cat_armor_effectiveness.armor_eff_restraint", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_effectiveness.@obj_attr_n:armor_eff_energy", "cat_armor_effectiveness.armor_eff_energy", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_effectiveness.@obj_attr_n:armor_eff_blast", "cat_armor_effectiveness.armor_eff_blast", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_effectiveness.@obj_attr_n:armor_eff_stun", "cat_armor_effectiveness.armor_eff_stun", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_effectiveness.@obj_attr_n:armor_eff_elemental_heat", "cat_armor_effectiveness.armor_eff_elemental_heat", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_effectiveness.@obj_attr_n:armor_eff_elemental_cold", "cat_armor_effectiveness.armor_eff_elemental_cold", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_effectiveness.@obj_attr_n:armor_eff_elemental_acid", "cat_armor_effectiveness.armor_eff_elemental_acid", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_effectiveness.@obj_attr_n:armor_eff_elemental_electrical", "cat_armor_effectiveness.armor_eff_elemental_electrical", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_effectiveness.@obj_attr_n:armor_eff_restraint", "cat_armor_effectiveness.armor_eff_restraint", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_vulnerability.@obj_attr_n:armor_eff_kinetic", "cat_armor_vulnerability.armor_eff_kinetic", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_vulnerability.@obj_attr_n:armor_eff_energy", "cat_armor_vulnerability.armor_eff_energy", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_vulnerability.@obj_attr_n:armor_eff_blast", "cat_armor_vulnerability.armor_eff_blast", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_vulnerability.@obj_attr_n:armor_eff_stun", "cat_armor_vulnerability.armor_eff_stun", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_vulnerability.@obj_attr_n:armor_eff_elemental_heat", "cat_armor_vulnerability.armor_eff_elemental_heat", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_vulnerability.@obj_attr_n:armor_eff_elemental_cold", "cat_armor_vulnerability.armor_eff_elemental_cold", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_vulnerability.@obj_attr_n:armor_eff_elemental_acid", "cat_armor_vulnerability.armor_eff_elemental_acid", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_vulnerability.@obj_attr_n:armor_eff_elemental_electrical", "cat_armor_vulnerability.armor_eff_elemental_electrical", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_vulnerability.@obj_attr_n:armor_eff_restraint", "cat_armor_vulnerability.armor_eff_restraint", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_encumbrance.@obj_attr_n:armor_health_encumbrance", "cat_armor_encumbrance.armor_health_encumbrance", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_encumbrance.@obj_attr_n:armor_action_encumbrance", "cat_armor_encumbrance.armor_action_encumbrance", object, true);
	AttributesHelper::SetOptionalAttribute(attribute_list, "cat_armor_encumbrance.@obj_attr_n:armor_mind_encumbrance", "cat_armor_encumbrance.armor_mind_encumbrance", object, true);
	attribute_list.push_back(Attribute("@obj_attr_n:crafter", object->GetAttributeAsString("crafter")));
	attribute_list.push_back(Attribute("@obj_attr_n:serial_number", object->GetAttributeAsString("serial_number")));
	attribute_list_message.attributes = move(attribute_list);

	object_attribute_list_messages_[object_id] = move(attribute_list_message);
	return attribute_list_message;
}

