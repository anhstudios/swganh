from swgpy import *

def KraytSpawn(kernel):
simulation = kernel.service_manager().simulation_service()

# Create Krayt
	krayt = simulation.createObject('object/mobile/shared_canyon_krayt_dragon.iff', ContainerPermission.Creature, False, False)
	krayt.custom_name = 'badass mofo'
	simulation.addObjectToScene(krayt, 'corellia')
	krayt.position = vector3(-137, 0, -4723)
	krayt.pvp_status = PVPSTATUS.PvPStatus_Attackable
	krayt.scale = 2
	return krayt