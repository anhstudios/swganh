import swgpy.object

actor.run_speed = 11.50
actor.Controller().SendSystemMessage(swgpy.OutOfBand('cbt_spam', 'burstrun_start_single', swgpy.ProseType.TT, actor.id), False, False) 