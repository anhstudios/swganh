Acontroller = actor.Controller()
Tcontroller = creature_target.Controller()

# Do Some Checking here
# TODO: Eventually call a CombatService function to handle this
actor.clear_auto_attack();
creature_target.clear_auto_attack();
actor.pvp_status = swganh.PVPSTATUS.PvPStatus_Player
creature_target.pvp_status = swganh.PVPSTATUS.PvPStatus_Player
actor.toggle_state_off(swganh.ACTION.COMBAT)
creature_target.toggle_state_off(swganh.ACTION.COMBAT)
actor.toggle_state_on(swganh.ACTION.PEACE)
creature_target.toggle_state_on(swganh.ACTION.PEACE)
actor.remove_defender(creature_target.id)
creature_target.remove_defender(actor.id)
actor.target_id = 0
creature_target.target_id = 0
actor.remove_duel_list(creature_target.id)
creature_target.remove_duel_list(actor.id)

#End Duel message
Acontroller.SendSystemMessage(swganh.OutOfBand("duel", "end_self", swganh.ProseType.TT, target.id), False, False)
Tcontroller.SendSystemMessage(swganh.OutOfBand("duel", "end_target", swganh.ProseType.TT, actor.id), False, False)
