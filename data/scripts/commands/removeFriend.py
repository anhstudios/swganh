import re, swgpy.command,swgpy.object, swgpy.app
     
class removeFriendCommand(swgpy.command.Command):
    def Run(self, kernel, actor, target, command_string):
        split = re.split('\W+', command_string)
        friend_name = split[0]

        player = actor.get_player()
        if player:
            # Check if the name is already our friend
            if player.is_friend(friend_name):
                player.remove_friend(friend_name)
                actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_removed', swgpy.ProseType.TT, friend_name), False, False)
            else:
                actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_not_found', swgpy.ProseType.TT, friend_name), False, False)

