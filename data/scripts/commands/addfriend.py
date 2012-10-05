import re
import swgpy
import swgpy.equipment
from swgpy.command import BaseSwgCommand

class AddFriendCommand(BaseSwgCommand):
    def run(self):
        split = re.split('\W+', self.getCommandString())
        friend_request_name = split[0]
        
        actor = self.getActor()
        player = self.getKernel().serviceManager().equipmentService().getPlayerObject(actor)
        if player:
            # Check if the name is already in our friends list
            if not player.isFriend(friend_request_name):
                added = self.getKernel().serviceManager().socialService().addFriend(player, friend_request_name)
                if added:
                    actor.sendSystemMessage(swgpy.outOfBand('cmnty', 'friend_added', swgpy.ProseType.TT, friend_request_name), False, False)
                else:
                    actor.sendSystemMessage(swgpy.outOfBand('cmnty', 'friend_not_found', swgpy.ProseType.TT, friend_request_name), False, False)	
            else:
                actor.sendSystemMessage(swgpy.outOfBand('cmnty', 'friend_duplicate', swgpy.ProseType.TT, friend_request_name), False, False)
        else:
            print('Player object not found for object id' + actor.id)
