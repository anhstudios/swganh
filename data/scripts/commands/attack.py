if not actor.has_state(bindings.ACTION.COMBAT):
	actor.toggle_state_off(bindings.ACTION.PEACE);
	actor.toggle_state_on(bindings.ACTION.COMBAT)
	actor.activate_auto_attack()