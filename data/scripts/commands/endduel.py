from swgpy.command import BaseSwgCommand
from swgpy import combat
class EndDuelCommand(BaseSwgCommand):
    def run(self):
        actor = self.getActor()
        target = self.getTargetCreature()
        
        service_mgr = self.getKernel().serviceManager()
        
        combat_svc = service_mgr.combatService()
        if (actor and target):
            combat_svc.endDuel(actor, target)