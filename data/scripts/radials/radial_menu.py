import swgpy
from swgpy.object import *
from swgpy.sui import RadialMenu, RadialOptions, RadialOptionsList

class PyRadialMenu(RadialMenu):
    def BuildRadial(self, owner, target, radials):
        radial_list = RadialOptionsList()
        if (radials):
            radial_list.append(radials)
        # if (target):
            # radial_list.extend( [ 
                    # RadialOptions(0, 3, 1, ''),
                    # RadialOptions(0, 7, 6, ''),
                    # RadialOptions(0, 21, 7, '')                    
                # ])
                
        owner.SetMenuResponse(radial_list)
        print('set radial menu')
        
    def HandleRadial(self, owner, target):
        print(owner)
        print(target)        