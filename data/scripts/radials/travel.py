import swgpy
from swgpy.object import *
from swgpy.player import *
from swgpy.sui import *
from swgpy.utility import vector3, quat

class PyRadialMenu(RadialMenu):
	def buildRadial(self, owner, target, radials):
		radial_list = RadialOptionsList()
		radial_list.append(RadialOptions(0, RadialIdentifier.itemUse, 3, ''))
		return radial_list
		
	def handleRadial(self, owner, target, action):
		travel_service = self.getKernel().serviceManager().travelService()
		travel_service.beginTicketTransaction(owner)
