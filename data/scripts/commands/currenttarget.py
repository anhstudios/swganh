from swgpy.command import BaseSwgCommand
from swgpy import POSTURE

class CurrentTargetCommand(BaseSwgCommand):    
	def run(self):    
		actor = self.getActor()
		actor.target_id = self.getTarget()
		print('set target')