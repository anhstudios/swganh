#### NOTICE: THIS FILE IS AUTOGENERATED
#### MODIFICATIONS MAY BE LOST IF DONE IMPROPERLY
#### PLEASE SEE THE ONLINE DOCUMENTATION FOR EXAMPLES

from swgpy.object import *

class Template(BaseTemplate):
	name = "object/building/naboo/shared_ply_nboo_house_lg_s01_fp1.iff"
	is_prototype = False
	
	def create(self, kernel, params):
		result = Building()
	
		result.template = "object/building/naboo/shared_ply_nboo_house_lg_s01_fp1.iff"
		result.attribute_template_id = -1
		result.stfName("building_name","housing_naboo_style_1")		
		
		#### BEGIN MODIFICATIONS ####
		####  END MODIFICATIONS  ####
		
		return result

def loadTemplates(addTemplate):
	addTemplate(Template())