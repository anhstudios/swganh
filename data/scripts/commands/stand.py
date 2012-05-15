from swgpy.command import BaseSwgCommand
from swgpy import POSTURE

class StandCommand(BaseSwgCommand):    
    base_run_speed = 5.75
    
    def Run(self):
        actor = self.GetActor()
        
        actor.posture = POSTURE.UPRIGHT
        actor.run_speed = self.base_run_speed
