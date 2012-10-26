import os, sys, importlib, re
from locations import Locations
from base_stats import *
from professions import *
from skills_professions import *
from skills_species import *
import starting_items

sys.path.append(os.path.abspath('../'))
import object.player.shared_player
import object.creature.player
import object.tangible.datapad.shared_datapad
import object.tangible.inventory.shared_character_inventory
import object.tangible.bank.shared_character_bank
import object.tangible.mission_bag.shared_mission_bag
import object.tangible.hair

def CreateStartingCharacter(kernel, id, scale, base_model, full_name, profession, appearance_customization, hair_model, hair_customization, start_city):
	# Get Starting Location
	startLoc = Locations[start_city]
	# Create appropriate creature object...
	species = GetSpecies(base_model)
	creature = GetCreatureObject(profession, base_model)
	creature.id = id
	creature.custom_name = full_name
	creature.position = vector3(startLoc.x, startLoc.y, startLoc.z)
	creature.bank_credits = 2000
	creature.speed_base = 5.75
	# Set Starting Stats
	startStats = StartingStats[species + ":" + profession]
	SetStartingStats(creature, startStats)
	# Set Starting Skills
	SetStartingSkills(creature, species, profession)
	# Create datapad
	datapad = shared_datapad.Template().create()
	datapad.id = id+2
	# Create inventory
	inventory = shared_character_inventory.Template().create()
	inventory.id = id+3
	# Create bank
	bank = shared_character_bank.Template().create()
	bank.id = id+4
	# Create mission
	mission = shared_mission_bag.Template().create()
	mission.id = id+5
	# Create hair
	hair = CreateHair(hair_model)
	hair.id = id+6
	# Create Base Player Object
	player = shared_player.Template().create()	
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
		item.id = item_id_start++
		inventory.add(inventory, item)
	
	return creature
# "object/creature/player/shared_bothan_male.iff"	
def GetSpecies(base_model):
	match = re.search('shared_(.*)_', base_model)
	return match.group(1)
	
def GetCreatureObject(base_model):
	c = importlib.import_module(base_model.replace('/','.').replace('.iff',''))
	return c.Template().create()

def SetStartingStats(creature, s):
	# Health Stats
	creature.setMaxStat(STATS.HEALTH, s.Health[0])
	creature.setMaxStat(STATS.STRENGTH, s.Health[1])
	creature.setMaxStat(STATS.CONSTITUTION, s.Health[2])
	creature.setCurrentStat(STATS.HEALTH, s.Health[0])
	creature.setCurrentStat(STATS.STRENGTH, s.Health[1])
	creature.setCurrentStat(STATS.CONSTITUTION, s.Health[2])
	# Action Stats
	creature.setMaxStat(STATS.ACTION, s.Action[0])
	creature.setMaxStat(STATS.QUICKNESS, s.Action[1])
	creature.setMaxStat(STATS.STAMINA, s.Action[2])
	creature.setCurrentStat(STATS.ACTION, s.Action[0])
	creature.setCurrentStat(STATS.QUICKNESS, s.Action[1])
	creature.setCurrentStat(STATS.STAMINA, s.Action[2])
	# Mind Stats
	creature.setMaxStat(STATS.MIND, s.Mind[0])
	creature.setMaxStat(STATS.FOCUS, s.Mind[1])
	creature.setMaxStat(STATS.WILLPOWER, s.Mind[2])
	creature.setCurrentStat(STATS.MIND, s.Mind[0])
	creature.setCurrentStat(STATS.FOCUS, s.Mind[1])
	creature.setCurrentStat(STATS.WILLPOWER, s.Mind[2])
	
def SetStartingSkills(creature, species, profession):
	creature.addSkill(profession += '_novice')
	for skill in baseSpeciesSkills[species]:
		creature.addSkill(skill)
		