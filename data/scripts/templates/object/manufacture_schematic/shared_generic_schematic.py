#### NOTICE: THIS FILE IS AUTOGENERATED
#### MODIFICATIONS MAY BE LOST IF DONE IMPROPERLY
#### PLEASE SEE THE ONLINE DOCUMENTATION FOR EXAMPLES

from swgpy.object import *

class Template(BaseTemplate):
	name = "object/manufacture_schematic/shared_generic_schematic.iff"
	is_prototype = False
	
	def create(self, kernel, params):
		result = ManufactureSchematic()
	
		result.template = "object/manufacture_schematic/shared_generic_schematic.iff"
		result.attribute_template_id = -1
		result.stfName("","")		
		
		#### BEGIN MODIFICATIONS ####
		####  END MODIFICATIONS  ####
		
		return result

def loadTemplates(addTemplate):
	addTemplate(Template())