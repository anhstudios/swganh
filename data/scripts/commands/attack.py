import swgpy.combat, swgpy.object, swgpy.app
     
class attackCommand(swgpy.combat.CombatCommand):
    def Run(self, kernel, actor, target, command_string):
        if not actor.has_state(swgpy.ACTION.COMBAT):
            actor.toggle_state_off(swgpy.ACTION.PEACE);
            actor.toggle_state_on(swgpy.ACTION.COMBAT)
            actor.activate_auto_attack()