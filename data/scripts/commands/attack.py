from swgpy.command import BaseCombatCommand
from swgpy import ACTION

class AttackCommand(BaseCombatCommand):
	def setup(self, properties):
		properties.default_time = 5.0
		properties.accuracy_bonus = 155