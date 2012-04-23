import swgpy.command,swgpy.object, swgpy.app
     
class proneCommand(swgpy.command.Command):
    def Run(self, kernel, actor, target, command_string):
        actor.posture = swgpy.POSTURE.PRONE
        actor.run_speed = 1.0           
