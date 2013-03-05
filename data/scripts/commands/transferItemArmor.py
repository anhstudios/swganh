import re
import swgpy
from swgpy.command import BaseSwgCommand

class TransferItemArmor(BaseSwgCommand):
    def run(self):
        split = re.split('\W+', self.getCommandString())
        container_id = split[1]
        container = self.getKernel().serviceManager().simulationService().findObjectById(int(container_id))
        self.getTarget().container().transfer(self.getActor(), self.getTarget(), container, vector3(0, 0, 0))