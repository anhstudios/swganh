import os, sys, importlib, re
from swgpy.utility import vector3
from swgpy.simulation import *
from swgpy.object import *
from character_create.locations import *
from character_create.base_stats import *
from character_create.professions import *
from character_create.skills_species import *
from character_create.starting_items import *

def CreateStartingCharacter(kernel, id, scale, base_model, full_name, profession, hair_model, start_city):
	# Simulation Service to add to scene
	simulation = kernel.serviceManager().simulationService()
	# Set Female
	if base_model.find('female') == -1:
		gender = 'male'
	else:
		gender = 'female'
	# Get Starting Location
	# HARDCODE FOR NOW...
	startLoc = Locations['coronet']
	# Create appropriate creature object...
	species = GetSpecies(base_model)
	creature = simulation.createObject(base_model.replace('player/', 'player/shared_'), ContainerPermission.CREATURE, True, True, id)
	creature.custom_name = full_name
	creature.position = vector3(startLoc.x, startLoc.y, startLoc.z)
	creature.bank_credits = 2000
	creature.speed_base = 5.75
	# Set Starting Stats
	startStats = StartStats[species + ":" + profession]
	SetStartingStats(creature, startStats)
	# Set Starting Skills
	SetStartingSkills(creature, species, profession)
	# Create datapad
	datapad = simulation.createObject('object/tangible/datapad/shared_character_datapad.iff', ContainerPermission.CREATURE_CONTAINER, True, True, id+2)
	# Create inventory
	inventory = simulation.createObject('object/tangible/inventory/shared_character_inventory.iff', ContainerPermission.CREATURE_CONTAINER, True, True, id+3)
	# Create bank
	bank = simulation.createObject('object/tangible/bank/shared_character_bank.iff', ContainerPermission.CREATURE_CONTAINER, True, True, id+4)
	# Create mission
	mission = simulation.createObject('object/tangible/mission_bag/shared_mission_bag.iff', ContainerPermission.CREATURE_CONTAINER, True, True, id+5)
	# Create hair
	hair = simulation.createObject(hair_model.replace('/hair_','/shared_hair_'), ContainerPermission.CREATURE_CONTAINER, True, True, id+6)
	# Create Base Player Object
	player = simulation.createObject('object/player/shared_player.iff', ContainerPermission.CREATURE_CONTAINER, True, True, id+1)
	# Create Starting Items
	startingItems = GetStartingItems(species, profession, gender)	
	# Add all sub objects to creature (parent)
	creature.add(creature, datapad)
	creature.add(creature, inventory)
	creature.add(creature, bank)
	creature.add(creature, mission)
	if (hair):
		creature.add(creature, hair)
	creature.add(creature, player)
	# Now add the objects to the inventory
	item_id_start = id + 10
	# Wearables get equipped
	for item in startingItems:
		item_obj  = simulation.createObject(item, ContainerPermission.DEFAULT, True, True, item_id_start)
		item_id_start += 1
		if 'wearables' in item:
			creature.add(creature, item_obj)
		else:
			inventory.add(creature, item_obj)
	
	print(item_id_start)
	simulation.addObjectToScene(creature, startLoc.name)
	return creature
def GetSpecies(base_model):
	match = re.search('player/(.*)_', base_model)
	return match.group(1)

def SetStartingStats(creature, s):
	# Health Stats
	creature.setStatMax(STATS.HEALTH, s.health[0])
	creature.setStatMax(STATS.STRENGTH, s.health[1])
	creature.setStatMax(STATS.CONSTITUTION, s.health[2])
	creature.setStatCurrent(STATS.HEALTH, s.health[0])
	creature.setStatCurrent(STATS.STRENGTH, s.health[1])
	creature.setStatCurrent(STATS.CONSTITUTION, s.health[2])
	# Action Stats
	creature.setStatMax(STATS.ACTION, s.action[0])
	creature.setStatMax(STATS.QUICKNESS, s.action[1])
	creature.setStatMax(STATS.STAMINA, s.action[2])
	creature.setStatCurrent(STATS.ACTION, s.action[0])
	creature.setStatCurrent(STATS.QUICKNESS, s.action[1])
	creature.setStatCurrent(STATS.STAMINA, s.action[2])
	# Mind Stats
	creature.setStatMax(STATS.MIND, s.mind[0])
	creature.setStatMax(STATS.FOCUS, s.mind[1])
	creature.setStatMax(STATS.WILLPOWER, s.mind[2])
	creature.setStatCurrent(STATS.MIND, s.mind[0])
	creature.setStatCurrent(STATS.FOCUS, s.mind[1])
	creature.setStatCurrent(STATS.WILLPOWER, s.mind[2])
	
def SetStartingSkills(creature, species, profession):
	creature.addSkill(profession + '_novice')
	for skill in baseSpeciesSkills[species]:
		creature.addSkill(skill)
		