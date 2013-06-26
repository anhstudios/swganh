import swgpy
from swgpy.command import BaseSwgCommand
from swgpy.object import SystemMessage


class TipCommand(BaseSwgCommand):
    def getCommandName(self):
        return 'tip'

    def run(self):
        actor = self.getActor()
        actor = actor.toCreature()
        service_manager = self.getKernel().serviceManager()
        equipment_service = service_manager.equipmentService()
        player_service = service_manager.playerService()
        results = self.getCommandString().split()
        result_count = len(results)
        if result_count == 1:
            #needs to have target
            target = self.getTargetCreature()
            if target:
                player = equipment_service.getPlayerObject(target)
                if player:
                    #get amount
                    amount = int(results[0])
                    if amount:
                        player_service.sendTip(actor, target, amount, False)
                else:
                    SystemMessage.sendSystemMessage(actor, swgpy.OutOfBand('error_message', 'tip_target_bad'), False, False)
        elif result_count >= 2:
            if results[1] == 'bank':
                #needs to have target
                target = self.getTargetCreature()
                if target:
                    player = equipment_service.getPlayerObject(target)
                    if player:
                        amount = int(results[0])
                        if amount:
                            player_service.sendTip(actor, target, amount, True)
                    else:
                        SystemMessage.sendSystemMessage(actor, swgpy.OutOfBand('error_message', 'tip_target_bad'), False, False)
                else:
                    SystemMessage.sendSystemMessage(actor, swgpy.OutOfBand('error_message', 'tip_target_bad'), False, False)
            else:
                target = service_manager.simulationService().findObject(results[0])
                if target:
                    player = equipment_service.getPlayerObject(target)
                    if player:
                        #get amount
                        amount = int(results[1])
                        if amount:
                            if result_count == 3:
                                #BankTip
                                player_service.sendTip(actor, target, amount, True)
                            else: #CashTip
                                player_service.sendTip(actor, target, amount, False)
                    else:
                        SystemMessage.sendSystemMessage(actor, swgpy.OutOfBand('error_message', 'tip_target_bad'), False, False)
                else:
                    SystemMessage.sendSystemMessage(actor, swgpy.OutOfBand('error_message', 'tip_target_bad'), False, False)
