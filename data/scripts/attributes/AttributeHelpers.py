import swgpy
from swgpy.object import *
from swgpy.attributes import *
def SetOptionalAttribute(attribute_list, attribute_name, attribute_label, object, is_recursive):
	if is_recursive:
		attr_val = object.getAttributeRecursive(attribute_label)
	else:
		attr_val = object.getStringAttribute(attribute_label)
		
	if len(attr_val) > 0:
		attribute_list.append(Attribute(attribute_name, attr_val))