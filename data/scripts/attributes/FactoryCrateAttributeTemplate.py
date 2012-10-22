import swgpy
from swgpy.object import *
from swgpy.attributes import *
from attributes.AttributeHelpers import SetOptionalAttribute
class FactoryCrateAttributeTemplate(BaseAttributeTemplate):
	def buildAttributeTemplate(self, object):
		attribute_list_message = AttributeListMessage()
		attribute_list_message.object_id = object.id
		attributes = AttributeList()
		attributes.extend([
			Attribute("@obj_attr_n:original_name", object.getStringAttribute("original_name")),
			Attribute("@obj_attr_n:volume", str(object.volume))
		])
		SetOptionalAttribute(attributes, '@obj_attr_n:crafter', 'crafter', object, False)
		SetOptionalAttribute(attributes, '@obj_attr_n:serial_number', 'serial_number', object, False)			
		attributes.extend([
			Attribute("@obj_attr_n:factory_count", object.getStringAttribute("factory_count")),
			Attribute("@obj_attr_n:factory_attribs", object.getStringAttribute("factory_attribs")),
			Attribute("@obj_attr_n:type", object.getStringAttribute("type"))
		])
		attribute_list_message.attributes = attributes
		return attribute_list_message