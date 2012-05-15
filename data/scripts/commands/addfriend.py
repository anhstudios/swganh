import re
from swgpy.command import BaseSwgCommand

class AddFriendCommand(BaseSwgCommand):
    def Run(self):
        split = re.split('\W+', self.GetCommandString())
        friend_request_name = split[0]
        
        actor = self.GetActor()
        player = actor.get_player()
        if player:
            # Check if the name is already in our friends list
            if not player.is_friend(friend_request_name):
                added = self.GetKernel().service_manager().social_service().add_friend(player, friend_request_name)
                if added:
                    actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_added', swgpy.ProseType.TT, friend_request_name), False, False)
                else:
                    actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_not_found', swgpy.ProseType.TT, friend_request_name), False, False)	
            else:
                actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_duplicate', swgpy.ProseType.TT, friend_request_name), False, False)
        else:
            print('Player object not found for object id' + actor.id)
