import swgpy
from swgpy.object import *
from swgpy.sui import RadialMenu, RadialOptions, RadialOptionsList, RadialIdentifier

class PyRadialMenu(RadialMenu):
	def buildRadial(self, owner, target, radials):
		radial_list = RadialOptionsList()
		if (radials):
			radial_list.extend(radials)
		return radial_list
		
	def handleRadial(self, owner, target, action):
		pass
		