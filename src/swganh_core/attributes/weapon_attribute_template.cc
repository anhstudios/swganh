// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "weapon_attribute_template.h"

#include <sstream>
#include "attributes_helper.h"
#include "swganh/event_dispatcher.h"
#include "swganh_core/object/tangible/tangible.h"

using namespace std;
using namespace swganh;
using namespace swganh::object;
using namespace swganh::messages;
using namespace swganh::attributes;

WeaponAttributeTemplate::WeaponAttributeTemplate(swganh::EventDispatcher* dispatcher)
	: BaseAttributeTemplate(dispatcher)
{
	RegisterEventHandlers();
}

swganh::messages::AttributeListMessage WeaponAttributeTemplate::BuildAttributeTemplate(shared_ptr<Object> object)
{
	uint64_t object_id = object->GetObjectId();
	if (HasAttributeListMessage(object_id))
		return object_attribute_list_messages_[object_id];

	swganh::messages::AttributeListMessage attribute_list_message;
	
	std::vector<Attribute> attribute_list;
	attribute_list_message.object_id = object->GetObjectId();

	attribute_list.push_back(Attribute("@obj_attr_n:original_name", object->GetAttributeAsString("original_name")));
    attribute_list.push_back(Attribute("@obj_attr_n:weapon_cert_status", object->GetAttributeAsString("weapon_cert_status")));
	attribute_list.push_back(Attribute("@obj_attr_n:condition", AttributesHelper::GetCondition(object)));
	attribute_list.push_back(Attribute("@obj_attr_n:volume", AttributesHelper::GetVolume(object)));
	AttributesHelper::SetOptionalAttribute(attribute_list, "@obj_attr_n:crafter", "crafter", object, false);
	AttributesHelper::SetOptionalAttribute(attribute_list, "@obj_attr_n:serial_number", "serial_number", object, false);
    AttributesHelper::SetOptionalAttribute(attribute_list, "@obj_attr_n:wpn_armor_pierce_rating", "wpn_armor_pierce_rating", object, true);
    AttributesHelper::SetOptionalAttribute(attribute_list, "@obj_attr_n:wpn_attack_speed", "wpn_attack_speed", object, true);
    AttributesHelper::SetOptionalAttribute(attribute_list, "cat_wpn_damage.@obj_attr_n:cat_wpn_damage", "cat_wpn_damage.wpn_damage_type", object, true);
    AttributesHelper::SetOptionalAttribute(attribute_list, "cat_wpn_damage.@obj_attr_n:wpn_damage_min", "cat_wpn_damage.wpn_damage_min", object, true);
    AttributesHelper::SetOptionalAttribute(attribute_list, "cat_wpn_damage.@obj_attr_n:wpn_damage_max", "cat_wpn_damage.wpn_damage_max", object, true);
    AttributesHelper::SetOptionalAttribute(attribute_list, "cat_wpn_damage.@obj_attr_n:wpn_wound_chance", "cat_wpn_damage.wpn_wound_chance", object, true);
    AttributesHelper::SetOptionalAttribute(attribute_list, "cat_wpn_rangemods.@obj_attr_n:wpn_range_zero", "cat_wpn_rangemods.wpn_range_zero", object, true);
    AttributesHelper::SetOptionalAttribute(attribute_list, "cat_wpn_rangemods.@obj_attr_n:wpn_range_mid", "cat_wpn_rangemods.wpn_range_mid", object, true);
    AttributesHelper::SetOptionalAttribute(attribute_list, "cat_wpn_rangemods.@obj_attr_n:wpn_range_max", "cat_wpn_rangemods.wpn_range_max", object, true);
    AttributesHelper::SetOptionalAttribute(attribute_list, "cat_wpn_attack_cost.@obj_attr_n:wpn_attack_cost_health", "cat_wpn_attack_cost.wpn_attack_cost_health", object, true);
    AttributesHelper::SetOptionalAttribute(attribute_list, "cat_wpn_attack_cost.@obj_attr_n:wpn_attack_cost_action", "cat_wpn_attack_cost.wpn_attack_cost_action", object, true);
    AttributesHelper::SetOptionalAttribute(attribute_list, "cat_wpn_attack_cost.@obj_attr_n:wpn_attack_cost_mind", "cat_wpn_attack_cost.wpn_attack_cost_mind", object, true);
	attribute_list_message.attributes = move(attribute_list);

	object_attribute_list_messages_[object_id] = move(attribute_list_message);
	return attribute_list_message;
}

