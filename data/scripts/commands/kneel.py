from swgpy.command import BaseSwgCommand
from swgpy import POSTURE

class KneelCommand(BaseSwgCommand):
    def Run(self):
        actor = self.GetActor()
        
        actor.posture = POSTURE.CROUCHED
        actor.run_speed = 0