import swgpy.object

actor.posture = swgpy.POSTURE.SITTING
actor.Controller().SendSystemMessage(swgpy.OutOfBand('shared', 'player_sit_on_chair_prose', swgpy.ProseType.TT, actor.type), False, False)