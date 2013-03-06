import swgpy
from swgpy.object import *
from swgpy.attributes import *
from attributes.AttributeHelpers import SetOptionalAttribute
class WeaponAttributeTemplate(BaseAttributeTemplate):
	def buildAttributeTemplate(self, object):
		attribute_list_message = AttributeListMessage()
		attribute_list_message.object_id = object.id
		attributes = AttributeList()
		if len(object.custom_name) > 0:
			attributes.append("@obj_attr_n:original_name", "@" + object.stf_file + ":" + object.stf_name)
		attributes.extend([
			Attribute("@obj_attr_n:weapon_cert_status", object.getStringAttribute("weapon_cert_status")),
			Attribute("@obj_attr_n:condition", str(object.max_condition-object.condition_damage) + "/" + str(object.max_condition)),
			Attribute("@obj_attr_n:volume", str(object.volume))
		])
		SetOptionalAttribute(attributes, "wpn_armor_pierce_rating", "wpn_armor_pierce_rating", object, True)
		SetOptionalAttribute(attributes, "wpn_attack_speed", "wpn_attack_speed", object, True)
		SetOptionalAttribute(attributes, "cat_wpn_damage.@obj_attr_n:cat_wpn_damage", "cat_wpn_damage.wpn_damage_type", object, True)
		SetOptionalAttribute(attributes, "cat_wpn_damage.@obj_attr_n:wpn_damage_min", "cat_wpn_damage.wpn_damage_min", object, True)
		SetOptionalAttribute(attributes, "cat_wpn_damage.@obj_attr_n:wpn_damage_max", "cat_wpn_damage.wpn_damage_max", object, True)
		SetOptionalAttribute(attributes, "cat_wpn_damage.@obj_attr_n:wpn_wound_chance", "cat_wpn_damage.wpn_wound_chance", object, True)
		SetOptionalAttribute(attributes, "cat_wpn_rangemods.@obj_attr_n:wpn_range_zero", "cat_wpn_rangemods.wpn_range_zero", object, True)
		SetOptionalAttribute(attributes, "cat_wpn_rangemods.@obj_attr_n:wpn_range_mid", "cat_wpn_rangemods.wpn_range_mid", object, True)
		SetOptionalAttribute(attributes, "cat_wpn_rangemods.@obj_attr_n:wpn_range_max", "cat_wpn_rangemods.wpn_range_max", object, True)
		SetOptionalAttribute(attributes, "cat_wpn_attack_cost.@obj_attr_n:health", "cat_wpn_attack_cost.wpn_attack_cost_health", object, True)
		SetOptionalAttribute(attributes, "cat_wpn_attack_cost.@obj_attr_n:action", "cat_wpn_attack_cost.wpn_attack_cost_action", object, True)
		SetOptionalAttribute(attributes, "cat_wpn_attack_cost.@obj_attr_n:mind", "cat_wpn_attack_cost.wpn_attack_cost_mind", object, True)
		
		attribute_list_message.attributes = attributes
		return attribute_list_message