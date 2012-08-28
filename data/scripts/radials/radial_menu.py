import swgpy
from swgpy.object import *
from swgpy.sui import RadialMenu, RadialOptions, RadialOptionsList, RadialIdentifier

class PyRadialMenu(RadialMenu):
    def BuildRadial(self, owner, target, radials):
        radial_list = RadialOptionsList()
        if (radials):
            radial_list.extend(radials)
        #TEST TO SHOW HOW RADIALS WORK
        #if (target):
        #radial_list.append(RadialOptions(0, RadialIdentifier.serverMenu3, 3, '@sui:bank_credits'))
        #radial_list.append(RadialOptions(0, RadialIdentifier.serverHarvestCorpse, 3, '@sui:harvest_corpse'))
        owner.SetMenuResponse(radial_list)
    def HandleRadial(self, owner, target):
        print(owner)
        print(target)        