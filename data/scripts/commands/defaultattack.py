from swgpy.command import BaseCombatCommand


class DefaultAttackCommand(BaseCombatCommand):
    def getCommandName(self):
        return 'defaultattack'

    def setup(self, properties):
        properties.default_time = 2.0
