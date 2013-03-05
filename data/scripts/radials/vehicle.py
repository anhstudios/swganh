import swgpy
from swgpy.object import *
from swgpy.sui import RadialMenu, RadialOptions, RadialOptionsList, RadialIdentifier
from swgpy.utility import vector3, quat

class PyRadialMenu(RadialMenu):
	def buildRadial(self, owner, target, radials):
		radial_list = RadialOptionsList()
		
		radial_list.append(RadialOptions(0, RadialIdentifier.examine, 1, ''))
		
		if owner.id == target.owner_id:
			if owner.container().id == target.id:
				radial_list.append(RadialOptions(0, RadialIdentifier.serverVehicleExit, 3, "@pet/pet_menu:menu_enter_exit"))
			else:
				radial_list.append(RadialOptions(0, RadialIdentifier.serverVehicleEnter, 3, "@pet/pet_menu:menu_enter_exit"))
			radial_list.append(RadialOptions(0, RadialIdentifier.vehicleStore, 3, "@pet/pet_menu:menu_store"))
		
		return radial_list
		
	def handleRadial(self, owner, target, action):
		if owner.id == target.owner_id:
			if action == RadialIdentifier.serverVehicleExit or action == RadialIdentifier.serverVehicleEnter:
				if owner.container().id == target.id and action == RadialIdentifier.serverVehicleExit:
					#Exit
					target.transfer(owner, owner,target.container(), target.position)
					target.toggleStateOff(ACTION.MOUNTED_CREATURE)
				elif action == RadialIdentifier.serverVehicleEnter:
					#Enter
					owner.container().transfer(owner, owner, target, vector3(0, 0, 0))
					target.toggleStateOn(ACTION.MOUNTED_CREATURE)
			elif action == RadialIdentifier.vehicleStore:
				sim = self.getKernel().serviceManager().simulationService()
				pcd = sim.findObjectById(target.getIntAttribute("pcd_id"))
				if pcd:
					if owner.container().id == target.id:
						target.transfer(owner, owner, target.container(), target.position)
					target.container().transfer(owner, target, pcd, vector3(0, 0, 0))
	