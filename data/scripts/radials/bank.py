import swgpy
from swgpy.object import *
from swgpy.player import *
from swgpy.sui import *
from swgpy.utility import vector3, quat

class PyRadialMenu(RadialMenu):
	def buildRadial(self, owner, target, radials):
		radial_list = RadialOptionsList()
		#Get bank object
		bank = self.getKernel().serviceManager().equipmentService().getEquippedObject(owner, "bank")
		if bank:
			radial_list.append(RadialOptions(0, RadialIdentifier.itemUse, 1, ''))
			radial_list.append(RadialOptions(1, RadialIdentifier.bankTransfer, 3, '@sui:bank_credits'))
			#Bank Radials
			if bank.scene_id == owner.scene_id:
				radial_list.append(RadialOptions(1, RadialIdentifier.bankItems, 3, '@sui:bank_items'))				
				radial_list.append(RadialOptions(1, RadialIdentifier.bankQuit, 3, '@sui:bank_quit'))								
			elif bank.scene_id == 0:
				radial_list.append(RadialOptions(1, RadialIdentifier.bankJoin, 3, '@sui:bank_join'))				
			if owner.bank_credits > 0:
				radial_list.append(RadialOptions(1, RadialIdentifier.bankWithdrawAll, 3, '@sui:bank_withdrawall'))
			if owner.cash_credits > 0:
				radial_list.append(RadialOptions(1, RadialIdentifier.bankDepositAll, 3, '@sui:bank_depositall'))
		#return the list
		return radial_list
		
	def handleRadial(self, owner, target, action):
		player_service = self.getKernel().serviceManager().playerService()
		if player_service:			
			if action == RadialIdentifier.bankItems:
				bank = self.getKernel().serviceManager().equipmentService().getEquippedObject(owner, "bank")
				player_service.openContainer(owner, bank)
			elif action == RadialIdentifier.bankJoin:
				self.joinBank(owner)
			elif action == RadialIdentifier.bankQuit:
				self.quitBank(owner)
			elif action == RadialIdentifier.bankTransfer:
				self.transferBank(owner, target)
			elif action == RadialIdentifier.bankWithdrawAll:
				amount = owner.bank_credits
				owner.cash_credits += owner.bank_credits
				owner.bank_credits = 0
				SystemMessage.sendSystemMessage(owner, swgpy.OutOfBand('base_player', 'prose_withdraw_success', swgpy.ProseType.DI, amount), False, False)
			elif action == RadialIdentifier.bankDepositAll:
				amount = owner.cash_credits
				owner.bank_credits += owner.cash_credits
				owner.cash_credits = 0
				SystemMessage.sendSystemMessage(owner, swgpy.OutOfBand('base_player', 'prose_deposit_success', swgpy.ProseType.DI, amount), False, False)
			
	def joinBank(self, owner):
		bank = self.getKernel().serviceManager().equipmentService().getEquippedObject(owner, "bank")
		if bank:
			if bank.scene_id == owner.scene_id:
				SystemMessage.sendSystemMessage(owner, 'system_msg', 'already_member_of_bank', False, False)
			elif bank.scene_id == 0:
				bank.scene_id = owner.scene_id
				SystemMessage.sendSystemMessage(owner, swgpy.OutOfBand('system_msg', 'succesfully_joined_bank'), False, False)
			else:
				SystemMessage.sendSystemMessage(owner, swgpy.OutOfBand('system_msg', 'member_of_different_bank'), False, False)
	def quitBank(self, owner):
		bank = self.getKernel().serviceManager().equipmentService().getEquippedObject(owner, "bank")
		if bank:
			if bank.hasContainedObjects() is not 0:
				bank.scene_id = 0
				SystemMessage.sendSystemMessage(owner, swgpy.OutOfBand('system_msg', 'succesfully_quit_bank'), False, False)		
			else:
				SystemMessage.sendSystemMessage(owner, swgpy.OutOfBand('system_msg', 'bank_not_empty'), False, False)	
	def transferBank(self, owner, target):
		sui = self.getKernel().serviceManager().suiService()
		
		#Enforce uniqueness
		if sui.getSUIWindowByScriptName(owner, 'Script.transfer') != None:
			return

		window = sui.createSUIWindow('Script.transfer', owner, target, 10)
		window.setProperty('transaction.txtInputFrom:Text', 'From')
		window.setProperty('transaction.txtInputTo:Text', 'To')	
		window.setProperty('bg.caption.lblTitle:Text', '@base_player:bank_title')
		window.setProperty('Prompt.lblPrompt:Text', '@base_player:bank_prompt')
		window.setProperty('transaction.lblFrom:Text', '@ui:inv_money_cash')
		window.setProperty('transaction.lblTo:Text', '@ui:inv_money_bank')	
		window.setProperty('transaction.lblStartingFrom:Text', str(owner.cash_credits))
		window.setProperty('transaction.lblStartingTo:Text', str(owner.bank_credits))
		window.setProperty('transaction.txtInputFrom:Text', str(owner.cash_credits))
		window.setProperty('transaction.txtInputTo:Text', str(owner.bank_credits))
		window.setProperty('transaction:ConversionRatioFrom', '1')
		window.setProperty('transaction:ConversionRatioTo', '1')
		results = ResultList()
		results.extend(
			['transaction.txtInputFrom:Text','transaction.txtInputTo:Text']
		)
		transferCallback = PythonCallback(self, 'transferCallback')
		window.subscribeToEventCallback(0, '', InputTrigger.OK, results, transferCallback)
		window.subscribeToEventCallback(1, '', InputTrigger.CANCEL, results, transferCallback)
		sui.openSUIWindow(window)
		
	def transferCallback(self, owner, event_id, results):
		if event_id == 0 and len(results) == 2:
			cash = int(results[0])
			bank = int(results[1])
			if cash + bank == owner.cash_credits + owner.bank_credits:
				owner.bank_credits = bank
				owner.cash_credits = cash
				SystemMessage.sendSystemMessage(owner, swgpy.OutOfBand('base_player', 'bank_success'), False, False)
			else:
				SystemMessage.sendSystemMessage(owner, 'NOPE', False, False)
		return True
				
			
