from swgpy.command import BaseSwgCommand
from swgpy import POSTURE

class KneelCommand(BaseSwgCommand):
    def Run(self):
        actor = self.getActor()
        
        actor.posture = POSTURE.CROUCHED
        actor.runSpeed = 0