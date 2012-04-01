import re, swgpy.object

split = re.split('\W+', command_string)
player_name = split[0]

player = actor.get_player()
if player:
	# Check if the name is in our ignore list
	if player.is_ignored(player_name):
		player.remove_ignore(player_name)
		actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'ignore_removed', swgpy.ProseType.TT, player_name), False, False)
	else:
		actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'ignore_not_found', swgpy.ProseType.TT, player_name), False, False)