from swgpy.command import BaseSwgCommand
from swgpy import POSTURE

class SitServerCommand(BaseSwgCommand):
    def Run(self):
        actor = self.GetActor()
        
        actor.posture = POSTURE.SITTING
        actor.run_speed = 0
