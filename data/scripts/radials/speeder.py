import swgpy
from swgpy.object import *
from swgpy.sui import RadialMenu, RadialOptions, RadialOptionsList, RadialIdentifier
from swgpy.utility import vector3, quat

class PyRadialMenu(RadialMenu):
	def buildRadial(self, owner, target, radials):
		radial_list = RadialOptionsList()
		
		#Extend existing radials
		if(radials):
			radial_list.extend(radials)
		
		radial_list.append(RadialOptions(0, RadialIdentifier.serverVehicleExit, 3, '@pet/pet_menu:menu_enter_exit'))
		radial_list.append(RadialOptions(0, RadialIdentifier.serverVehicleEnter, 3, '@pet/pet_menu:menu_enter_exit'))
		radial_list.append(RadialOptions(0, RadialIdentifier.vehicleStore, 3, '@pet/pet_menu:menu_store')
		
	def handleRadial(self, owner, target, action):
		if action == RadialIdentifier.serverVehicleExit:
			pass
		elif action == RadialIdentifier.serverVehicleEnter:
			pass
		elif action == RadialIdentifier.vehicleStore:
			pass
