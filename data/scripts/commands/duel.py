from swgpy.object import *
from swgpy.command import BaseSwgCommand
from swgpy import PVPSTATUS, OutOfBand, ProseType


class DuelCommand(BaseSwgCommand):
    def getCommandName(self):
        return 'duel'

    def run(self):
        actor = self.getActor()
        target = self.getTarget()

        if not actor.inDuelList(target.id):
            actor.addDuelList(target.id)
            #system message
            SystemMessage.sendSystemMessage(actor, OutOfBand("duel", "challenge_self", ProseType.TT, target.id), False, False)
            SystemMessage.sendSystemMessage(target, OutOfBand("duel", "challenge_target", ProseType.TT, actor.id), False, False)
        if actor.inDuelList(target.id) and target.inDuelList(actor.id):
            # Start The Duel
            SystemMessage.sendSystemMessage(actor, OutOfBand("duel", "accept_target", ProseType.TT, target.id), False, False)
            SystemMessage.sendSystemMessage(target, OutOfBand("duel", "accept_self", ProseType.TT, actor.id), False, False)
            actor.pvp_status = PVPSTATUS.PvPStatus_Attackable
            target.pvp_status = PVPSTATUS.PvPStatus_Attackable
            actor.activateAutoAttack()
            target.activateAutoAttack()
