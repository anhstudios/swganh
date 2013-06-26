from swgpy.command import BaseSwgCommand
from swgpy.object import Waypoint
import sys

class SetWaypointNameCommand(BaseSwgCommand):

    def getCommandName(self):
        return 'setwaypointname'

    def run(self):
        waypoint = self.getTarget().castToWaypoint()

        waypoint.setName(self.getCommandString())

        player = waypoint.container().castToPlayer()
        player.modifyWaypoint(waypoint.id)
