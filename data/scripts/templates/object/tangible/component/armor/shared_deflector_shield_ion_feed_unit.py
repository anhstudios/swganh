#### NOTICE: THIS FILE IS AUTOGENERATED
#### MODIFICATIONS MAY BE LOST IF DONE IMPROPERLY
#### PLEASE SEE THE ONLINE DOCUMENTATION FOR EXAMPLES

from swgpy.object import *

class Template(BaseTemplate):
	name = "object/tangible/component/armor/shared_deflector_shield_ion_feed_unit.iff"
	is_prototype = False
	
	def create(self, kernel, params):
		result = Tangible()
	
		result.template = "object/tangible/component/armor/shared_deflector_shield_ion_feed_unit.iff"
		result.attribute_template_id = -1
		result.stfName("craft_clothing_ingredients_n","deflector_shield_ion_feed_unit")		
		
		#### BEGIN MODIFICATIONS ####
		####  END MODIFICATIONS  ####
		
		return result

def loadTemplates(addTemplate):
	addTemplate(Template())