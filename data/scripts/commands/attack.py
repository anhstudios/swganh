from swgpy.command import BaseCombatCommand


class AttackCommand(BaseCombatCommand):
    def getCommandName(self):
        return 'attack'

    def setup(self, properties):
        properties.max_range_to_target = 7.5
