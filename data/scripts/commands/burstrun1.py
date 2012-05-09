import swgpy

class BurstRunCommand(swgpy.command.BaseSwgCommand):
    #this data exists in all instances of burst run.
    base_run_multiplier = 10.0
    base_run_duration = 15.0
    base_cooldate_timer = 60.0

    def __init__(self, kernel, actor, target, command_string):
        super().__init__(self, kernel, properties, controller, command_string)

    def Validate(self):
        #if actor.has_flag("BurstRunning"):
        #    actor.Controller().SendSystemMessage(swgpy.OutOfBand('combat_effects', 'burstrun_stop_single', swgpy.ProseType.TT, actor.id), False, False)
        #    return False
        return True

    def Run(self):
        #if actor && !actor.has_flag("BurstRunning"):
        #    actor.Controller().SendSystemMessage(swgpy.OutOfBand('cbt_spam', 'burstrun_start_single', swgpy.ProseType.TT, actor.id), False, False)
        #    actor.run_speed *= base_run_multiplier

        #    # function to call, duration from now to call it
        #    return PythonCallback(EndBurstRun, self, base_run_duration)
        print('BurstRunning')

    def EndBurstRun(self):
        actor.Controller().SendSystemMessage(swgpy.OutOfBand('cbt_spam', 'burstrun_stop_single', swgpy.ProseType.TT, actor.id), False, False)
        actor.run_speed /= base_run_multiplier
