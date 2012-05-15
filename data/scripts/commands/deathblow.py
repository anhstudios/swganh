from swgpy.command import BaseSwgCommand
from swgpy import POSTURE

class DeathBlowCommand(BaseSwgCommand):
    def Run(self):
        target = self.GetTarget()
        if target:
            if target.is_incap():
                target.posture = POSTURE.DEAD
                target.clear_auto_attack()
                actor.target_id = 0
                actor.remove_duel_list(target.id)
                target.remove_duel_list(actor.id)
