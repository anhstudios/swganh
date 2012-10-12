from swgpy.command import BaseCombatCommand
from swgpy import ACTION

class DefaultAttackCommand(BaseCombatCommand):
	def setup(self, properties):
		print(properties)
		properties.default_time = 2.0
		print(properties.default_time)
		properties.accuracy_bonus = 155