if not actor.has_state(bindings.object.ACTION.COMBAT):
	actor.toggle_state_off(bindings.object.ACTION.PEACE);
	actor.toggle_state_on(bindings.object.ACTION.COMBAT)
	actor.activate_auto_attack()