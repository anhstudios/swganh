import swgpy.object

if creature_target:
	if creature_target.is_incap():
		creature_target.posture = swgpy.POSTURE.DEAD
		creature_target.clear_auto_attack()
		actor.target_id = 0
		actor.remove_duel_list(creature_target.id)
		creature_target.remove_duel_list(actor.id)