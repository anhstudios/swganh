import swgpy
from swgpy.object import *
from swgpy.attributes import *
from attributes.AttributeHelpers import SetOptionalAttribute
class TicketAttributeTemplate(BaseAttributeTemplate):
	def buildAttributeTemplate(self, object):
		attribute_list_message = AttributeListMessage()
		attribute_list_message.object_id = object.id
		attributes = AttributeList()
		SetOptionalAttribute(attributes, '@obj_attr_n:travel_departure_planet', 'travel_departure_planet', object, False)
		SetOptionalAttribute(attributes, '@obj_attr_n:travel_departure_point', 'travel_departure_point', object, False)
		SetOptionalAttribute(attributes, '@obj_attr_n:travel_arrival_planet', 'travel_arrival_planet', object, False)
		SetOptionalAttribute(attributes, '@obj_attr_n:travel_arrival_point', 'travel_arrival_point', object, False)
		attribute_list_message.attributes = attributes
		return attribute_list_message
	    
