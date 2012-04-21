import swgpy.command, swgpy.combat, swgpy.object, swgpy.app
     
class endDuelCommand(swgpy.command.Command):
    def SetUp(self):
        self.name = "duel"
    def Run(self, kernel, actor, target, command_string):
        service_mgr = kernel.service_manager()
        combat_svc = service_mgr.combat_service()
        if (actor and creature_target):
            combat_svc.end_duel(actor, creature_target)
