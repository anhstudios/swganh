from swgpy.command import BaseSwgCommand
from swgpy import POSTURE

class KneelCommand(BaseSwgCommand):
    def run(self):
        actor = self.getActor()
        
        actor.posture = POSTURE.CROUCHED
        actor.runSpeed = 0