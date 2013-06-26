from swgpy.command import BaseSwgCommand
from swgpy import POSTURE


class KneelCommand(BaseSwgCommand):
    def getCommandName(self):
        return 'kneel'

    def run(self):
        actor = self.getActor()

        actor.posture = POSTURE.CROUCHED
        actor.runSpeed = 0
