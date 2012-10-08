from swgpy.command import BaseSwgCommand
from swgpy import POSTURE

class SitServerCommand(BaseSwgCommand):
    def run(self):
        actor = self.getActor()
        
        actor.posture = POSTURE.SITTING
        actor.run_speed = 0
