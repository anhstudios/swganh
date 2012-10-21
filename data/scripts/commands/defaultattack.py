from swgpy.command import BaseCombatCommand
from swgpy import ACTION

class DefaultAttackCommand(BaseCombatCommand):
	def setup(self, properties):
		properties.default_time = 2.0	