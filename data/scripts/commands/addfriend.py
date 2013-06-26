import re
import swgpy
import swgpy.equipment
from swgpy.object import *
from swgpy.command import BaseSwgCommand


class AddFriendCommand(BaseSwgCommand):
    def getCommandName(self):
        return 'addfriend'

    def run(self):
        split = re.split('\W+', self.getCommandString())
        friend_request_name = split[0]

        serviceManager = self.getKernel().serviceManager()
        equipmentService = serviceManager.equipmentService()
        chatService = serviceManager.chatService()

        actor = self.getActor()
        player = equipmentService.getPlayerObject(actor)
        if player:
            # Check if the name is already in our friends list
            if not player.isFriend(friend_request_name):
                if chatService.getObjectIdByName(friend_request_name) != 0:
                    player.addFriend(friend_request_name)
                    SystemMessage.sendSystemMessage(actor, swgpy.OutOfBand('cmnty', 'friend_added', swgpy.ProseType.TT, friend_request_name), False, False)
                else:
                    SystemMessage.sendSystemMessage(actor, swgpy.OutOfBand('cmnty', 'friend_not_found', swgpy.ProseType.TT, friend_request_name), False, False)
            else:
                SystemMessage.sendSystemMessage(actor, swgpy.OutOfBand('cmnty', 'friend_duplicate', swgpy.ProseType.TT, friend_request_name), False, False)
        else:
            print('Player object not found for object id' + actor.id)
