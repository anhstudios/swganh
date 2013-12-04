import swgpy
from swgpy.object import *
from swgpy.command import BaseSwgCommand
from swgpy.gamesystems import *
from swgpy.object import SystemMessage
from swgpy import ACTION


class SurrenderSkillCommand(BaseSwgCommand):
    def getCommandName(self):
        return 'surrenderskill'
    
    def validate(self):
        actor = self.getActor()

        if actor.hasFlag("entertaining_music"):
            SystemMessage.sendSystemMessage(actor, 'performance', 'already_performing_self')
            return False

        if actor.hasFlag("entertaining_dance"):
            SystemMessage.sendSystemMessage(actor, 'performance', 'already_performing_self')
            return False

        return True

    def run(self):
        actor = self.getActor()

        results = self.getCommandString().split()
        result_count = len(results)
        if result_count == 1:
            print('trying to drop : ' + "{0}".format(results[0]))
            creature = actor.toCreature()
            if creature.hasSkill("{0}".format(results[0])):
                print("{0}".format(results[0])+' wurde gefunden')
                skill = "{0}".format(results[0]);
                GameSytems = self.getKernel().serviceManager().gamesystemsService()
                GameSytems.dropSkill(creature, skill )
            else:
                print("{0}".format(results[0])+' nicht gefunden')

                
        else:
            SystemMessage.sendSystemMessage(actor, 'performance', 'music_no_music_param')



