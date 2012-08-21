from swgpy.command import BaseSwgCommand
from swgpy import combat
class EndDuelCommand(BaseSwgCommand):
    def Run(self):
        actor = self.GetActor()
        target = self.GetTargetCreature()
        
        service_mgr = self.GetKernel().service_manager()
        
        combat_svc = service_mgr.combat_service()
        if (actor and target):
            combat_svc.end_duel(actor, target)