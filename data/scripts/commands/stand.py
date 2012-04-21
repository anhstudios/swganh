import swgpy.command,swgpy.object, swgpy.app
     
class standCommand(swgpy.command.Command):
    def Run(self, kernel, actor, target, command_string):
        actor.posture = swgpy.POSTURE.UPRIGHT
        actor.run_speed = 5.75      
