import swgpy.command,swgpy.object, swgpy.app
     
class burstRunCommand(swgpy.command.Command):
    def Run(self, kernel, actor, target, command_string):
        if actor:
            # Check for burstrun flag
            if actor.run_speed == 5.75:
                actor.Controller().SendSystemMessage(swgpy.OutOfBand('cbt_spam', 'burstrun_start_single', swgpy.ProseType.TT, actor.id), False, False)
                actor.run_speed = 10.0

            # function to call, duration from now to call it
            event = PythonEvent(EndBurstRun, 15.0)
            kernel.event_dispatcher().dispatch(event)
    def EndBurstRun():
        actor.Controller().SendSystemMessage(swgpy.OutOfBand('cbt_spam', 'burstrun_stop_single', swgpy.ProseType.TT, actor.id), False, False)
        actor.run_speed = 5.75

