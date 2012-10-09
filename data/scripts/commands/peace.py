from swgpy.object import *
from swgpy.command import BaseSwgCommand
from swgpy import ACTION

class PeaceCommand(BaseSwgCommand):
	def run(self):
		actor = self.getActor()
		target = self.getTarget()
		
		if actor.hasState(ACTION.COMBAT):
			actor.toggleStateOff(ACTION.COMBAT)
			actor.toggleStateOn(ACTION.PEACE)
			actor.removeDefender(target.id)
			actor.targetId = 0
			actor.clearAutoAttack()
			if not target.hasState(ACTION.COMBAT):
				target.removeDefender(actor.id)
			
			SystemMessage.sendFlyText(actor, "@combat_effects:go_peace", FlyTextColor.WHITE)
		else:
			actor.stateBitmask = ACTION.NONE   
