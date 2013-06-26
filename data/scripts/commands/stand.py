from swgpy.command import BaseSwgCommand
from swgpy import POSTURE


class StandCommand(BaseSwgCommand):
    base_run_speed = 5.75

    def getCommandName(self):
        return 'stand'

    def run(self):
        print("test")
        actor = self.getActor()
        print("test")
        actor.posture = POSTURE.UPRIGHT
        actor.run_speed = self.base_run_speed
