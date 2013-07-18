from swgpy.command import BaseSwgCommand
import re

commandRegex = re.compile(r"([0-9]+) ([-+]?[0-9]*\.?[0-9]+) ([-+]?[0-9]*\.?[0-9]+) ([-+]?[0-9]*\.?[0-9]+)")

class PlaceStructureCommand(BaseSwgCommand):
    def getCommandName(self):
        return 'placestructure'

    def run(self):
        actor = self.getActor()
        result = commandRegex.match(self.getCommandString())
        if result:
            #Get the values
            deed_id, x, z, direction = result.groups()
            
            #Perform casts
            deed_id = int(deed_id)
            x = float(x)
            z = float(z)
            direction = float(direction)
        
            self.getKernel().serviceManager().structureService().placeStructureByDeed(actor, deed_id, x, z, direction)