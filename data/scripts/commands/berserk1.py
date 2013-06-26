from swgpy.command import BaseCombatCommand
from swgpy.object import SystemMessage, FlyTextColor


class Berserk1Command(BaseCombatCommand):
    def getCommandName(self):
        return 'berserk1'

    def setup(self, properties):
        properties.combat_spam = 'berserk'
        properties.health_cost_multiplier = 0.5
        properties.action_cost_multiplier = 0.5
        properties.mind_cost_multiplier = 0.5
        properties.damage_multiplier = 2.0

    def run(self):
        actor = self.getActor()
        SystemMessage.sendEffect(actor, "combat_special_attacker_berserk", "root")
        SystemMessage.sendFlyText(actor, "@combat_effects:go_berserk", FlyTextColor.GREEN)
        # TODO: Set Buff...
