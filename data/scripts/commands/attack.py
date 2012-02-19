if not actor.has_state(swganh.ACTION.COMBAT):
	actor.toggle_state_on(swganh.ACTION.COMBAT)
	print(actor.state_bitmask)
	actor.activate_auto_attack()