import re, swgpy.object

split = re.split('\W+', command_string)
player_name = split[0]
# Get the player object
player = actor.get_player()
if player:
	# Check if the name is already in our friends list
	if not player.is_ignored(player_name):
		added = kernel.service_manager().social_service().add_ignore(player, player_name)
		if added:
			actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'ignore_added', swgpy.ProseType.TT, player_name), False, False)
		else:
			actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'ignore_not_found', swgpy.ProseType.TT, player_name), False, False)	
	else:
		actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'ignore_duplicate', swgpy.ProseType.TT, player_name), False, False)
else:
	print('Player object not found for object id' + actor.id)
	