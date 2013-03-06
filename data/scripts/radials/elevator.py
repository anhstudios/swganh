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
		
		#If the elevator can go up, then add that option
		if target.hasAttribute("elevator_can_go_up"):
			radial_list.append(RadialOptions(0, RadialIdentifier.elevatorUp, 3, '@elevator_text:up'))
		
		#If the elevator can go down, then add that option
		if target.hasAttribute("elevator_can_go_down"):
			radial_list.append(RadialOptions(0, RadialIdentifier.elevatorDown, 3, '@elevator_text:down'))	
		
		#return the list
		return radial_list
		
	def handleRadial(self, owner, target, action):
		if action == RadialIdentifier.elevatorUp and target.hasAttribute("elevator_can_go_up"):
			handleElevatorAction(self,target, owner, False, "elevator_rise")
		elif action == RadialIdentifier.elevatorDown and target.hasAttribute("elevator_can_go_down"):
			handleElevatorAction(self,target, owner, True, "elevator_descend")
			
def handleElevatorAction(self, target, owner, expected_elevator_action, effect_name):
	static_service = self.getKernel().serviceManager().staticService()
	simulation_service = self.getKernel().serviceManager().simulationService()
	for data in static_service.getElevatorDataForObject(target.id):
		if data.going_down == expected_elevator_action:
			#Move the player
			destination_cell = simulation_service.findObjectById(data.dst_cell)
			new_position = vector3(owner.position.x, data.dst_position.y, owner.position.z)
			owner.updatePosition(new_position, owner.orientation, destination_cell)
			SystemMessage.sendEffect(owner, effect_name, data.dst_orientation, data.dst_position)