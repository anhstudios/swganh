import re
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
                actor.sendSystemMessage(swgpy.outOfBand('cmnty', 'ignore_removed', swgpy.ProseType.TT, player_name), False, False)
            else:
                actor.sendSystemMessage(swgpy.outOfBand('cmnty', 'ignore_not_found', swgpy.ProseType.TT, player_name), False, False)
