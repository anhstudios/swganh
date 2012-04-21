import swgpy.object
import swgpy.command,swgpy.object, swgpy.app

class sitServerCommand(swgpy.command.Command):
    def Run(self, kernel, actor, target, command_string):
        actor.posture = swgpy.POSTURE.SITTING