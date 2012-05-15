from swgpy.command import BaseCombatCommand

class Berserk1Command(BaseCombatCommand):
    combat_spam = 'berserk'
    health_cost_multiplier = 0.5
    action_cost_multiplier = 0.5
    mind_cost_multiplier = 0.5
    damage_multiplier = 5.0