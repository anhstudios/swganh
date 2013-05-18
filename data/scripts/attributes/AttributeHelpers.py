import swgpy
from swgpy.object import *
from swgpy.attributes import *

def RAW(input):
	return input

def PERCENT(input):
	return ('%.1f' % float(input)) + "%"
		
def PERCENT_DASH(input):
	output = PERCENT(input)
	if output == '0.0%':
		return '-';
	else:
		return output
		
def SetOptionalAttribute(attribute_list, attribute_name, attribute_label, object, is_recursive, func=RAW):
	attr_val = ""
	if object.hasAttribute(attribute_label):
		attr_val = object.getStringAttribute(attribute_label)
			
	attr_val = func(attr_val)
	
	if attr_val != '' and attr_val != '0.0%':
		attribute_list.append(Attribute(attribute_name, attr_val))