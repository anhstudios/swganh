actor.toggle_state_on(swganh.ACTION.PEACE)
if actor.has_state(swganh.ACTION.COMBAT):
	actor.toggle_state_off(swganh.ACTION.COMBAT)
	actor.toggle_state_on(swganh.ACTION.PEACE)
	actor.remove_defender(target.id)
	actor.target_id = 0
	actor.clear_auto_attack()
	if not target.has_state(swganh.ACTION.COMBAT):
		target.remove_defender(actor.id)
else:
	actor.state_bitmask = swganh.ACTION.NONE