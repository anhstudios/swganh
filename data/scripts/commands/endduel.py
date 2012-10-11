from swgpy.command import BaseSwgCommand
from swgpy import combat
class EndDuelCommand(BaseSwgCommand):
    def run(self):
        actor = self.getActor()
        target = self.getTargetCreature()
        
        combat_service = self.getKernel().serviceManager().combatService()
        
        if (actor and target):
            combat_service.endDuel(actor, target)