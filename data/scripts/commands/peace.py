from swgpy.object import *
from swgpy.command import BaseSwgCommand
from swgpy import ACTION


class PeaceCommand(BaseSwgCommand):
    def getCommandName(self):
        return 'peace'

    def run(self):
        actor = self.getActor()
        target = self.getTarget()

        if actor.hasState(ACTION.COMBAT):
            actor.toggleStateOff(ACTION.COMBAT)
            actor.toggleStateOn(ACTION.PEACE)
            if target:
                actor.removeDefender(target.id)
                if not target.hasState(ACTION.COMBAT):
                    target.removeDefender(actor.id)
            actor.targetId = 0
            SystemMessage.sendFlyText(actor, "@combat_effects:go_peace", FlyTextColor.WHITE)
        else:
            actor.stateBitmask = ACTION.NONE

    def postRun(self, success):
        if success:
            combat_svc = self.getKernel().serviceManager().combatService()
            actor = self.getActor()
            target = self.getTargetCreature()
            if (actor and target):
                combat_svc.endCombat(actor, target)
