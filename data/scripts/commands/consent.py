import swgpy
import re
from swgpy.command import BaseSwgCommand
from swgpy.object import SystemMessage
from swgpy import OutOfBand, ProseType

class ConsentCommand(BaseSwgCommand):
	def run(self):
		actor = self.getActor()
		actor = actor.toCreature()
		
		sim_serv = self.getKernel().serviceManager().simulationService()
		play_serv = self.getKernel().serviceManager().playerService()
		
		results = re.split('\W+', self.getCommandString())
		len_result = len(results)
		if len_result == 1 and results[0] == '':
			creo = sim_serv.findObjectById(actor.target_id)
			if creo and creo.hasFlag("player_creature") and creo.id != actor.id:
				self.addConsent(actor, creo)
			else:
				SystemMessage.sendSystemMessage(actor, "You may not consent that target.", False, False)
		elif len_result >= 1:
			for name in results:
				creo = play_serv.getPlayerByFirstName(name)
				if creo.id != actor.id:	
					if not self.addConsent(actor, creo):
						break
				else:
					SystemMessage.sendSystemMessage(actor, "You may not consent yourself.", False, False)
				
	def addConsent(self, actor, creo):
		if creo:
			if actor.grantConsent(creo.id):
				SystemMessage.sendSystemMessage(actor, OutOfBand("base_player", "prose_consent", ProseType.TO, creo.first_name) , False, False)
				SystemMessage.sendSystemMessage(creo, OutOfBand("base_player", "prose_got_consent", ProseType.TO, actor.first_name) , False, False)
				return True
			else:
				SystemMessage.sendSystemMessage(actor, "You have consented too many targets. Use /unconsent first.", False, False)
				return False
		else:
			SystemMessage.sendSystemMessage(actor, "That name is invalid.", False, False)