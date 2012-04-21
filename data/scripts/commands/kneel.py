import swgpy.command,swgpy.object, swgpy.app

class kneelCommand(swgpy.command.Command)
    def Run(self, kernel, actor, target, command_string):
        actor.posture = swgpy.POSTURE.CROUCHED