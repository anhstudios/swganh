import swgpy
from swgpy.object import *
from swgpy.player import *
from swgpy.sui import RadialMenu, RadialOptions, RadialOptionsList, RadialIdentifier
from swgpy.utility import vector3, quat

class PyRadialMenu(RadialMenu):
	def buildRadial(self, owner, target, radials):
		radial_list = RadialOptionsList()
		#Get bank object
		bank = self.getKernel().serviceManager().equipmentService().getEquippedObject(owner, "bank")
		if bank:
			radial_list.append(RadialOptions(0, RadialIdentifier.itemUse, 1, ''))
			radial_list.append(RadialOptions(1, RadialIdentifier.bankTransfer, 2, '@sui:bank_credits'))
			#Bank Radials
			if bank.scene_id == owner.scene_id:
				radial_list.append(RadialOptions(1, RadialIdentifier.bankItems, 3, '@sui:bank_items'))
				radial_list.append(RadialOptions(1, RadialIdentifier.bankQuit, 4, '@sui:bank_quit'))
				radial_list.append(RadialOptions(1, RadialIdentifier.bankWithdrawAll, 5, '@sui:bank_withdrawall'))
				radial_list.append(RadialOptions(1, RadialIdentifier.bankDepositAll, 6, '@sui:bank_depositall'))
			elif bank.scene_id == 0:
				radial_list.append(RadialOptions(1, RadialIdentifier.bankJoin, 3, '@sui:bank_join'))
				radial_list.append(RadialOptions(1, RadialIdentifier.bankWithdrawAll, 4, '@sui:bank_withdrawall'))
				radial_list.append(RadialOptions(1, RadialIdentifier.bankDepositAll, 5, '@sui:bank_depositall'))
			else:
				radial_list.append(RadialOptions(1, RadialIdentifier.bankWithdrawAll, 3, '@sui:bank_withdrawall'))
				radial_list.append(RadialOptions(1, RadialIdentifier.bankDepositAll, 4, '@sui:bank_depositall'))
		#return the list
		return radial_list
		
	def handleRadial(self, owner, target, action):
		player_service = self.getKernel().serviceManager().playerService()
		if player_service:			
			if action == RadialIdentifier.bankItems:			
				player_service.openBank(owner)
			elif action == RadialIdentifier.bankJoin:
				joinBank(self, owner)
			elif action == RadialIdentifier.bankQuit:
				quitBank(self, owner)
			#elif action == RadialIdentifier.bankWithdrawAll:
				#player_service.BankWithdrawAll(owner)
			#elif action == RadialIdentifier.bankDepositAll:
				#player_service.BankDepositAll(owner)
			
def joinBank(self, owner):
	bank = self.getKernel().serviceManager().equipmentService().getEquippedObject(owner, "bank")
	if bank:
		if bank.scene_id == owner.scene_id:
			SystemMessage.sendSystemMessage(owner, 'system_msg', 'already_member_of_bank', False, False)
		elif bank.scene_id == 0:
			bank.scene_id = owner.scene_id
			SystemMessage.sendSystemMessage(owner, 'system_msg', 'successfully_joined_bank', False, False)
		else:
			SystemMessage.sendSystemMessage(owner, 'system_msg', 'member_of_different_bank', False, False)
def quitBank(self, owner):
	bank = self.getKernel().serviceManager().equipmentService().getEquippedObject(owner, "bank")
	if bank:
		if bank.hasContainedObjects():
			bank.scene_id = 0
			SystemMessage.sendSystemMessage(owner, 'system_msg', 'successfully_quit_bank', False, False)		
		else:
			SystemMessage.sendSystemMessage(owner, 'system_msg', 'bank_not_empty', False, False)	