Acontroller = actor.Controller()
Tcontroller = target.Controller()
if not actor.in_duel_list(target.id):
	actor.add_duel_list(target.id)
	#system message
	Acontroller.SendSystemMessage(bindings.OutOfBand("duel", "challenge_self", bindings.ProseType.TT, target.id), False, False)
	Tcontroller.SendSystemMessage(bindings.OutOfBand("duel", "challenge_target", bindings.ProseType.TT, actor.id), False, False)
if actor.in_duel_list(target.id) and target.in_duel_list(actor.id):
	# Start The Duel
	Tcontroller.SendSystemMessage(bindings.OutOfBand("duel", "accept_self", bindings.ProseType.TT, actor.id), False, False)
	Acontroller.SendSystemMessage(bindings.OutOfBand("duel", "accept_target", bindings.ProseType.TT, target.id), False, False)
	# 
	actor.pvp_status = bindings.PVPSTATUS.PvPStatus_Attackable
	target.pvp_status = bindings.PVPSTATUS.PvPStatus_Attackable
	actor.activate_auto_attack();
	target.activate_auto_attack();
