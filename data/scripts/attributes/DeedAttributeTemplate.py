import swgpy
from swgpy.object import *
from swgpy.attributes import *
from attributes.AttributeHelpers import SetOptionalAttribute
class DeedAttributeTemplate(BaseAttributeTemplate):
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
		scenes = object.getStringAttribute('examine_scene').split(',')
		for scene in scenes:
			attributes.append(Attribute('@obj_attr_n:examine_scene', scene))
		attribute_list_message.attributes = attributes
		return attribute_list_message