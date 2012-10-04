import swgpy
from swgpy.object import *
from swgpy.sui import RadialMenu, RadialOptions, RadialOptionsList, RadialIdentifier
from swgpy.static import ElevatorData, ElevatorDataList
from swgpy.utility import vector3, quat

class PyRadialMenu(RadialMenu):
	def BuildRadial(self, owner, target, radials):
		radial_list = RadialOptionsList()
		
		#Extend existing radials
		if(radials):
			radial_list.extend(radials)
		
		#If the elevator can go up, then add that option
		if target.has_attribute("elevator_can_go_up"):
			radial_list.append(RadialOptions(0, RadialIdentifier.elevatorUp, 3, '@elevator_text:up'))
		
		#If the elevator can go down, then add that option
		if target.has_attribute("elevator_can_go_down"):
			radial_list.append(RadialOptions(0, RadialIdentifier.elevatorDown, 3, '@elevator_text:down'))	
		
		#return the list
		return radial_list
		
	def HandleRadial(self, owner, target, action):
		if action == RadialIdentifier.elevatorUp and target.has_attribute("elevator_can_go_up"):
			handleElevatorAction(self,target, owner, False)
		elif action == RadialIdentifier.elevatorDown and target.has_attribute("elevator_can_go_down"):
			handleElevatorAction(self,target, owner, True)
			
def handleElevatorAction(self, target, owner, expected_elevator_action):
	static_service = self.GetKernel().service_manager().static_service()
	simulation_service = self.GetKernel().service_manager().simulation_service()
	for data in static_service.GetElevatorDataForObject(target.id):
		if data.going_down == expected_elevator_action:
			#Move the player
			destination_cell = simulation_service.findObjectById(data.dst_cell)
			owner.UpdatePosition(destination_cell, data.dst_position, data.dst_orientation)
			
			#Perform Effect