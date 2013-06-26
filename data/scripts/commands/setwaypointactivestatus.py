from swgpy.command import BaseSwgCommand
from swgpy.object import Waypoint
import sys

class SetWaypointActiveStatusCommand(BaseSwgCommand):

    def getCommandName(self):
        return 'setwaypointactivestatus'

    def run(self):
        waypoint = self.getTarget().castToWaypoint()

        if(waypoint.isActive()):
            waypoint.deactivate()
        else:
            waypoint.activate()
