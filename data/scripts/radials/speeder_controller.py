import swgpy
from swgpy.object import *
from swgpy.sui import RadialMenu, RadialOptions, RadialOptionsList, RadialIdentifier
from swgpy.static import ElevatorData, ElevatorDataList
from swgpy.utility import vector3, quat

class PyRadialMenu(RadialMenu):
	def buildRadial(self, owner, target, radials):
		radial_list = RadialOptionsList()
		
		#Extend existing radials
		if(radials):
			radial_list.extend(radials)
		
		radial_list.append(RadialOptions(0, RadialIdentifier.vehicleGenerate, 3, '@pet/pet_menu:menu_call'))
		
	def handleRadial(self, owner, target, action):
		pass
