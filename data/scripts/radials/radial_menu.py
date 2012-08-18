import swgpy
from swgpy.object import *
from swgpy.sui import RadialMenu, RadialOptions, RadialOptionsList

class PyRadialMenu(RadialMenu):
    def BuildRadial(self, owner, target, radials):
        print('hi')
        radial_list = RadialOptionsList()
        print('hi again')
        if (radials):
            radial_list.append(radials)
        if (target):
            radial_list.extend( [ 
                RadialOptions(1, 68, 3, '@sui:bank_credits')                
            ])
        print('hi again')
        print(radial_list)
        owner.SetMenuResponse(radial_list)
        print('set radial menu')
        
    def HandleRadial(self, owner, target):
        print(owner)
        print(target)        