import swgpy
from swgpy.object import *
from swgpy.simulation import ContainerPermission
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
			pcd = sim.createObject(target.getStringAttribute("deed_pcd"), ContainerPermission.NO_VIEW)
			mobile = sim.createObject(target.getStringAttribute("deed_mobile"), ContainerPermission.RIDEABLE)
			
			if not (datapad is None or pcd is None or mobile is None):
				
				#Set Owner
				mobile.owner_id = owner.id
				pcd.owner_id = owner.id
				
				#Link the two pieces
				mobile.setIntAttribute("pcd_id", pcd.id)
				pcd.setIntAttribute("mobile_id", mobile.id)
				
				#Store any called vehicles
				self.getKernel().serviceManager().playerService().storeAllCalledMounts(owner)
				
				#Add the the pcd to the datapad, and mobile to world
				datapad.add(owner, pcd)
				
				pos = owner.position
				mobile.position = vector3(pos.x, pos.y-0.3, pos.z)
				mobile.orientation = owner.orientation
				owner.container().add(owner, mobile)
			
				#Destroy the deed
				target.container().remove(owner, target)
