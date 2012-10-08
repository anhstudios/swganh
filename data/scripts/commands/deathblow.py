from swgpy.command import BaseSwgCommand
from swgpy import POSTURE

class DeathBlowCommand(BaseSwgCommand):
    def run(self):
        target = self.getTarget()
        if target:
            if target.isIncap():
                target.posture = POSTURE.DEAD
                target.clearAutoAttack()
                actor.targetId = 0
                actor.removeDuelList(target.id)
                target.removeDuelList(actor.id)
