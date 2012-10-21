import swgpy
from swgpy.object import *
from swgpy.command import BaseSwgCommand
from swgpy import combat
class EndDuelCommand(BaseSwgCommand):
	def run(self):
		actor = self.getActor()
		target = self.getTargetCreature()
		
		service_mgr = self.getKernel().serviceManager()
		combat_svc = service_mgr.combatService()
		print(combat_svc)
		print(target)
		if (actor and target):
			print(combat_svc.endDuel)
			combat_svc.endDuel(actor, target)