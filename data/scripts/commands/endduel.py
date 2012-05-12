from swgpy.command import BaseSwgCommand

class EndDuelCommand(BaseSwgCommand):
    def Run(self):
        actor = self.GetActor()
        target = self.GetTarget()
        
        service_mgr = self.GetKernel().service_manager()
        combat_svc = service_mgr.combat_service()
        if (actor and target):
            combat_svc.end_duel(actor, target)
