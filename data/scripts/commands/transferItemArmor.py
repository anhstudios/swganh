import re
from swgpy.command import BaseSwgCommand
from swgpy.utility import *


class TransferItemArmorCommand(BaseSwgCommand):
    def getCommandName(self):
        return 'transferitemarmor'

    def run(self):
        split = re.split('\W+', self.getCommandString())
        container_id = split[1]
        container = self.getKernel().serviceManager().simulationService().findObjectById(int(container_id))
        self.getTarget().container().transfer(self.getActor(), self.getTarget(), container, vector3(0, 0, 0))
