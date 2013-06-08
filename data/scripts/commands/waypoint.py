import re
import swgpy
from swgpy.object import *
from swgpy.simulation import *
from swgpy.utility import vector3
from swgpy.command import BaseSwgCommand

class WaypointCommand(BaseSwgCommand):
    def getCommandName(self):
        return 'waypoint'

    def run(self):
        simulation = self.getKernel().serviceManager().simulationService()
        actor = self.getActor()

        digits = re.findall(r'\-?\d+(?:\.\d*)?', self.getCommandString())
        length = len(digits)

        waypoint_position = vector3()

        if length == 3:
            waypoint_position.x = float(digits[0])
            waypoint_position.y = float(digits[1])
            waypoint_position.z = float(digits[2])
        elif length == 2:
            waypoint_position.x = float(digits[0])
            waypoint_position.z = float(digits[1])
        elif length == 0:
            waypoint_position = actor.position
        else:
            SystemMessage.sendSystemMessage(actor, "[SYNTAX] /waypoint <x> <z> or /waypoint <x> <y> <z>", True, False)
            return

        planet = simulation.getSceneNameById(actor.scene_id)
        player = self.getKernel().serviceManager().equipmentService().getPlayerObject(actor)

        waypoint = simulation.createObject('object/waypoint/shared_waypoint.iff').castToWaypoint()
        waypoint.setCoordinates(waypoint_position)
        waypoint.setPlanet(planet)
        waypoint.setColor(WaypointColor.BLUE)
        waypoint.activate()

        player.addWaypoint(waypoint)
