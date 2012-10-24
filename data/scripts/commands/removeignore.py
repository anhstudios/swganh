import re
from swgpy.object import *
from swgpy.command import BaseSwgCommand

class SitServerCommand(BaseSwgCommand):
	def run(self):
		split = re.split('\W+', self.getCommandString())
		player_name = split[0]
		
		actor = self.getActor()
		player = actor.getPlayer()
		
		if player:
			# Check if the name is in our ignore list
			if player.isIgnored(player_name):
				player.removeIgnore(player_name)
				SystemMessage.sendSystemMessage(actor, swgpy.OutOfBand('cmnty', 'ignore_removed', swgpy.ProseType.TT, player_name), False, False)
			else:
				SystemMessage.sendSystemMessage(actor, swgpy.OutOfBand('cmnty', 'ignore_not_found', swgpy.ProseType.TT, player_name), False, False)
