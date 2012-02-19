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
print('end the duel yo')