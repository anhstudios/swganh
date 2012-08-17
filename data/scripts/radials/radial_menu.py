import swgpy
from swgpy.sui import RadialMenu

class PythonRadialMenu(RadialMenu):
    def BuildRadial(self, owner, target, radials, action):
        print('hi')
        print(owner.id)
        print(target.id)
        print(radials)
        print(action)
        
    def HandleRadial(self, owner, target, action):
        print(owner.id)
        print(target.id)
        print(action)