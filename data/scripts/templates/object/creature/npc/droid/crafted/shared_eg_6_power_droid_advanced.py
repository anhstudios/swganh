#### NOTICE: THIS FILE IS AUTOGENERATED
#### MODIFICATIONS MAY BE LOST IF DONE IMPROPERLY
#### PLEASE SEE THE ONLINE DOCUMENTATION FOR EXAMPLES

from swgpy.object import *

class Template(BaseTemplate):
	name = "object/creature/npc/droid/crafted/shared_eg_6_power_droid_advanced.iff"
	is_prototype = False
	
	def create(self, kernel, params):
		result = Creature()
	
		result.template = "object/creature/npc/droid/crafted/shared_eg_6_power_droid_advanced.iff"
		result.attribute_template_id = 3
		result.stfName("droid_name","eg_6_power_droid_crafted_advanced")		
		
		#### BEGIN MODIFICATIONS ####
		####  END MODIFICATIONS  ####
		
		return result

def loadTemplates(addTemplate):
	addTemplate(Template())