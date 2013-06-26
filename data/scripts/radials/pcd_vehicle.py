import swgpy
from swgpy.object import *
from swgpy.sui import RadialMenu, RadialOptions, RadialOptionsList, RadialIdentifier
from swgpy.utility import vector3, quat

class PyRadialMenu(RadialMenu):
	def buildRadial(self, owner, target, radials):
		radial_list = RadialOptionsList()
		
		if target.hasContainedObjects():
			radial_list.append(RadialOptions(0, RadialIdentifier.vehicleGenerate, 3, '@pet/pet_menu:menu_call'))
		else:
			radial_list.append(RadialOptions(0, RadialIdentifier.vehicleGenerate, 3, '@pet/pet_menu:menu_store'))
		
		radial_list.append(RadialOptions(0, RadialIdentifier.examine, 1, ''))
		radial_list.append(RadialOptions(0, RadialIdentifier.itemDestroy, 1, ''))
		
		return radial_list
		
	def handleRadial(self, owner, target, action):
		if action == RadialIdentifier.vehicleGenerate:
			sim = self.getKernel().serviceManager().simulationService()
			mobile = sim.findObjectById(target.getIntAttribute("mobile_id"))
			if mobile:
				if target.hasContainedObjects():
					#Call it!
					playServ = self.getKernel().serviceManager().playerService()
					playServ.storeAllCalledMounts(owner)
					pos = owner.position
					mobile.orientation = owner.orientation
					mobile.container().transfer(mobile, mobile, owner.container(), vector3(pos.x, pos.y-0.3, pos.z))
				else:
					#Store it!
					if owner.container().id == mobile.id:
						#Move the player out first
						mobile.transfer(owner, owner, mobile.container(), mobile.position)
					mobile.container().transfer(owner, mobile, target, vector3(0, 0, 0))
	
