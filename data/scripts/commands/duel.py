Acontroller = actor.Controller()
Tcontroller = target.Controller()
if not actor.in_duel_list(target.id):
	actor.add_duel_list(target.id)
	#system message
	Acontroller.SendSystemMessage(swganh.OutOfBand("duel", "challenge_self", swganh.ProseType.TT, target.id), False, False)
	Tcontroller.SendSystemMessage(swganh.OutOfBand("duel", "challenge_target", swganh.ProseType.TT, actor.id), False, False)
if actor.in_duel_list(target.id) and target.in_duel_list(actor.id):
	# Start The Duel
	Tcontroller.SendSystemMessage(swganh.OutOfBand("duel", "accept_self", swganh.ProseType.TT, actor.id), False, False)
	Acontroller.SendSystemMessage(swganh.OutOfBand("duel", "accept_target", swganh.ProseType.TT, target.id), False, False)
	# 
	actor.pvp_status = swganh.PVPSTATUS.PvPStatus_Attackable
	target.pvp_status = swganh.PVPSTATUS.PvPStatus_Attackable
	actor.activate_auto_attack();
	target.activate_auto_attack();
