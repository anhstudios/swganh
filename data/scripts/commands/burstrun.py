import swgpy

class BurstRunCommand(swgpy.command.BaseSwgCommand):
    #this data exists in all instances of burst run.
    base_run_multiplier = 1.7
    base_run_duration = 15.0
    base_cooldate_timer = 60.0

    def Validate(self):
        #if actor.has_flag("BurstRunning"):
        #    actor.Controller().SendSystemMessage(swgpy.OutOfBand('combat_effects', 'burstrun_stop_single', swgpy.ProseType.TT, actor.id), False, False)
        #    return False
        return True

    def Run(self):
        actor = self.GetActor()
        if actor:
            actor.Controller().SendSystemMessage(swgpy.OutOfBand('cbt_spam', 'burstrun_start_single', swgpy.ProseType.TT, actor.id), False, False)
            actor.run_speed *= self.base_run_multiplier

        #    # function to call, duration from now to call it
        #    return PythonCallback(EndBurstRun, self, base_run_duration)

    def EndBurstRun(self):
        actor = self.GetActor()
        actor.Controller().SendSystemMessage(swgpy.OutOfBand('cbt_spam', 'burstrun_stop_single', swgpy.ProseType.TT, actor.id), False, False)
        actor.run_speed /= self.base_run_multiplier
