import swgpy.object, swgpy.app
from swgpy.command import *
     
class standCommand(Command):
    # Setup Value
    def SetUp(self):
        print('setup')
        self.name = "stand"
        self.health_cost = 10
    def Run(self, kernel, actor, target, command_string):
        print('run')
        actor.posture = swgpy.POSTURE.UPRIGHT
        actor.run_speed = 5.75      
