from swgpy.command import BaseSwgCommand, Callback

class BurstRunCommand(BaseSwgCommand):
    base_run_multiplier = 2.0
    base_run_duration_ms = 15000
    base_cooldate_timer_ms = 60000

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
        actor.SetFlag("BurstRunCooldown")
        
        # increase the actor's run speed
        actor.run_speed *= self.base_run_multiplier

        self.GetController().SendSystemMessage('cbt_spam', 'burstrun_start_single')
   
        return Callback(self.EndBurstRun, self.base_run_duration_ms)

    def EndBurstRun(self):        
        actor = self.GetActor()
        
        actor.RemoveFlag("BurstRunning")
        
        # decrease the actor's run speed by the increased amount
        actor.run_speed /= self.base_run_multiplier

        self.GetController().SendSystemMessage('cbt_spam', 'burstrun_stop_single')
        self.GetController().SendSystemMessage('combat_effects', 'burst_run_tired')
 
        return Callback(self.EndBurstRunCooldown, self.base_cooldate_timer_ms - self.base_run_duration_ms)

    def EndBurstRunCooldown(self):
        actor = self.GetActor()
        
        actor.RemoveFlag("BurstRunCooldown")
        
        self.GetController().SendSystemMessage('combat_effects', 'burst_run_not_tired')
