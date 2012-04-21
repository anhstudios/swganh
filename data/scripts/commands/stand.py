import swgpy.object
from swgpy.command import *
     
class StandCommand(Command):
    # Setup Value
    def SetUp(self):
        print('setup')
        self.name = "stand"
        self.health_cost = 10
    def Run(actor):
        actor.posture = swgpy.POSTURE.UPRIGHT
        actor.run_speed = 5.75      

command = StandCommand()
command.SetUp()