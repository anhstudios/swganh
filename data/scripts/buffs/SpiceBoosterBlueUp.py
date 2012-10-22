from swgpy.combat import Buff

class SpiceBoosterBlueUp(Buff):
	
	def getName(self):
		return "spice.booster_blue.up"
		
	def getDuration(self):
		return 60 #seconds
	
	def applyBuff(self, creature):
		pass
		
	def removeBuff(self, creature):
		creature.addBuff("SpiceBoosterBlueDown")
		
