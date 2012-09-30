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
	
def trainerTest(kernel):
	sim = kernel.service_manager().simulation_service()
	trainer = sim.createObject("object/mobile/shared_dressed_architect_trainer_03.iff", ContainerPermission.Creature, False, False, 47244640935)
	trainer.position = vector3(-161, 28, -4698)
	trainer.orientation = quat(0, -0.71, 0, 0.7)
	sim.addObjectToScene(trainer, "corellia")