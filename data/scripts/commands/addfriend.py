import re, swgpy.command,swgpy.object, swgpy.app
     
class addFriendCommand(swgpy.command.Command):
    def Run(self, kernel, actor, target, command_string):
        split = re.split('\W+', command_string)
        friend_request_name = split[0]
        # Get the player object
        player = actor.get_player()
        if player:
            # Check if the name is already in our friends list
            if not player.is_friend(friend_request_name):
                added = kernel.service_manager().social_service().add_friend(player, friend_request_name)
                if added:
                    actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_added', swgpy.ProseType.TT, friend_request_name), False, False)
                else:
                    actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_not_found', swgpy.ProseType.TT, friend_request_name), False, False)	
            else:
                actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_duplicate', swgpy.ProseType.TT, friend_request_name), False, False)
        else:
            print('Player object not found for object id' + actor.id)


	