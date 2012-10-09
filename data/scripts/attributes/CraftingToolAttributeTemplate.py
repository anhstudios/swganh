import swgpy
from swgpy.object import *
from swgpy.attributes import *
from attributes.AttributeHelpers import SetOptionalAttribute
class CraftingToolAttributeTemplate(BaseAttributeTemplate):
	def buildAttributeTemplate(self, object):
		attribute_list_message = AttributeListMessage()
		attribute_list_message.object_id = object.id
		attributes = AttributeList()
		attributes.extend([
			Attribute("@obj_attr_n:original_name", object.getStringAttribute("original_name")),
			Attribute("@obj_attr_n:condition", str(object.max_condition - object.condition_damage) + "/" + str(object.max_condition)),
			Attribute("@obj_attr_n:volume", str(object.volume))
			Attribute("@obj_attr_n:craft_tool_effectiveness", object.getStringAttribute("craft_tool_effectiveness")),
			Attribute("@obj_attr_n:craft_tool_status", object.getStringAttribute("craft_tool_status")),
		])
		SetOptionalAttribute(attributes, '@obj_attr_n:crafter', 'crafter', object, False)
		SetOptionalAttribute(attributes, '@obj_attr_n:serial_number', 'serial_number', object, False)			
		attribute_list_message.attributes = attributes
		return attribute_list_message