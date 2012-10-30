import swgpy
from swgpy.object import *
from swgpy.attributes import *
from attributes.AttributeHelpers import SetOptionalAttribute
class FoodAttributeTemplate(BaseAttributeTemplate):
	def buildAttributeTemplate(self, object):
		attribute_list_message = AttributeListMessage()
		attribute_list_message.object_id = object.id
		attributes = AttributeList()
		if len(object.custom_name) > 0:
			attributes.append("@obj_attr_n:original_name", "@" + object.stf_file + ":" + object.stf_name)
		attributes.extend([
			Attribute("@obj_attr_n:volume", str(object.volume)),
			Attribute("@obj_attr_n:counter_uses_remaining", str(object.counter))
		])
		SetOptionalAttribute(attributes, '@obj_attr_n:crafter', 'crafter', object, False)
		SetOptionalAttribute(attributes, '@obj_attr_n:serial_number', 'serial_number', object, False)			
		attribute_list_message.attributes = attributes
		return attribute_list_message