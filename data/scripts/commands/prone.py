from swgpy.command import BaseSwgCommand
from swgpy import POSTURE

class ProneCommand(BaseSwgCommand):
    def Run(self):
        actor = self.GetActor()
        
        actor.posture = POSTURE.PRONE
        actor.run_speed = 1.0           
