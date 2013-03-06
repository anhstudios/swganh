import swgpy
from swgpy.command import BaseSwgCommand
from swgpy.object import SystemMessage
from swgpy.sui import *

class HaveConsentCommand(BaseSwgCommand):
	def run(self):
		actor = self.getActor()
		actor = actor.toCreature()
		
		sui = self.getKernel().serviceManager().suiService()
		sim = self.getKernel().serviceManager().simulationService()
		
		if sui.getSUIWindowByScriptName(actor, 'Script.listBox') != None:
			return

		options = EventResultList()
		session_data = actor.getConsentedPlayers()
		
		if len(session_data) > 0:
			for option in session_data:
				creo = sim.findObjectById(option)
				options.append(creo.first_name)
			
			window = sui.createListBox(ListBoxType.OK, '@cmd_n:haveconsent', '', options, actor)
				
			results = ResultList()
			results.append('List.lstList:SelectedRow')
			callback = PythonCallback(self, session_data, 'transferCallback')
			
			window.subscribeToEventCallback(0, '', InputTrigger.OK, results, callback)
			sui.openSUIWindow(window)
		else:
			SystemMessage.sendSystemMessage(actor, "You have no consented targets.", False, False)
		
	def transferCallback(self, session_data, owner, event_id, results):
		return True
		