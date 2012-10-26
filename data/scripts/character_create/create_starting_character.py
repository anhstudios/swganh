import os, sys, importlib, re
from swgpy.utility import vector3
from swgpy.object import *
from character_create.locations import *
from character_create.base_stats import *
from character_create.professions import *
from character_create.skills_species import *
from character_create.starting_items import *

import templates.object.player.shared_player as shared_player
import templates.object.tangible.datapad.shared_character_datapad as shared_character_datapad
import templates.object.tangible.inventory.shared_character_inventory as shared_character_inventory
import templates.object.tangible.bank.shared_character_bank as shared_character_bank
import templates.object.tangible.mission_bag.shared_mission_bag as mission_bag
import templates.object.tangible.hair

def CreateStartingCharacter(kernel, id, scale, base_model, full_name, profession, hair_model, start_city):
	# Get Starting Location
	startLoc = Locations[start_city]
	# Create appropriate creature object...
	species = GetSpecies(base_model)
	creature = GetCreatureObject(kernel, base_model.replace('player/', 'player/shared_'))
	creature.id = id
	creature.custom_name = full_name
	creature.position = vector3(startLoc.x, startLoc.y, startLoc.z)
	creature.bank_credits = 2000
	creature.speed_base = 5.75
	# Set Starting Stats
	print(StartingStats)
	startStats = StartStats[species + ":" + profession]
	SetStartingStats(creature, startStats)
	# Set Starting Skills
	SetStartingSkills(creature, species, profession)
	# Create datapad
	datapad = shared_character_datapad.Template().create(kernel, {})
	datapad.id = id+2
	# Create inventory
	inventory = shared_character_inventory.Template().create(kernel, {})
	inventory.id = id+3
	# Create bank
	bank = shared_character_bank.Template().create(kernel, {})
	bank.id = id+4
	# Create mission
	mission = mission_bag.Template().create(kernel, {})
	mission.id = id+5
	# Create hair
	hair = CreateHair(kernel, hair_model)
	hair.id = id+6
	# Create Base Player Object
	player = shared_player.Template().create(kernel, {})	
	player.id = id+1
	# Create Starting Items
	startingItems = starting_items.GetStartingItems(species, profession, gender)	
	# Simulation Service to add to scene
	simulation = kernel.serviceManager().simulation_service()
	simulation.AddObjectToScene(creature, startLoc.name)
	# Add all sub objects to creature (parent)
	creature.add(creature, datapad)
	creature.add(creature, inventory)
	creature.add(creature, bank)
	creature.add(creature, mission)
	creature.add(creature, hair)
	creature.add(creature, player)
	# Now add the objects to the inventory
	item_id_start = id + 10
	for item in startingItems:
		item.id = item_id_start
		item_id_start += 1
		inventory.add(inventory, item)
	
	return creature
# "object/creature/player/bothan_male.iff"	
def GetSpecies(base_model):
	print(base_model)
	match = re.search('player/(.*)_', base_model)
	return match.group(1)
	
def GetCreatureObject(kernel, base_model):
	c = importlib.import_module('templates.' + base_model.replace('/','.').replace('.iff',''))
	return c.Template().create(kernel, {})
	
def CreateHair(kernel, hair_model):
	# match = re.search('(?<=/)', hair_model)
	# hair_model = 
	h = importlib.import_module('templates.' + hair_model.replace('/','.').replace('.iff',''))
	return h.Template().create(kernel, {})

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
		