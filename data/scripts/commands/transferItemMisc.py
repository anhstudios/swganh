import re
from swgpy.command import BaseSwgCommand
from swgpy.utility import *


class TransferItemMiscCommand(BaseSwgCommand):
    def getCommandName(self):
        return 'transferitemmisc'

    def run(self):
        split = re.split('\W+', self.getCommandString())
        container_id = split[1]
        c = self.getKernel().serviceManager().simulationService().findObjectById(int(container_id))
        self.getTarget().container().transfer(self.getActor(), self.getTarget(), c, vector3(0, 0, 0))
