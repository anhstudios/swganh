import swgpy
import swgpy.sui

print('loaded radial menu')
help(RadialMenu)

class Radials(RadialMenu):
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