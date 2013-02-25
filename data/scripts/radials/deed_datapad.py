import swgpy
from swgpy.object import *
from swgpy.sui import RadialMenu, RadialOptions, RadialOptionsList, RadialIdentifier
from swgpy.utility import vector3, quat

class PyRadialMenu(RadialMenu):
	def buildRadial(self, owner, target, radials):
		radial_list = RadialOptionsList()
		radial_list.append(RadialOptions(0, RadialIdentifier.examine, 1, ''))
		radial_list.append(RadialOptions(0, RadialIdentifier.itemDestroy, 1, ''))
		radial_list.append(RadialOptions(0, RadialIdentifier.serverMenu1, 3, '@ui_radial:item_use_deed'))
		return radial_list
		
	def handleRadial(self, owner, target, action):
		if action == RadialIdentifier.serverMenu1:
			sim = self.getKernel().serviceManager().simulationService()
			datapad = self.getKernel().serviceManager().equipmentService().getEquippedObject(owner, "datapad")
			result = sim.createObject(target.getStringAttribute("deed_result"))
			if result is not None:
				datapad.add(owner, result)
			
			#Destroy the deed
			target.container().remove(owner, target)