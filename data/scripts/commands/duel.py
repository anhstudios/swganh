from swgpy.command import BaseSwgCommand
from swgpy import PVPSTATUS, OutOfBand, ProseType

class DuelCommand(BaseSwgCommand):
    def run(self):
        actor = self.getActor()
        target = self.getTarget()
        
        if not actor.inDuelList(target.id):
            actor.addDuelList(target.id)
            #system message
            actor.sendSystemMessage(outOfBand("duel", "challenge_self", ProseType.TT, target.id), False, False)
            target.sendSystemMessage(outOfBand("duel", "challenge_target", ProseType.TT, actor.id), False, False)
        if actor.inDuelList(target.id) and target.inDuelList(actor.id):
            # Start The Duel
            actor.sendSystemMessage(outOfBand("duel", "accept_target", ProseType.TT, target.id), False, False)
            target.sendSystemMessage(outOfBand("duel", "accept_self", ProseType.TT, actor.id), False, False)
            actor.pvp_status = PVPSTATUS.PvPStatus_Attackable
            target.pvp_status = PVPSTATUS.PvPStatus_Attackable
            actor.activateAutoAttack();
            target.activateAutoAttack();
