import re
import swgpy.equipment
from swgpy.command import BaseSwgCommand

class AddIgnoreCommand(BaseSwgCommand):
    def run(self):
        split = re.split('\W+', self.GetCommandString())
        player_name = split[0]
        
        actor = self.getActor()
        player = self.getKernel().serviceManager().equipmentService().getPlayerObject(actor)
        if player:
            # Check if the name is already in our friends list
            if not player.isIgnored(player_name):
                added = self.getKernel().serviceManager().socialService().addIgnore(player, player_name)
                if added:
                    actor.SendSystemMessage(swgpy.outOfBand('cmnty', 'ignore_added', swgpy.ProseType.TT, player_name), False, False)
                else:
                    actor.SendSystemMessage(swgpy.outOfBand('cmnty', 'ignore_not_found', swgpy.ProseType.TT, player_name), False, False)	
            else:
                actor.SendSystemMessage(swgpy.outOfBand('cmnty', 'ignore_duplicate', swgpy.ProseType.TT, player_name), False, False)
        else:
            print('Player object not found for object id' + actor.id)
