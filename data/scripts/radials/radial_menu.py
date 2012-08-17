import swgpy
from swgpy.sui import RadialMenu

class PyRadialMenu(RadialMenu):
    def BuildRadial(self, owner, target, radials):
        print('hi')
        print(owner.id)
        print(target.id)
        print(radials)
        
    def HandleRadial(self, owner, target):
        print(owner.id)
        print(target.id)        