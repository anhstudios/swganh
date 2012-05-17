import re
from swgpy.command import BaseSwgCommand

class SitServerCommand(BaseSwgCommand):
    def Run(self):
        split = re.split('\W+', self.GetCommandString())
        player_name = split[0]

        player = self.GetActor().get_player()
        if player:
            # Check if the name is in our ignore list
            if player.is_ignored(player_name):
                player.remove_ignore(player_name)
                self.GetController().SendSystemMessage(swgpy.OutOfBand('cmnty', 'ignore_removed', swgpy.ProseType.TT, player_name), False, False)
            else:
                self.GetController().SendSystemMessage(swgpy.OutOfBand('cmnty', 'ignore_not_found', swgpy.ProseType.TT, player_name), False, False)
