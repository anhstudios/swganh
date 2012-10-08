from swgpy.command import BaseSwgCommand

class SetMoodInternalCommand(BaseSwgCommand):
    def run(self):
        animation1 = [3, 43, 55, 66, 169, 145, 50, 129, 161, 151, 179] #Happy
        animation2 = [4, 147, 137, 168, 63, 155, 184, 148, 166, 170] #Angry
        animation3 = [88, 163, 141, 120, 150, 35, 28, 138, 31, 149, 154, 95, 158] #Sad
        animation4 = [33, 49,] #disgruntled
        animation5 = [159, 126, 167, 132] #worried
        animation6 = [0, 153] #none

        actor = self.getActor()
        mood = int(self.getCommandString())
        actor.mood_id = mood

        if mood in animation1:
            actor.mood_animation = "happy"
        elif mood in animation2:
            actor.mood_animation = "angry"
        elif mood in animation3:
            actor.mood_animation = "sad"
        elif mood in animation4:
            actor.mood_animation = "disgruntled"
        elif mood in animation5:
            actor.mood_animation = "worried"
        else:
            actor.mood_animation = "neutral"

        

        
