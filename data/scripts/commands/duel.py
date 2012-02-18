print('I challenge you to a duel')
print(target.id)
if not actor.in_duel_list(target.id):
	actor.add_duel_list(target.id)
	actor.pvp_status = swganh.PVPSTATUS.PvPStatus_Duel
if actor.in_duel_list(target.id) and target.in_duel_list(actor.id):
	actor.pvp_status = swganh.PVPSTATUS.PvPStatus_Attackable
	target.pvp_status = swganh.PVPSTATUS.PvPStatus_Attackable
	actor.activate_auto_attack();
	target.activate_auto_attack();