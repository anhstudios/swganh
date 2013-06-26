from swgpy.command import BaseSwgCommand
from swgpy import POSTURE


class DeathBlowCommand(BaseSwgCommand):
    def getCommandName(self):
        return 'deathblow'

    def run(self):
        target = self.getTarget()
        actor = self.getActor()
        if target:
            if target.isIncap():
                target.posture = POSTURE.DEAD
                target.clearAutoAttack()
                actor.targetId = 0
                actor.removeDuelList(target.id)
                target.removeDuelList(actor.id)
