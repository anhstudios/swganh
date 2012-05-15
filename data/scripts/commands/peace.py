from swgpy.command import BaseSwgCommand
from swgpy import ACTION

class PeaceCommand(BaseSwgCommand):
    def Run(self):
        actor = self.GetActor()
        target = self.GetTarget()
        
        if actor.has_state(ACTION.COMBAT):
            actor.toggle_state_off(ACTION.COMBAT)
            actor.toggle_state_on(ACTION.PEACE)
            actor.remove_defender(target.id)
            actor.target_id = 0
            actor.clear_auto_attack()
            if not target.has_state(ACTION.COMBAT):
                target.remove_defender(actor.id)
            #Send VIA ShowFlyText
            #actor.Controller().SendSystemMessage("@combat_effects:go_peace")
        else:
            actor.state_bitmask = ACTION.NONE   
