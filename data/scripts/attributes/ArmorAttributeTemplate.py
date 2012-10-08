import swgpy
from swgpy.object import *
from swgpy.attributes import *

class ArmorAttributeTemplate(BaseAttributeTemplate):
	def buildAttributeTemplate(self, object):
		return AttributeListMessage()