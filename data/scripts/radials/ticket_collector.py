import sys
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
		self.openTicketSelectionWindow(owner, target)

	def openTicketSelectionWindow(self, owner, target):
		sui = self.getKernel().serviceManager().suiService()
		
		
		equipment = self.getKernel().serviceManager().equipmentService()
		inventory = equipment.getEquippedObject(owner, "inventory")
		travel = self.getKernel().serviceManager().travelService()
		opts = travel.getAvailableTickets(owner)
		options = EventResultList()
		for i in range(0, len(opts)):
			options.append(opts[i])
		window = sui.createListBox(ListBoxType.OK_CANCEL, 'Select Destination', 'Select Destination', options,  owner)
		
		useTicketCallback = PythonCallback(self, 'useTicket')
		
		results = ResultList()
		results.extend(['List.lstList:SelectedRow'])
		window.subscribeToEventCallback(0, '', InputTrigger.OK, results, useTicketCallback)
		window.subscribeToEventCallback(1, '', InputTrigger.CANCEL, results, useTicketCallback)
		sui.openSUIWindow(window)

	def useTicket(self, owner, event_id, results):
		if event_id == 0:
			selection = str(results[0])
			print(int(selection))
			travel = self.getKernel().serviceManager().travelService()
			ticket = travel.getAvailableInventoryTicketBySelection(owner, int(selection))
			travel.useTicket(owner, ticket)
		return True;
