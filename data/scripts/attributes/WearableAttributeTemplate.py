import swgpy
from swgpy.object import *
from swgpy.attributes import *

class WearableAttributeTemplate(BaseAttributeTemplate):
	def buildAttributeTemplate(self, object):
		attribute_list_message = AttributeListMessage()
		attributes = AttributeList()
		attributes.extend([
			Attribute("@obj_attr_n:original_name", object.getStringAttribute("original_name")),
			Attribute("@obj_attr_n:condition", object.getStringAttribute("condition"))
		])
		attribute_list_message.attributes = attributes
		return attribute_list_message