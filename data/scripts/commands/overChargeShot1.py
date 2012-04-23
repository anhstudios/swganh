import swgpy.command, swgpy.object, swgpy.app
     
class overChargeShot1Command(swgpy.command.CombatCommand):
    def SetUp(self):
        combat_spam = 'overchargeshot'
        health_cost_multiplier = 0.5
        action_cost_multiplier = 0.5
        mind_cost_multiplier = 0.5
        damage_multiplier = 5.0