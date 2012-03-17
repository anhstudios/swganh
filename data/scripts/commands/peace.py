import swgpy.object

if actor.has_state(swgpy.ACTION.COMBAT):
	actor.toggle_state_off(swgpy.ACTION.COMBAT)
	actor.toggle_state_on(swgpy.ACTION.PEACE)
	actor.remove_defender(target.id)
	actor.target_id = 0
	actor.clear_auto_attack()
	if not target.has_state(swgpy.ACTION.COMBAT):
		target.remove_defender(actor.id)
	#Send VIA ShowFlyText
	#actor.Controller().SendSystemMessage("@combat_effects:go_peace")
else:
	actor.state_bitmask = swgpy.ACTION.NONE