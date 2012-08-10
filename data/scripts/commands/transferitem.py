import re
import swgpy
from swgpy.command import BaseSwgCommand

class TransferItem(BaseSwgCommand):
    def Run(self):
        print('transfer item entered')
        split = re.split('\W+', self.GetCommandString())
        target_id = split[1]
        arrangement_id = split[2]
        x = split[3]
        y = split[4]
        z = split[5]
        actor = self.GetActor()
        print(split)
        
        target = self.GetKernel().service_manager().simulation_service().findObjectById(int(target_id))
        
        actor.Equip(target)