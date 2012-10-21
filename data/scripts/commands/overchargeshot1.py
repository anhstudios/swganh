from swgpy.command import BaseCombatCommand

class OverchargeShot1Command(BaseCombatCommand):
	def setup(self, properties):
		properties.combat_spam = 'overchargeshot'
		properties.health_cost_multiplier = 0.5
		properties.action_cost_multiplier = 0.5
		properties.mind_cost_multiplier = 0.5
		properties.damage_multiplier = 5.0