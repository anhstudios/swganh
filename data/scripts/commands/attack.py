if not actor.has_state(swganh.ACTION.COMBAT):
	actor.toggle_state_off(swganh.ACTION.PEACE);
	actor.toggle_state_on(swganh.ACTION.COMBAT)
	actor.activate_auto_attack()