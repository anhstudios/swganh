from swgpy.command import BaseCombatCommand
from swgpy import ACTION

class AttackCommand(BaseCombatCommand):
	def setup(self, properties):
		properties.max_range_to_target = 5.0
		