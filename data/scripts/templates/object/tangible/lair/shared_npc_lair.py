#### NOTICE: THIS FILE IS AUTOGENERATED
#### MODIFICATIONS MAY BE LOST IF DONE IMPROPERLY
#### PLEASE SEE THE ONLINE DOCUMENTATION FOR EXAMPLES

from swgpy.object import *

class Template(BaseTemplate):
	name = "object/tangible/lair/shared_npc_lair.iff"
	is_prototype = False
	
	def create(self, kernel, params):
		result = Tangible()
	
		result.template = "object/tangible/lair/shared_npc_lair.iff"
		result.attribute_template_id = 9
		result.stfName("lair_n","base")		
		
		#### BEGIN MODIFICATIONS ####
		####  END MODIFICATIONS  ####
		
		return result

def loadTemplates(addTemplate):
	addTemplate(Template())