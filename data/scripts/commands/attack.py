import swgpy.object

if not actor.has_state(swgpy.ACTION.COMBAT):
	actor.toggle_state_off(swgpy.ACTION.PEACE);
	actor.toggle_state_on(swgpy.ACTION.COMBAT)
	actor.activate_auto_attack()