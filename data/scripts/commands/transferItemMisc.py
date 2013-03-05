import re
import swgpy
from swgpy.command import BaseSwgCommand

class TransferItem(BaseSwgCommand):
    def run(self):
        split = re.split('\W+', self.getCommandString())
        container_id = split[1]
        c = self.getKernel().serviceManager().simulationService().findObjectById(int(container_id))
        self.getTarget().container().transfer(self.getActor(), self.getTarget(), c, vector3(0, 0, 0))