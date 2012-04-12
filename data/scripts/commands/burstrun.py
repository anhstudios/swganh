import swgpy.object
import threading

if actor:

    if actor.run_speed == 5.75:
        actor.Controller().SendSystemMessage(swgpy.OutOfBand('cbt_spam', 'burstrun_start_single', swgpy.ProseType.TT, actor.id), False, False)
        actor.run_speed = 10.0
        Event = threading.Event()
