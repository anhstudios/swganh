from swgpy.command import BaseSwgCommand
from swgpy import POSTURE


class ProneCommand(BaseSwgCommand):
    def getCommandName(self):
        return 'prone'

    def run(self):
        actor = self.getActor()

        actor.posture = POSTURE.PRONE
        actor.run_speed = 1.0
