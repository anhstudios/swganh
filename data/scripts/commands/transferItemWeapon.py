import re
import swgpy
from swgpy.command import BaseSwgCommand

class TransferItemWeapon(BaseSwgCommand):
    def Run(self):
        print('transfer item weapon entered')
        split = re.split('\W+', self.GetCommandString())
        container_id = split[1]
        container = self.GetKernel().service_manager().simulation_service().findObjectById(int(container_id))
        self.GetTarget().Container().Transfer(self.GetActor(), self.GetTarget(), container)