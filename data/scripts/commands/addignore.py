import re
from swgpy.command import BaseSwgCommand

class AddIgnoreCommand(BaseSwgCommand):
    def Run(self):
        split = re.split('\W+', self.GetCommandString())
        player_name = split[0]
        
        actor = self.GetActor()
        player = actor.get_player()
        if player:
            # Check if the name is already in our friends list
            if not player.is_ignored(player_name):
                added = self.GetKernel().service_manager().social_service().add_ignore(player, player_name)
                if added:
                    self.GetController().SendSystemMessage(swgpy.OutOfBand('cmnty', 'ignore_added', swgpy.ProseType.TT, player_name), False, False)
                else:
                    self.GetController().SendSystemMessage(swgpy.OutOfBand('cmnty', 'ignore_not_found', swgpy.ProseType.TT, player_name), False, False)	
            else:
                self.GetController().SendSystemMessage(swgpy.OutOfBand('cmnty', 'ignore_duplicate', swgpy.ProseType.TT, player_name), False, False)
        else:
            print('Player object not found for object id' + actor.id)
