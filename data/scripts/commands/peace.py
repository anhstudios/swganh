if actor.has_state(bindings.ACTION.COMBAT):
	actor.toggle_state_off(bindings.ACTION.COMBAT)
	actor.toggle_state_on(bindings.ACTION.PEACE)
	actor.remove_defender(target.id)
	actor.target_id = 0
	actor.clear_auto_attack()
	if not target.has_state(bindings.ACTION.COMBAT):
		target.remove_defender(actor.id)
	#Send VIA ShowFlyText
	#actor.Controller().SendSystemMessage("@combat_effects:go_peace")
else:
	actor.state_bitmask = bindings.ACTION.NONE