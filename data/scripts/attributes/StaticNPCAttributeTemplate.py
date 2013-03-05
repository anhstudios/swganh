import swgpy
from swgpy.object import *
from swgpy.attributes import *
from attributes.AttributeHelpers import SetOptionalAttribute
class StaticNPCAttributeTemplate(BaseAttributeTemplate):
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
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_kinetic", "cat_armor_special_protection.armor_eff_kinetic", object, True);
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_energy", "cat_armor_special_protection.armor_eff_energy", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_blast", "cat_armor_special_protection.armor_eff_blast", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_stun", "cat_armor_special_protection.armor_eff_stun", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_elemental_heat", "cat_armor_special_protection.armor_eff_elemental_heat", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_elemental_cold", "cat_armor_special_protection.armor_eff_elemental_cold", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_elemental_acid", "cat_armor_special_protection.armor_eff_elemental_acid", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_elemental_electrical", "cat_armor_special_protection.armor_eff_elemental_electrical", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_special_protection.@obj_attr_n:armor_eff_restraint", "cat_armor_special_protection.armor_eff_restraint", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_kinetic", "cat_armor_effectiveness.armor_eff_restraint", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_energy", "cat_armor_effectiveness.armor_eff_energy", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_blast", "cat_armor_effectiveness.armor_eff_blast", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_stun", "cat_armor_effectiveness.armor_eff_stun", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_elemental_heat", "cat_armor_effectiveness.armor_eff_elemental_heat", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_elemental_cold", "cat_armor_effectiveness.armor_eff_elemental_cold", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_elemental_acid", "cat_armor_effectiveness.armor_eff_elemental_acid", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_elemental_electrical", "cat_armor_effectiveness.armor_eff_elemental_electrical", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_effectiveness.@obj_attr_n:armor_eff_restraint", "cat_armor_effectiveness.armor_eff_restraint", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_kinetic", "cat_armor_vulnerability.armor_eff_kinetic", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_energy", "cat_armor_vulnerability.armor_eff_energy", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_blast", "cat_armor_vulnerability.armor_eff_blast", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_stun", "cat_armor_vulnerability.armor_eff_stun", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_elemental_heat", "cat_armor_vulnerability.armor_eff_elemental_heat", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_elemental_cold", "cat_armor_vulnerability.armor_eff_elemental_cold", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_elemental_acid", "cat_armor_vulnerability.armor_eff_elemental_acid", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_elemental_electrical", "cat_armor_vulnerability.armor_eff_elemental_electrical", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_vulnerability.@obj_attr_n:armor_eff_restraint", "cat_armor_vulnerability.armor_eff_restraint", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_encumbrance.@obj_attr_n:armor_health_encumbrance", "cat_armor_encumbrance.armor_health_encumbrance", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_encumbrance.@obj_attr_n:armor_action_encumbrance", "cat_armor_encumbrance.armor_action_encumbrance", object, True);		
		SetOptionalAttribute(attributes, "cat_armor_encumbrance.@obj_attr_n:armor_mind_encumbrance", "cat_armor_encumbrance.armor_mind_encumbrance", object, True);		
		attribute_list_message.attributes = attributes
		return attribute_list_message