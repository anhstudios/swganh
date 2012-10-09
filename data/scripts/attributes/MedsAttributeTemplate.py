import swgpy
from swgpy.object import *
from swgpy.attributes import *
from attributes.AttributeHelpers import SetOptionalAttribute
class MedsAttributeTemplate(BaseAttributeTemplate):
	def buildAttributeTemplate(self, object):
		attribute_list_message = AttributeListMessage()
		attribute_list_message.object_id = object.id
		attributes = AttributeList()
		attributes.extend([
			Attribute("@obj_attr_n:original_name", object.getStringAttribute("original_name")),
			Attribute("@obj_attr_n:volume", str(object.volume)),
			Attribute("@obj_attr_n:counter_uses_remaining", object.getStringAttribute("counter_uses_remaining"))
		])
		SetOptionalAttribute(attributes, '@obj_attr_n:crafter', 'crafter', object, False)
		SetOptionalAttribute(attributes, '@obj_attr_n:serial_number', 'serial_number', object, False)			
		attributes.append(Attribute("@obj_attr_n:healing_ability", object.getStringAttribute("healing_ability")))
		attribute_list_message.attributes = attributes
		return attribute_list_message