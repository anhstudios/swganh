import swgpy
from swgpy.simulation import ContainerPermission
from swgpy.sui import RadialMenu, RadialOptions, RadialOptionsList, RadialIdentifier

class PyRadialMenu(RadialMenu):
	def buildRadial(self, owner, target, radials):
		radial_list = RadialOptionsList()
		radial_list.append(RadialOptions(0, RadialIdentifier.itemUse, 3, ''))
		radial_list.append(RadialOptions(0, RadialIdentifier.examine, 1, ''))
		radial_list.append(RadialOptions(0, RadialIdentifier.itemDestroy, 1, ''))
		return radial_list
		
	def handleRadial(self, owner, target, action):
		if action == RadialIdentifier.itemUse:
			structure = self.getKernel().serviceManager().structureService()
		
			# TODO: CHECK PLANTARY RESTRICTIONS
			
			# TODO: CHECK NPC CITY BOUNDARIES

			# TODO: CHECK PLAYER CITY BOUNARARIES

			# TODO: CHECK REMAINING STRUCTURE SLOTS

			structure.enterStructurePlacementMode(owner, target.id, target.getStringAttribute("deed_building"))