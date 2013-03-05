import swgpy
from swgpy.object import *
from swgpy.attributes import *
from attributes.AttributeHelpers import SetOptionalAttribute, RAW, PERCENT, PERCENT_DASH
class ArmorAttributeTemplate(BaseAttributeTemplate):
	def buildAttributeTemplate(self, object):
		attribute_list_message = AttributeListMessage()
		attribute_list_message.object_id = object.id
		attributes = AttributeList()
		if len(object.custom_name) > 0:
			attributes.append("@obj_attr_n:original_name", "@" + object.stf_file + ":" + object.stf_name)
		attributes.extend([
			Attribute("@obj_attr_n:condition", str(object.max_condition-object.condition_damage) + "/" + str(object.max_condition)),
			Attribute("@obj_attr_n:volume", str(object.volume)),
			Attribute("@obj_attr_n:armor_rating", object.getStringAttribute("armor_rating"))
		])
		SetOptionalAttribute(attributes, '@obj_attr_n:sockets', 'sockets', object, True)
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_kinetic", "cat_armor_special_protection.armor_eff_kinetic", object, True, PERCENT);
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_energy", "cat_armor_special_protection.armor_eff_energy", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_blast", "cat_armor_special_protection.armor_eff_blast", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_stun", "cat_armor_special_protection.armor_eff_stun", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_elemental_heat", "cat_armor_special_protection.armor_eff_elemental_heat", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_elemental_cold", "cat_armor_special_protection.armor_eff_elemental_cold", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_elemental_acid", "cat_armor_special_protection.armor_eff_elemental_acid", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_elemental_electrical", "cat_armor_special_protection.armor_eff_elemental_electrical", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_restraint", "cat_armor_special_protection.armor_eff_restraint", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_kinetic", "cat_armor_effectiveness.armor_eff_restraint", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_energy", "cat_armor_effectiveness.armor_eff_energy", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_blast", "cat_armor_effectiveness.armor_eff_blast", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_stun", "cat_armor_effectiveness.armor_eff_stun", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_elemental_heat", "cat_armor_effectiveness.armor_eff_elemental_heat", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_elemental_cold", "cat_armor_effectiveness.armor_eff_elemental_cold", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_elemental_acid", "cat_armor_effectiveness.armor_eff_elemental_acid", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_elemental_electrical", "cat_armor_effectiveness.armor_eff_elemental_electrical", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_restraint", "cat_armor_effectiveness.armor_eff_restraint", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_kinetic", "cat_armor_vulnerability.armor_eff_kinetic", object, True, PERCENT_DASH);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_energy", "cat_armor_vulnerability.armor_eff_energy", object, True, PERCENT_DASH);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_blast", "cat_armor_vulnerability.armor_eff_blast", object, True, PERCENT_DASH);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_stun", "cat_armor_vulnerability.armor_eff_stun", object, True, PERCENT_DASH);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_elemental_heat", "cat_armor_vulnerability.armor_eff_elemental_heat", object, True, PERCENT_DASH);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_elemental_cold", "cat_armor_vulnerability.armor_eff_elemental_cold", object, True, PERCENT_DASH);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_elemental_acid", "cat_armor_vulnerability.armor_eff_elemental_acid", object, True, PERCENT_DASH);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_elemental_electrical", "cat_armor_vulnerability.armor_eff_elemental_electrical", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_restraint", "cat_armor_vulnerability.armor_eff_restraint", object, True, PERCENT);		
		SetOptionalAttribute(attributes, "cat_armor_encumbrance.@obj_attr_n:health", "cat_armor_encumbrance.armor_health_encumbrance", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_encumbrance.@obj_attr_n:action", "cat_armor_encumbrance.armor_action_encumbrance", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_encumbrance.@obj_attr_n:mind", "cat_armor_encumbrance.armor_mind_encumbrance", object, True);		
		SetOptionalAttribute(attributes, '@obj_attr_n:crafter', 'crafter', object, False)
		SetOptionalAttribute(attributes, '@obj_attr_n:serial_number', 'serial_number', object, False)			
		attribute_list_message.attributes = attributes
		return attribute_list_message