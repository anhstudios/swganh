import re
import swgpy
from swgpy.command import BaseSwgCommand

class TransferItem(BaseSwgCommand):
    def Run(self):
        split = re.split('\W+', self.GetCommandString())
        container_id = split[1]
        container = self.GetKernel().service_manager().simulation_service().findObjectById(int(container_id))
        self.GetActor().Transfer(self.GetActor(), self.GetTarget(), container)