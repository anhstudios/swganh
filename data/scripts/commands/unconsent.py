import swgpy
from swgpy import OutOfBand, ProseType
from swgpy.command import BaseSwgCommand
from swgpy.object import SystemMessage
from swgpy.sui import *


class UnconsentCommand(BaseSwgCommand):
	def run(self):
		actor = self.getActor()
		actor = actor.toCreature()
		
		sui = self.getKernel().serviceManager().suiService()
		sim = self.getKernel().serviceManager().simulationService()
		
		#if sui.getSUIWindowByScriptName(actor, 'Script.listBox') != None:
		#	return

		options = EventResultList()
		session_data = actor.getConsentedPlayers()
		
		if len(session_data) > 0:
			for option in session_data:
				creo = sim.findObjectById(option)
				options.append(creo.first_name)
			
			window = sui.createListBox(ListBoxType.OK_CANCEL, '@cmd_n:unconsent', '', options, actor)
				
			results = ResultList()
			results.append('List.lstList:SelectedRow')
			callback = PythonCallback(self, session_data, 'transferCallback')
			
			window.subscribeToEventCallback(0, '', InputTrigger.OK, results, callback)
			window.subscribeToEventCallback(1, '', InputTrigger.CANCEL, results, callback)
			sui.openSUIWindow(window)
		else:
			SystemMessage.sendSystemMessage(actor, "You have no consented targets.", False, False)
		
	def transferCallback(self, session_data, owner, event_id, results):
		choice = int(results[0])
		if event_id == 0 and choice >= 0:	
			sim = self.getKernel().serviceManager().simulationService()
			creo = sim.findObjectById(session_data[choice])
			if creo:
				owner.revokeConsent(creo.id)
				SystemMessage.sendSystemMessage(owner, OutOfBand("base_player", "prose_unconsent", ProseType.TO, creo.first_name) , False, False)
				SystemMessage.sendSystemMessage(creo, OutOfBand("base_player", "prose_lost_consent", ProseType.TO, owner.first_name) , False, False)
			else:
				SystemMessage.sendSystemMessage(actor, "An internal error occurred.", False, False)
		return True
		