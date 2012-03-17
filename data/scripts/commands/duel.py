import swgpy.object

Acontroller = actor.Controller()
Tcontroller = target.Controller()
if not actor.in_duel_list(target.id):
	actor.add_duel_list(target.id)
	#system message
	Acontroller.SendSystemMessage(swgpy.OutOfBand("duel", "challenge_self", swgpy.ProseType.TT, target.id), False, False)
	Tcontroller.SendSystemMessage(swgpy.OutOfBand("duel", "challenge_target", swgpy.ProseType.TT, actor.id), False, False)
if actor.in_duel_list(target.id) and target.in_duel_list(actor.id):
	# Start The Duel
	Tcontroller.SendSystemMessage(swgpy.OutOfBand("duel", "accept_self", swgpy.ProseType.TT, actor.id), False, False)
	Acontroller.SendSystemMessage(swgpy.OutOfBand("duel", "accept_target", swgpy.ProseType.TT, target.id), False, False)
	# 
	actor.pvp_status = swgpy.PVPSTATUS.PvPStatus_Attackable
	target.pvp_status = swgpy.PVPSTATUS.PvPStatus_Attackable
	actor.activate_auto_attack();
	target.activate_auto_attack();
