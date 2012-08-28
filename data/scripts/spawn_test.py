from swgpy import *

def darth(kernel, x, y, z, planet):
	sim = kernel.service_manager().simulation_service()
	vader = sim.createObject("object/mobile/shared_darth_vader.iff", ContainerPermission.Creature, False, False)
	vader.position = vector3(x, y, z)
	vader.orientation = quat(0, 0, 0, 1)
	vader.custom_name = "Rcythr"
	sim.addObjectToScene(vader, planet)
	
def playerTest(kernel, x, y, z, planet):
	sim = kernel.service_manager().simulation_service()
	vader = sim.createObject("object/creature/player/shared_twilek_male.iff", ContainerPermission.Creature, False, False)
	vader.position = vector3(x, y, z)
	vader.orientation = quat(0, 0, 0, 1)
	vader.custom_name = "Rcythr"
	sim.addObjectToScene(vader, planet)
	