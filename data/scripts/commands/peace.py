if actor.state_bitmask == swganh.ACTION.COMBAT:
	actor.state_bitmask = swganh.ACTION.PEACE
else:
	actor.state_bitmask = swganh.ACTION.NONE