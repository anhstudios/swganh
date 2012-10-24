from swgpy.combat import Buff

class SpiceBoosterBlueDown(Buff):
	
	def getName(self):
		return "spice.booster_blue.down"
		
	def getDuration(self):
		return 60 #seconds
	
	def applyBuff(self, creature):
		pass
		
	def removeBuff(self, creature):
		pass
		