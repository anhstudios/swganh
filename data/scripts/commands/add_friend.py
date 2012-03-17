import re, swgpy.object

split = re.split('\W+', command_string)
friend_request_name = split[0]
print(friend_request_name)
# Get the player object
player = actor.get_player()
if player:
	# Check if the name is already in our friends list
	if not player.is_friend(friend_request_name):
		if (kernel.SocialService().AddFriend(friend_request_name))
			actor.ObjectController().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_added', swgpy.ProseType.TT, target.id), False, False)
		else:
			actor.ObjectController().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_not_found', swgpy.ProseType.TT, target.id), False, False)	
	else:
		actor.ObjectController().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_duplicate', swgpy.ProseType.TT, target.id), False, False)

print('Player object not found for object id' + actor.id)
	