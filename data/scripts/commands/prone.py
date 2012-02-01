class ProneCommand():
    label = "prone"
    
    def execute(self, context):
    #validation that we can transfer into posture is done before the script is run
        actor.posture = swganh.POSTURE.PRONE
        
