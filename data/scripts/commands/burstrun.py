import swgpy

class BurstRunCommand(swgpy.command.BaseSwgCommand):
    #this data exists in all instances of burst run.
    base_run_multiplier = 1.7
    base_run_duration = 1500
    base_cooldate_timer = 6000

    def Validate(self):
        actor = self.GetActor()

        if actor == None:
            return False

        if actor.HasFlag("BurstRunning"):
            self.GetController().SendSystemMessage('combat_effects', 'burst_run_no')
            return False

        if actor.HasFlag("BurstRunCooldown"):
            self.GetController().SendSystemMessage('combat_effects', 'burst_run_wait')
            return False

        # @TODO Check for mounts and whether or not in a space station/vehicle

        return True

    def Run(self):
        actor = self.GetActor()            
        actor.SetFlag("BurstRunning")
        actor.run_speed *= self.base_run_multiplier

        self.GetController().SendSystemMessage('cbt_spam', 'burstrun_start_single')
   
        return swgpy.command.Callback(self.EndBurstRun, self, self.base_run_duration)

    def EndBurstRun(self):
        actor = self.GetActor()
        actor.SetFlag("BurstRunCooldown")
        actor.RemoveFlag("BurstRunning")
        actor.run_speed /= self.base_run_multiplier

        self.GetController().SendSystemMessage('cbt_spam', 'burstrun_stop_single')
        self.GetController().SendSystemMessage('combat_spam', 'burst_run_tired')
 
        return swgpy.command.Callback(self.EndBurstRunCooldown, self, self.base_cooldate_timer)

    def EndBurstRunCooldown(self):
        actor = self.GetActor()
        self.GetController().SendSystemMessage('combat_effects', 'burst_run_not_tired')
        actor.RemoveFlag("BurstRunCooldown")
